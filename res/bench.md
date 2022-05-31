```
[BufferSize : 4096, BatchSize : 16, Iterations : 32]
                 iouring              aio         posix_io         block_io
submit:      4074.593750     82388.875000      7371.687500               --
peek:        3300.757301      6502.343750    147535.906250               --
total:       7375.351051     88891.218750    154907.593750    217535.104167
```

```
[BufferSize : 512, BatchSize : 32, Iterations : 32]
                 iouring              aio         posix_io         block_io
submit:      3216.843750     64251.781250     20156.718750               --
peek:        5324.594882      3699.187500    111051.172925               --
total:       8541.438632     67950.968750    131207.891675    178777.406250
```

```
[BufferSize : 32, BatchSize : 16, Iterations : 32]
                 iouring              aio         posix_io         block_io
submit:      5518.312500        -0.000000     40848.031250               --
peek:        6723.820686        -0.000000    166963.187500               --
total:      12242.133186        -0.000000    207811.218750     26320.916667
```

```
[BufferSize : 1, BatchSize : 32, Iterations : 32]
                 iouring              aio         posix_io         block_io
submit:      3998.531250        -0.000000     10101.125000               --
peek:        4566.127454        -0.000000     18526.364258               --
total:       8564.658704        -0.000000     28627.489258      3130.614583
```

```
fio -thread -size=1G -bs=1k -direct=1 -rw=randread -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=io_uring --iodepth=128 --sqthread_poll 1

iops: min=13234, max=18676, avg=15744.11, stdev=563.71
cpu: usr=93.85%, sys=1.23%, ctx=2588
```

```
fio -thread -size=1G -bs=1k -direct=1 -rw=randread -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=io_uring --iodepth=128 --sqthread_poll 0

iops: min=11664, max=13234, avg=12980.54, stdev=188.90
cpu: usr=4.08%, sys=95.81%, ctx=2388
```

```
fio -thread -size=1G -bs=1k -direct=0 -rw=randread -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=io_uring --iodepth=128 --sqthread_poll 1

iops: min=14864, max=60104, avg=36792.82, stdev=14575.98
```

```
fio -thread -size=1G -bs=1k -direct=0 -rw=randread -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=io_uring --iodepth=128 --sqthread_poll 0

iops: min= 7278, max=39504, avg=23161.97, stdev=9767.98
```

```
fio -thread -size=1G -bs=1k -direct=1 -rw=randread -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=libaio --iodepth=128

iops: min= 9753, max=12605, avg=12116.24, stdev=481.37
cpu: usr=4.51%, sys=95.44%, ctx=2344
```

```
fio -thread -size=1G -bs=1k -direct=1 -rw=randread -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=posixaio --iodepth=128

iops: min= 2716, max= 4514, avg=3616.70, stdev=322.52
cpu: usr=1.50%, sys=0.16%, ctx=120093
```

```
fio -thread -size=1G -bs=1k -direct=1 -rw=randread -name=test -group_reporting -filename=./io.tmp -runtime 600 --ioengine=sync --iodepth=128

iops: min= 2165, max= 4172, avg=3115.22, stdev=279.05
cpu: usr=3.51%, sys=34.36%, ctx=1046140
```

