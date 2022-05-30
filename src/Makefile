INCLUDE_PATH = -I/usr/share/liburing/src/include
LIBRARY_PATH = -L/usr/lib/x86_64-linux-gnu
LIB = -luring -lrt -laio



build:	test.c
	clang -o test $(INCLUDE_PATH) $(LIBRARY_PATH) $(LIB) $?

clean:
	rm -f test ctrl testBlockIO testGlibcIO testIOuring testNativeAIO
run:
	sudo ./test & sleep 1 && sudo python3 test.py
.DEFAULT_GOAL := build
