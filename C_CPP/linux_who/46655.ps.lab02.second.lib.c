//PS IS1 324 LAB02
//Igor Stodolny
//si46655@zut.edu.pl
#include <utmpx.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "show.h"
void who_v2()
{
  setutxent();
  const struct utmpx *entry = getutxent();
  const struct passwd *result = NULL;
  while(entry != NULL)
  {
    if(entry->ut_type == 7)
    {
      printf("%s\n", entry->ut_user);
      result = getpwnam(entry->ut_user);
      printf("%d\n\n", result->pw_uid);
    }
    entry = getutxent();
  }
}
