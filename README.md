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

### Report

