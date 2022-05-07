//PS IS1 324 LAB07
//Igor Stodolny
//si46655@zut.edu.pl
#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <crypt.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
static int thread_number = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char *hashed_passwd;
char *salt;
static int lines_checked;
static int lines_overall = 1000;
static bool found = false;
static bool run = true;
int nr_of_threads_left;

char *get_hash(char* hash)
{
  int counter = 0;
  int dollar = 0;
  char temp[200];
  int i = 0;
  while(hash[i] != '\0')
  {
    if(dollar == 3)
    {
      temp[counter] = hash[i];
      counter++;
    }
    if(hash[i] == '$')
    {
      dollar++;
    }


    i++;
  }
  char *new_hash = (char*)malloc(sizeof(char)*counter + 1);
  for(i = 0; i < counter; i++)
  {
    new_hash[i] = temp[i];
  }
  new_hash[counter] = '\0';

  return new_hash;
}

char* get_salt(char *hash)
{
  int counter = 0;
  int dollar = 0;
  char temp[200];
  int i = 0;
  while(hash[i] != '\0')
  {
    if(hash[i] == '$')
    {
      dollar++;
    }
    if(dollar < 3)
    {
      temp[counter] = hash[i];
      counter++;
    }



    i++;
  }
  char *new_salt = (char*)malloc(sizeof(char)*counter + 1);
  for(i = 0; i < counter; i++)
  {
    new_salt[i] = temp[i];
  }
  new_salt[counter] = '\0';

  return new_salt;
}



char* get_chunk(char *buffer, int *start, int *end, const int max_size, int *length, int thread)
{
  //printf("PRZED start = %d, end = %d, length = %d\n", *start, *end, *length);
  //printf("-----THREAD = %d------\n", thread);
  if(thread == thread_number)
  {
    //printf("OSTATNI FRAGMENT\n");
    *end = max_size - 1;
    int chunk_size = *end-*start;

    char *chunk = (char*)malloc(sizeof(char)*chunk_size+1);
    for(int i = 0 ; i < chunk_size; i++)
    {
      chunk[i] = buffer[*start+i];
    }
    //printf("PO start = %d, end = %d, length = %d\n\n", *start, *end, *length);
    //printf("%s\n", chunk);
    chunk[chunk_size] = '\0';
    return chunk;

  }
  if(buffer[*end] == '\n')
  {
    *end -= 1;
  }
  else if(buffer[*end - 1] == '\n')
  {
    *end -= 2;
  }
  else if(buffer[*end] != '\n' && buffer[*end - 1] != '\n' && buffer[*end + 1] != '\n')
  {
    int i = 1;
    while(buffer[*end-i] != '\n')
    {
         i++;
    }
       *end -= i;

  }

  if(*start > * end)
  {
    int i = 1;
    while(buffer[*start+i] != '\n')
    {
         i++;
    }
       *end = *start + i;
  }



  if(buffer[*end] == '\n')
  {
    *end -= 1;
  }
  //printf("PO start = %d, end = %d, length = %d\n\n", *start, *end, *length);
  int chunk_size = *end-*start;
  //printf("%d\n", chunk_size);
  char *chunk = (char*)malloc(sizeof(char)*chunk_size+2);
  for(int i = 0 ; i < chunk_size + 1; i++)
  {
    chunk[i] = buffer[*start+i];
  }

  *start = *end + 2;

  if(*start + *length < max_size)
  {
    *end = *start + *length;
  }
  else
  {
    *end = max_size - 1;
  }
  chunk[chunk_size + 1] = '\0';

  return chunk;
}

void *crack(void *args)
{

  if(found)
  {
    return NULL;
  }
  char* chunk = (char*)args;
  int nr_of_lines = 0;
  // pthread_mutex_lock(&lock);
  // printf("HASLA %s\n", chunk);
  // pthread_mutex_unlock(&lock);
  int counter = 0;
  while(true)
  {
    if(chunk[counter] == '\0')
    {
      nr_of_lines++;
      break;
    }
    else if(chunk[counter] == '\n')
    {
      nr_of_lines++;
    }

    counter++;
  }
  counter = 0;
  int new_start = 0;
  char *only_hashed_passwd = get_hash(hashed_passwd);
  char *only_salt = get_salt(hashed_passwd);

  bool stop = false;
  //printf("liczba lin = %d\n", nr_of_lines);
  for(int i = 0; i < nr_of_lines; i++)
  {
    pthread_mutex_lock(&lock);
    lines_checked++;
    pthread_mutex_unlock(&lock);

    while(true)
    {
      if(chunk[counter + new_start] == '\n' || chunk[counter + new_start] == '\0')
      {
        break;
      }
      counter++;
    }

    char* line = (char*)malloc(sizeof(char)*counter);

    for(int j = 0 ; j < counter; j++)
    {
      line[j] = chunk[new_start+j];
    }



    if(strcmp(get_hash(crypt(line, only_salt)), only_hashed_passwd)==0)
    {
      found = true;
      printf("ZNALEZIONO %s\n", line);
    }

    if(found)
    {
      return NULL;
    }

    new_start += counter + 1;
    counter = 0;
  }
  pthread_mutex_lock(&lock);
  nr_of_threads_left--;
  pthread_mutex_unlock(&lock);
}




int main(int argc, char *argv[])
{
  int opt;
  char *passwords;
  char *file_with_hashed_passwd;
  while((opt = getopt(argc, argv, "p:f:t:"))!= -1)
  {
      switch(opt)
      {
          case 'p' : file_with_hashed_passwd = optarg; break;
          case 'f' : passwords = optarg; break;
          case 't' : thread_number = atoi(optarg); break;
          default: abort();
      }
  }
  int p1 = open(file_with_hashed_passwd, O_RDONLY);
  struct stat sb2;
  if(fstat(p1, &sb2) == -1)
  {
    printf("Cant get file size.\n");
    return -1;
  }

  char *buffer2 = (char*)malloc(sizeof(char)*sb2.st_size);
  read(p1, buffer2, sb2.st_size);
  hashed_passwd = buffer2;

  const long available_threads = sysconf(_SC_NPROCESSORS_CONF);
  if(thread_number > available_threads)
  {
    thread_number = available_threads;
  }


  int fd = open(passwords, O_RDONLY);
  //int fd = open("test.txt", O_RDONLY);
  struct stat sb;
  if(fstat(fd, &sb) == -1)
  {
    printf("Cant get file size.\n");
    return -1;
  }
  char *buffer = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  int start_pos = 0;

  if(thread_number != 0)
  {
    int end = sb.st_size/thread_number - 1;
    int length = sb.st_size/thread_number - 1;
    pthread_t *threads_id = (pthread_t *)malloc(sizeof(pthread_t)*thread_number);
    char **chunks = malloc(sizeof(char*)*thread_number);
    for(int i = 0 ; i < thread_number; i++)
    {
      //char *chunk = get_chunk(buffer, &start_pos, &end, sb.st_size, &length, i+1);
      chunks[i] = get_chunk(buffer, &start_pos, &end, sb.st_size, &length, i+1);
      //pthread_create(&threads_id[i], NULL, crack, chunk);
      pthread_create(&threads_id[i], NULL, crack, chunks[i]);
    }

    while(!found && lines_checked != lines_overall)
    {
      for(int i = 0 ; i < thread_number; i++)
      {
        pthread_join(threads_id[i], NULL);
      }
    }

    if(!found)
    {
      printf("Nie znaleziono hasla\n");
      double status = ((double)lines_checked/lines_overall)*100;
      printf("%f%%\n", status);
    }


    for(int i = 0 ; i < thread_number; i++)
    {
      free(chunks[i]);
    }

    free(chunks);
    //munmap(&buffer, sb.st_size);
    free(threads_id);


}
else
{
  pthread_t *threads_id;
  //printf("Zerowa ilosc watkow\n");
  struct timeval tv1, tv2;
  for(int i = 0; i < available_threads; i++)
  {
    //printf("----PETLA %d----\n", i);
    thread_number = i+1;
    nr_of_threads_left = i+1;
    lines_checked = 0;
    int end = sb.st_size/(i+1) - 1;
    int length = sb.st_size/(i+1) - 1;
    threads_id = (pthread_t *)malloc(sizeof(pthread_t)*i+1);
    char **chunks = malloc(sizeof(char*)*thread_number);
    gettimeofday(&tv1, NULL);
    for(int j = 0 ; j < i+1; j++)
    {
      //char *chunk = get_chunk(buffer, &start_pos, &end, sb.st_size, &length, j+1);
      //printf("HASLA %s\n", chunk);
      chunks[j] = get_chunk(buffer, &start_pos, &end, sb.st_size, &length, j+1);
      pthread_create(&threads_id[j], NULL, crack, chunks[j]);
      //pthread_create(&threads_id[j], NULL, crack, chunk);
    }

    while(!found && lines_checked < lines_overall && nr_of_threads_left > 0)
    {
      //printf("%d\n", lines_checked);
      for(int j = 0 ; j < i+1; j++)
      {
        pthread_join(threads_id[j], NULL);
      }
    }
    gettimeofday(&tv2, NULL);
    double time_spend = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000.0 + (double)(tv2.tv_sec - tv1.tv_sec);
    printf("%d watkow potrzebowalo %lfs\n", i+1, time_spend);
    if(found)
    {
      found = false;
      free(threads_id);
      start_pos = 0;
      double status = lines_checked/lines_overall;
      printf("%f%%\n", status);
      lines_checked = 0;


    }
    else
    {
      printf("Nie znaleziono hasla\n");
      free(threads_id);
      //printf("%d\n", lines_checked);
      double status = ((double)lines_checked/lines_overall)*100;
      printf("%f%%\n", status);
      start_pos = 0;

    }



    for(int j = 0 ; j < thread_number; j++)
    {

      free(chunks[j]);

    }

    free(chunks);

  }
}





  free(buffer2);
  munmap(&buffer, sb.st_size);
  close(fd);
  close(p1);


  return 0;
}
