#include "header.h"

static inline void exception(const char *msg) {
    perror(msg);
    exit(0);
}

int init(){
    FILE *file = fopen("ctrl","w");
    fprintf(file, "%d", getpid());
    printf("Susccefully init!\n");
    fclose(file);
    sleep(2);
    return 0;
}

void test_iouring(){
    int fd = open("testIOuring",O_RDWR | O_CREAT | O_DIRECT);
    // int fd = open("testIOuring",O_RDWR | O_CREAT);
    if(fd < 0){
        exception("Fail to open test file");
    }
    truncate("testIOuring", 2 * BufferSize);
    char * buf;
    int ret = posix_memalign((void **)&buf, BlockSize, BufferSize);
    if(ret != 0){
        exception("Fail to memalign alloc");
    }
    memset(buf, '@', BufferSize);
    for(int i = 0;i < IterationSize; ++i){
        struct io_uring ring;
        struct io_uring_sqe * sqe;
        struct io_uring_cqe * cqe;
        struct io_uring_cqe ** completes = malloc(sizeof(struct io_uring_cqe *) * BatchSize);
        ret = io_uring_queue_init(BatchSize * 2, &ring, IORING_SETUP_SQPOLL);
        // ret = io_uring_queue_init(BatchSize * 2, &ring, IORING_SETUP_IOPOLL);
        if(ret != 0){
            printf("Fail to init uring\n");
            exception(strerror(-ret));
        }
        for(int j = 0 ;j < BatchSize; ++j){
            sqe = io_uring_get_sqe(&ring);
            if(j % 2) io_uring_prep_read(sqe, fd, buf, BufferSize, 0);
            else io_uring_prep_write(sqe, fd, buf, BufferSize, 0);
        }
        int tmpCount = 0;
        while(1){
            tmpCount += io_uring_submit(&ring);
            if(tmpCount == BatchSize) break;
        }
        tmpCount = 0;
        while(1){
            int completed = io_uring_peek_batch_cqe(&ring, completes, BatchSize);
            if(completed == 0) continue;
            tmpCount += completed;
            for(int j = 0;j < completed; ++j){
                if(completes[j]->res < 0){
                    printf("Complete requires failed\n");
                    exception(strerror(-completes[j]->res));
                }
            }
            io_uring_cq_advance(&ring, completed); // Must be called to tell kernel cqe has been solved.
            if(tmpCount == BatchSize) break;
        }
        free(completes);
    }
    free(buf);
}

void test_blockio(){
    int fd = open("testBlockIO",O_RDWR | O_CREAT | O_DIRECT);
    // int fd = open("testBlockIO",O_RDWR | O_CREAT);
    if(fd < 0){
        exception("Fail to open test file");
    }
    truncate("testBlockIO", 2 * BufferSize);
    char * buf;
    int ret = posix_memalign((void **)&buf, BlockSize, BufferSize);
    if(ret != 0){
        exception("Fail to memalign alloc");
    }
    memset(buf, '@', BufferSize);
    for(int i = 0; i < IterationSize; ++i){
        for(int j = 0;j < BatchSize; ++j){
            lseek(fd, 0, SEEK_SET);
            DTRACE_PROBE(try, blockio_latency_in);
            if(j % 2) read(fd, buf, BufferSize);
            else write(fd, buf, BufferSize);
            DTRACE_PROBE(try, blockio_latency_out);
        }
        if(i == 2) break; // it's too slow
    }
    free(buf);
}

// void callback(__sigval_t signal_value){
//     pthread_mutex_lock(&mutex);
//     glibc_io_count += 1;
//     pthread_mutex_unlock(&mutex);
// }

void test_glibcio(){
    int fd = open("testGlibcIO",O_RDWR | O_CREAT | O_DIRECT);
    // int fd = open("testGlibcIO",O_RDWR | O_CREAT);
    if(fd < 0){
        exception("Fail to open test file");
    }
    truncate("testGlibcIO", BatchSize * BufferSize);
    char * buf;
    int ret = posix_memalign((void **)&buf, BlockSize, BatchSize * BufferSize);
    // int ret = posix_memalign((void **)&buf, BlockSize, BufferSize);
    if(ret != 0){
        exception("Fail to memalign alloc");
    }
    memset(buf, '@', BatchSize * BufferSize);

    for(int i = 0;i < IterationSize; ++i){
        struct aiocb * iocb_single;
        struct aiocb * iocbs[BatchSize];
        for(int j = 0;j < BatchSize;++j){
            iocb_single = malloc(sizeof(struct aiocb));
            memset(iocb_single,0,sizeof(struct aiocb));
            iocb_single->aio_fildes = fd;
            iocb_single->aio_offset = 0;
            iocb_single->aio_buf = buf + j * BufferSize;
            // iocb_single->aio_buf = buf;
            iocb_single->aio_nbytes = BufferSize;
            iocb_single->aio_lio_opcode = (j % 2) ? IO_CMD_PREAD : IO_CMD_PWRITE;
            iocb_single->aio_sigevent.sigev_notify = SIGEV_NONE;
            // iocb_single->aio_sigevent.sigev_notify = SIGEV_THREAD;
            // iocb_single->aio_sigevent.sigev_notify_function = callback;
            // iocb_single->aio_sigevent.sigev_value.sival_ptr = iocb_single;
            iocbs[j] = iocb_single;
        }
        ret = lio_listio(LIO_NOWAIT, iocbs, BatchSize, NULL);
        if(ret < 0){
            printf("Fail to add operations in glibc io\n");
            exception(strerror(errno));
        }
        int tmpCount = 0;
        while(1){
            ret = aio_suspend(iocbs, BatchSize, NULL);
            if(ret < 0){
                printf("aio_suspend failed !\n");
                exception(strerror(errno));
            }
            for(int j = 0;j < BatchSize;++j){
                if(iocbs[j] == NULL) continue;
                int target_error = aio_error(iocbs[j]);
                if(target_error == EINPROGRESS || target_error == ECANCELED) continue;
                if(target_error > 0){
                    printf("Error happens during I/O Operation.\n");
                    exception(strerror(errno));
                }
                assert(target_error == 0);
                tmpCount += 1;
                free(iocbs[j]);
                iocbs[j] = NULL;
            }
            if(tmpCount == BatchSize) break;
        }
    }
    free(buf);
}

void test_nativeio(){
    int fd = open("testNativeAIO",O_RDWR | O_CREAT | O_DIRECT);
    if(fd < 0){
        exception("Fail to open test file");
    }
    truncate("testNativeAIO", BatchSize * BufferSize);
    char * buf;
    int ret = 0; // todo : why use posix_memalign so slow? guess : copy_from_user()
    // int ret = posix_memalign((void **)&buf, BlockSize, BatchSize * BufferSize);
    // if(ret != 0){
    //     exception("Fail to memalign alloc");
    // }
    for(int i = 0;i < IterationSize;++i){
        buf = malloc(BatchSize * BufferSize);
        memset(buf, '@', BatchSize * BufferSize);
        io_context_t ctx;
        struct iocb ** iocbs; // Used to describe request
        struct io_event *results; // used to receive the result
        memset(&ctx, 0, sizeof(io_context_t));
        results = malloc(sizeof(struct io_event) * BatchSize);
        iocbs = malloc(sizeof(struct iocb *) * BatchSize);
        ret = io_setup(BatchSize * 2, &ctx);
        if(ret != 0){
            printf("Fail to setup aio!\n");
            exception(strerror(errno));
        }
        for(int j = 0; j < BatchSize;++j){
            iocbs[j] = malloc(sizeof(struct iocb));
            if(j % 2) io_prep_pread(iocbs[j], fd, buf + j * BufferSize, BufferSize, 0);
            else io_prep_pwrite(iocbs[j], fd, buf + j * BufferSize, BufferSize, 0);
        }
        int tmpCount = 0;
        while(1){
            int accepted = io_submit(ctx, BatchSize, iocbs);
            if(accepted < 0){
                printf("Fail to does io_submit in aio!\n");
                exception(strerror(-accepted));
            }
            tmpCount += accepted;
            // printf("Iteration: %d, accepted : %d\n",i, accepted);
            if(tmpCount == BatchSize) break;
        }
        tmpCount = 0;
        while(1){
            int completed = io_getevents(ctx, 0, BatchSize, results, NULL);
            if(completed < 0){
                printf("Fail to get io events!\n");
                exception(strerror(-completed));
            }
            if(completed == 0) continue;
            tmpCount += completed;
            for(int j = 0; j < completed;++j){
                if(results[j].res < 0){
                    printf("Something wrong when doing operations.\n");
                    exception(strerror(errno));
                }
            }
            if(tmpCount == BatchSize) break;
        }
        free(results);
        io_destroy(ctx);
        for(int j = 0;j < BatchSize;++j) free(iocbs[j]);
        free(iocbs);
        free(buf);
    }
}

int main(){
    init();
    test_nativeio();
    sleep(1);
    // test_iouring();
    sleep(1);
    // test_blockio();
    sleep(1);
    // test_glibcio();
    printf("Press Ctrl+c to print result\n");
    printf("[BufferSize : %d, BatchSize : %d, Iterations : %d]\n",BufferSize, BatchSize, IterationSize);
    return 0;
}