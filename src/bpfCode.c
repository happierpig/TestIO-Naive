#include <uapi/linux/ptrace.h>

BPF_HASH(map_iouring_submit_in_time);

int bpf_iouring_submit_in(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_iouring_submit_in_time.lookup(&key);
    if(val == NULL){
        map_iouring_submit_in_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_iouring_submit_in_time.update(&key, &cnt);
    }
    map_iouring_submit_in_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_iouring_submit_out_time);

int bpf_iouring_submit_out(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_iouring_submit_out_time.lookup(&key);
    if(val == NULL){
        map_iouring_submit_out_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_iouring_submit_out_time.update(&key, &cnt);
    }
    map_iouring_submit_out_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_iouring_peek_in_time);

int bpf_iouring_peek_in(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_iouring_peek_in_time.lookup(&key);
    if(val == NULL){
        map_iouring_peek_in_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_iouring_peek_in_time.update(&key, &cnt);
    }
    map_iouring_peek_in_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_iouring_peek_out_time);

int bpf_iouring_peek_out(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_iouring_peek_out_time.lookup(&key);
    if(val == NULL){
        map_iouring_peek_out_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_iouring_peek_out_time.update(&key, &cnt);
    }
    map_iouring_peek_out_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_blockio_latency_in_time);

int bpf_blockio_latency_in(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_blockio_latency_in_time.lookup(&key);
    if(val == NULL){
        map_blockio_latency_in_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_blockio_latency_in_time.update(&key, &cnt);
    }
    map_blockio_latency_in_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_blockio_latency_out_time);

int bpf_blockio_latency_out(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_blockio_latency_out_time.lookup(&key);
    if(val == NULL){
        map_blockio_latency_out_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_blockio_latency_out_time.update(&key, &cnt);
    }
    map_blockio_latency_out_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_glibcio_submit_in_time);

int bpf_glibcio_submit_in(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_glibcio_submit_in_time.lookup(&key);
    if(val == NULL){
        map_glibcio_submit_in_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_glibcio_submit_in_time.update(&key, &cnt);
    }
    map_glibcio_submit_in_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_glibcio_submit_out_time);

int bpf_glibcio_submit_out(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_glibcio_submit_out_time.lookup(&key);
    if(val == NULL){
        map_glibcio_submit_out_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_glibcio_submit_out_time.update(&key, &cnt);
    }
    map_glibcio_submit_out_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_glibcio_peek_in_time);

int bpf_glibcio_peek_in(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_glibcio_peek_in_time.lookup(&key);
    if(val == NULL){
        map_glibcio_peek_in_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_glibcio_peek_in_time.update(&key, &cnt);
    }
    map_glibcio_peek_in_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_glibcio_peek_out_time);

int bpf_glibcio_peek_out(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_glibcio_peek_out_time.lookup(&key);
    if(val == NULL){
        map_glibcio_peek_out_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_glibcio_peek_out_time.update(&key, &cnt);
    }
    map_glibcio_peek_out_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_nativeio_submit_in_time);

int bpf_nativeio_submit_in(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_nativeio_submit_in_time.lookup(&key);
    if(val == NULL){
        map_nativeio_submit_in_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_nativeio_submit_in_time.update(&key, &cnt);
    }
    map_nativeio_submit_in_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_nativeio_submit_out_time);

int bpf_nativeio_submit_out(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_nativeio_submit_out_time.lookup(&key);
    if(val == NULL){
        map_nativeio_submit_out_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_nativeio_submit_out_time.update(&key, &cnt);
    }
    map_nativeio_submit_out_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_nativeio_peek_in_time);

int bpf_nativeio_peek_in(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_nativeio_peek_in_time.lookup(&key);
    if(val == NULL){
        map_nativeio_peek_in_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_nativeio_peek_in_time.update(&key, &cnt);
    }
    map_nativeio_peek_in_time.insert(&cnt, &time);
    return 0;
}

BPF_HASH(map_nativeio_peek_out_time);

int bpf_nativeio_peek_out(struct pt_regs *ctx) {
    u64 time = bpf_ktime_get_ns();
    u64 key = 0,zero = 1,cnt;
    u64 * val;
    val = map_nativeio_peek_out_time.lookup(&key);
    if(val == NULL){
        map_nativeio_peek_out_time.insert(&key,&zero);
        cnt = 1;
    }else{
        cnt = (*val) + 1;
        map_nativeio_peek_out_time.update(&key, &cnt);
    }
    map_nativeio_peek_out_time.insert(&cnt, &time);
    return 0;
}