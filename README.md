## TestIO-Naïve

> CS2952 Homework : A project to test performance differences of different asynchronous I/O

### Environment

1. BPF Compiler Collection([BCC](https://github.com/iovisor/bcc))

   Install from source code. See tutorial [here](https://github.com/iovisor/bcc/blob/master/INSTALL.md#ubuntu---source)

   And enable USDT

   ```shell
   $ sudo apt-get install systemtap-sdt-dev
   ```

2. Flexible I/O Tester([fio](https://github.com/axboe/fio))

   See tutorial [here](https://github.com/axboe/fio#building)

3. Flame Graph and perf

   For detailed usage : [here](https://zhuanlan.zhihu.com/p/402188023)

   ```shell
   $ git clone https://github.com/brendangregg/FlameGraph
   $ sudo apt install linux-tools-common
   ```

4. Install IO library

   ``` shell
   $ sudo apt-get install libaio-dev
   $ git clone https://github.com/axboe/liburing.git
   $ cd liburing && make && make all
   $ sudo cp -r ../liburing /usr/share
   $ sudo cp src/liburing.so.2.2 /usr/lib/x86_64-linux-gnu/liburing.so
   $ sudo cp src/liburing.so.2.2 /usr/lib/x86_64-linux-gnu/liburing.so.2
   ```

### Method

We focus on the **latency** and **IOPS** (I/O Operations Per Second) performance of I/O library which may be two main dimensions that people are concerned about. And all tests are done with flag **O_DIRECT** in order to avoid cache interference except small read/write operations imitating **zero-delay** cases.

1. Latency

   **BCC** is used to write eBPF tracing program to detect the running time of specific kernel functions, user functions and code segments. Namely **uprobe** and **uretprobe** are used to catch I/O library functions. **USDT** are defined to trace specific code segments. For example : (code in `src/bpfCode.c` and `src/test.py`)

   ```python
   b.attach_uprobe(name="uring", sym="io_uring_submit", fn_name="bpf_iouring_submit_in")
   b.attach_uretprobe(name="uring", sym="io_uring_submit", fn_name="bpf_iouring_submit_out")
   ```

2. Throughput(IOPS)

   **Fio** is used to perform pressure testing on different I/O library. We control the parameters to implement the comparison experiment. The detailed instruction is in `res/bench.md`. The sample is below : 

   ```shell
   dreamer@ubuntu:~/Desktop/try/bench$ fio -thread -size=1G -bs=4k -direct=1 -rw=randwrite -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=io_uring --iodepth=128
   ```

3. Call Stack Analysis

   We use **perf** and **FlameGraph** to generate the flame graph of the procedure. 

   ```shell
   $ sudo perf record -F 99 -p `pid of test program` -g -- sleep 30
   $ sudo perf script -i perf.data &> perf.unfold
   $ sudo ./FlameGraph/stackcollapse-perf.pl perf.unfold &> perf.folded
   $ sudo ./FlameGraph/flamegraph.pl perf.folded > perf.svg
   ```

   And the sample graph is like :

   ![](./res/nativeaio.svg)

### Analysis



### Reference

1. [io_uring manpage](https://unixism.net/loti/index.html#)

2. [Blog of io_uring](https://thenewstack.io/how-io_uring-and-ebpf-will-revolutionize-programming-in-linux)

3. [Another blog of io_uring](https://zhuanlan.zhihu.com/p/380726590)

4. [bcc repo](https://github.com/iovisor/bcc/blob/master/docs/tutorial_bcc_python_developer.md)