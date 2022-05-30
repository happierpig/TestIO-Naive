from __future__ import print_function
from bcc import BPF, USDT
from time import sleep



pid = open('ctrl','r').readline()

# load BPF program
bpf_text = open('bpfCode.c','r').read()

# enable USDT probe from given PID
u = USDT(pid=int(pid))
u.enable_probe(probe="blockio_latency_in", fn_name="bpf_blockio_latency_in")
u.enable_probe(probe="blockio_latency_out", fn_name="bpf_blockio_latency_out")


# initialize BPF
b = BPF(text=bpf_text, usdt_contexts=[u])

# enable uprobe and uretprobe
b.attach_uprobe(name="uring", sym="io_uring_submit", fn_name="bpf_iouring_submit_in")
b.attach_uretprobe(name="uring", sym="io_uring_submit", fn_name="bpf_iouring_submit_out")

b.attach_uprobe(name="uring", sym="io_uring_peek_batch_cqe", fn_name="bpf_iouring_peek_in")
b.attach_uretprobe(name="uring", sym="io_uring_peek_batch_cqe", fn_name="bpf_iouring_peek_out")

b.attach_uprobe(name="rt", sym="lio_listio", fn_name="bpf_glibcio_submit_in")
b.attach_uretprobe(name="rt", sym="lio_listio", fn_name="bpf_glibcio_submit_out")

b.attach_uprobe(name="rt", sym="aio_suspend", fn_name="bpf_glibcio_peek_in")
b.attach_uretprobe(name="rt", sym="aio_suspend", fn_name="bpf_glibcio_peek_out")

b.attach_uprobe(name="aio", sym="io_submit", fn_name="bpf_nativeio_submit_in")
b.attach_uretprobe(name="aio", sym="io_submit", fn_name="bpf_nativeio_submit_out")

b.attach_uprobe(name="aio", sym="io_getevents", fn_name="bpf_nativeio_peek_in")
b.attach_uretprobe(name="aio", sym="io_getevents", fn_name="bpf_nativeio_peek_out")

def getAvgIntervalTime(map_start,map_finish):
	start = []
	finish = []
	totalTime = 0.0
	size = 0
	for _, v in map_start.items():
		start.append(v.value)
		size = size + 1
	for _, v in map_finish.items():
		finish.append(v.value)
	start.sort(reverse=False)
	finish.sort(reverse=False)	
	for i in range(size):
		totalTime += finish[i] - start[i]
		#print("(%d,%d)->%d\n" % (start[i],finish[i],finish[i]-start[i]))
	# print("Map Size : %d \n" % size)
	return totalTime / (size-1)

try:
	sleep(9999999)
except KeyboardInterrupt:
	pass

iouring_submit = getAvgIntervalTime(b["map_iouring_submit_in_time"], b["map_iouring_submit_out_time"])
iouring_peek = getAvgIntervalTime(b["map_iouring_peek_in_time"], b["map_iouring_peek_out_time"])
blockIO_latency = getAvgIntervalTime(b["map_blockio_latency_in_time"], b["map_blockio_latency_out_time"])
glibcIO_submit = getAvgIntervalTime(b["map_glibcio_submit_in_time"], b["map_glibcio_submit_out_time"])
glibcIO_peek = getAvgIntervalTime(b["map_glibcio_peek_in_time"], b["map_glibcio_peek_out_time"])
nativeAIO_submit = getAvgIntervalTime(b["map_nativeio_submit_in_time"], b["map_nativeio_submit_out_time"])
nativeAIO_peek  = getAvgIntervalTime(b["map_nativeio_peek_in_time"], b["map_nativeio_peek_out_time"])
print("\n%24s %16s %16s %16s" % ("iouring", "aio", "posix_io", "block_io"))
print("submit:%17f %16f %16f %16s" % (iouring_submit,nativeAIO_submit,glibcIO_submit,"--"))
print("peek:%19f %16f %16f %16s" % (iouring_peek,nativeAIO_peek,glibcIO_peek,"--"))
print("total:%18f %16f %16f %16f" % (iouring_peek+iouring_submit,nativeAIO_peek+nativeAIO_submit,glibcIO_peek+glibcIO_submit,blockIO_latency))
