//PS IS1 324 LAB03
//Igor Stodolny
//si46655@zut.edu.pl
#include <utmpx.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include "46655.ps.lab03.static.lib.h"
void show_users()
{
  const struct utmpx *entry = getutxent();
  while(entry != NULL)
  {
    if(entry->ut_type == 7)
    {
      printf("%s\n", entry->ut_user);
    }
    entry = getutxent();
  }
}

void show_users_and_hosts()
{
  const struct utmpx *entry = getutxent();
  while(entry != NULL)
  {
    if(entry->ut_type == 7)
    {
      printf("%s ", entry->ut_user);
      printf("(%s)\n", entry->ut_host);
    }
    entry = getutxent();
  }
}


void show_users_and_groups()
{
  const struct utmpx *entry = getutxent();
  const struct passwd *result = NULL;
  while(entry != NULL)
  {
    if(entry->ut_type == 7)
    {
      printf("%s ", entry->ut_user);
      result = getpwnam(entry->ut_user);
      show_group(entry, result);
    }
    entry = getutxent();
  }
}

void show_users_hosts_groups()
{
  const struct utmpx *entry = getutxent();
  const struct passwd *result = NULL;
  while(entry != NULL)
  {
    if(entry->ut_type == 7)
    {
      printf("%s ", entry->ut_user);
      printf("(%s)", entry->ut_host);
      result = getpwnam(entry->ut_user);
      int ngroups = 1;
      const struct group *gr = NULL;
      gid_t *groups = malloc(sizeof(*groups)*ngroups);
      while(getgrouplist(entry->ut_user, result->pw_gid, groups, &ngroups) == -1)
      {
        free(groups);
        ngroups++;
        groups = malloc(sizeof(*groups)*ngroups);
      }
      printf("[");
      for(int i = 0; i < ngroups -1 ; i++)
      {
        gr = getgrgid(groups[i]);
        printf("%s, ", gr->gr_name);
      }
      gr = getgrgid(groups[ngroups -1]);
      printf("%s]\n", gr->gr_name);
    }
    entry = getutxent();
  }
}

int main(int argc, char *argv[])
{
  int opt[2] = {0, 0};
  int temp = 0;
  int i = 0;
  int sum = 0;
  while((temp = getopt(argc, argv, "hg")) != -1)
  {
    opt[i] = temp;
    i++;
  }
  sum = opt[0] + opt[1];
  switch(sum)
  {
    case 0: show_users(); break;
    case 104: show_users_and_hosts(); break; //h
    case 103: show_users_and_groups(); break; //g
    case 207: show_users_hosts_groups(); break; //hg
  }
  return 0;
}
