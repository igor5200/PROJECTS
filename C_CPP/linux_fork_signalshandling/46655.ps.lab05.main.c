//PS IS1 324 LAB05
//Igor Stodolny
//si46655@zut.edu.pl
#include <sys/times.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int counter = 0;
bool run = true;
bool flag = true;
int lifetime = 0;
int interval = 0;

void parent_sig_handler(int signal, siginfo_t *info, void *ucontext)
{
  if(signal == (SIGRTMIN + 1))
  {
    char buff[100];
    time_t now = time(0);
    strftime(buff, 100, "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("\t \t[%d] [%d] [%s]\n", info->si_pid, info->si_status, buff);
    counter--;

  }
  else if(signal == SIGINT)
  {
    run = false;
  }
}



void child_sig_handler(int signal, siginfo_t *info, void *ucontext)
{
  printf("Koniec dziecka\n");
  flag = false;
}


int main(int argc, char *argv[])
{
  int opt, lifetime, interval;
  bool create_start = true;
  bool create_process = true;
  while((opt = getopt(argc, argv, "w:m:"))!= -1)
  {
    switch(opt)
    {
      case 'w' : lifetime =atoi(optarg); break;
      case 'm' : interval = atoi(optarg); break;
      default: abort();
    }
  }

  struct sigaction sa;
  sa.sa_sigaction = parent_sig_handler;
  sigemptyset(&(sa.sa_mask));
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGRTMIN + 1, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  struct timespec start;
  struct timespec end;

  int time_left = 0;

  pid_t child;
  while(run)
  {
      if(create_start)
      {
        clock_gettime(CLOCK_REALTIME, &start);
        create_start = false;
      }
      if(create_process)
      {
        counter++;
        child = fork();
        if(child == 0)
        {
          int sum = 0;
          srand(time(NULL));
          int choice = (rand()%lifetime)+1;


          struct sigaction sa;
          sigset_t iset;
          sa.sa_sigaction = child_sig_handler;
          sigemptyset(&iset);
          sigaddset(&iset, SIGINT);
          sigprocmask(SIG_BLOCK, &iset, NULL);
          sigemptyset(&(sa.sa_mask));
          sa.sa_flags = SA_SIGINFO;
          sigaction(SIGALRM, &sa, NULL);



          char buff[100];
          time_t now = time(0);
          strftime(buff, 100, "%Y-%m-%d %H:%M:%S", localtime(&now));
          printf("\n[%d] [%d] [%s]\n", getpid(), choice, buff);

          alarm(choice);
          while(flag)
          {
            sum += 1;
          }
          union sigval sv;
          sv.sival_int = choice;
          sigqueue(getppid(), SIGRTMIN + 1, sv);
          exit(choice);
        }
      }
      clock_gettime(CLOCK_REALTIME, &end);
      float time_waited = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1000000000.0;

      if(time_waited >= interval)
      {
        create_process = true;
        create_start = true;
      }
      else
      {
        create_process = false;
      }
  }

  while(counter != 0);





  return 0;
}
