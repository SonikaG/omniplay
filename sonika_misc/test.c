#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
void y(){
  int i = 0;
  i += 5;
}

int main(){
  int  *ignore_flag = (int *)malloc(sizeof(int));
  int fd;
  int rc = devspec_init (&fd);
  if (rc < 0) return rc;
//  printf("test!\n");
  set_ign(fd, ignore_flag);
  int a = 5;
  int b = 6;
  b += a;
  y();
  printf("The number is %d\n", b);
}
