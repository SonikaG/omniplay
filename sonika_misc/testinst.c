#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"
void y(){
  int i = 0;
  i += 5;
}

int main(){
  int  *ignore_flag = (int *)malloc(sizeof(int));
  set_analysis_flag_add((void *)ignore_flag);
  int fd;
  int rc = devspec_init (&fd);
  if (rc < 0) return rc;
//  printf("test!\n");
  set_ign(fd, ignore_flag);
  int a = 5;
  int b = 6;
  *ignore_flag = 1; 
  //getpid();
//  printf("here!\n");
  char buf[200];
  sprintf(buf, "here!\n");
  write(1, buf, strlen(buf) +1);
  *ignore_flag = 0;
  b += a;
  y();
  printf("The number is %d\n", b);
}
