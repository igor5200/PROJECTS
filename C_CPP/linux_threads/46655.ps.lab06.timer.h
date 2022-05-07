#include <sys/times.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

static pthread_key_t Key_start;

static pthread_once_t Once_start = PTHREAD_ONCE_INIT;

struct timespec *begin;

static void freememory(void *);

static void createKey(void);

void start();

float stop();
