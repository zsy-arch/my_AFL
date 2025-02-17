#ifndef _MY_LOG_H_
#define _MY_LOG_H_
#define BUFF_MAX (1024)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

char my_log_filename[] = "/tmp/tmpfile-XXXXXX";
int fd = -1;

const char *log_level_str[] = {
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"};

const char *get_log_level_str(int level)
{
  return log_level_str[level];
}

int my_log(int level, const char *pformat, va_list _va_list)
{
  time_t t;
  struct tm *tmp;
  unsigned int len = 0;
  char szbuf[BUFF_MAX] = {0};

  if (pformat == NULL)
  {
    return -1;
  }

  t = time(NULL);
  tmp = localtime(&t);

  memset(szbuf, 0, 1024);

  if (-1 != fd)
  {
    len = sprintf(szbuf, "[%04d-%02d-%02d %02d:%02d:%02d][%s] ",
                  tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec, get_log_level_str(level));
    len += vsprintf(szbuf + len, pformat, _va_list);
    szbuf[len++] = '\n';
    lseek(fd, 0L, SEEK_END);
    write(fd, szbuf, len);

    // fseek(pfile, 0L, SEEK_END);
    // fwrite(szbuf, len, 1, pfile);
    // fwrite("\n", 1, 1, pfile);
  }
  return 0;
}

int log_print(int log_level, const char *format, ...)
{
  if (fd == -1)
  {
    fd = mkstemp(my_log_filename);
  }
  va_list list;
  va_start(list, format);
  my_log(log_level, format, list);
  va_end(list);
  return 0;
}

#endif