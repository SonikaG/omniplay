#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int sonika_print (const char *format, ...){
  char buf[1000];
  int ret_sp = sprintf(buf, format);
  if(ret_sp < 0){
      return ret_sp;
  }
  int done = write(1, buf, strlen(buf) +1);
  return done;
}
