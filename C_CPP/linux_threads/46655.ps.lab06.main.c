//PS IS1 324 LAB06
//Igor Stodolny
//si46655@zut.edu.pl
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
#include "46655.ps.lab06.timer.h"



bool run = true;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int temp_quantity = 0;
void handler(int sig)
{
  //printf("TID=%ld\n", pthread_self());
  //start();

  char buff[100];
  time_t now = time(0);
  strftime(buff, 100, "%Y-%m-%d %H:%M:%S", localtime(&now));
  printf("\t \t[%ld]  [%s] [%lf]\n", pthread_self(), buff, stop());

  pthread_mutex_lock(&mutex);
  temp_quantity--;
  pthread_mutex_unlock(&mutex);

}

void* count(void *arg)
{
  start();
  int i = 0;
  struct sigaction act;
  act.sa_handler = handler;
  act.sa_flags = 0;
  sigemptyset(&(act.sa_mask));
  sigaction(SIGUSR1, &act, NULL);
  while(run)
  {
    i++;
  }

  return NULL;
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    int opt, max_life_time, quantity;
    while((opt = getopt(argc, argv, "w:m:"))!= -1)
    {
        switch(opt)
        {
            case 'w' : max_life_time = atoi(optarg); break;
            case 'm' : quantity = atoi(optarg); break;
            default: abort();
        }
    }

    int *tab_time = (int*)malloc(quantity*sizeof(int));
    pthread_t *threads_id = (pthread_t *)malloc(quantity*sizeof(pthread_t));
    temp_quantity = quantity;



    for(int i = 0 ; i < quantity; i++)
    {

        pthread_create(&threads_id[i], NULL, count, NULL);
        tab_time[i] = (rand()%max_life_time) + 1;
        char buff[100];
        time_t now = time(0);
        strftime(buff, 100, "%Y-%m-%d %H:%M:%S", localtime(&now));
        printf("\n[%ld] [%d] [%s]\n", threads_id[i], tab_time[i], buff);
    }
    start();
    while(temp_quantity > 0)
    {
        for(int i = 0; i < quantity; i++)
        {
            if(tab_time[i] != -1 && stop() >= tab_time[i])
            {
                pthread_kill(threads_id[i], SIGUSR1);
                tab_time[i] = -1;
            }
        }
    }

    // for(int i = 0; i < quantity; i++)
    // {
    //     pthread_join(threads_id[i], NULL);
    // }



    free(tab_time);
    free(threads_id);
}
