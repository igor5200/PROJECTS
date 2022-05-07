//PS IS1 324 LAB06
//Igor Stodolny
//si46655@zut.edu.pl
#include "46655.ps.lab06.timer.h"




static void freememory(void *buffer)
{
  free(buffer);
}

static void createKey(void)
{
  pthread_key_create(&Key_start, freememory);
}

void start()
{
  struct timespec temp;
  clock_gettime(CLOCK_REALTIME, &temp);
  pthread_once(&Once_start, createKey);
  begin = pthread_getspecific(Key_start);
  if(begin == NULL)
  {
    begin = malloc(sizeof(struct timespec));
    pthread_setspecific(Key_start, begin);
  }
  *begin = temp;
}

float stop()
{
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);
    struct timespec *started_time = pthread_getspecific(Key_start);

    return((end.tv_sec - started_time->tv_sec) + (end.tv_nsec - started_time->tv_nsec)/1000000000.0);
}
