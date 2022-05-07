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




void show_group(const struct utmpx *entry, const struct passwd *result)
{
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
