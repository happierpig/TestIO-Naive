#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/sdt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
#include <aio.h>
#include <libaio.h>
#include <pthread.h>
#include "liburing.h"

#define BufferSize 4096
#define BlockSize 512
#define BatchSize 16
#define IterationSize 32
