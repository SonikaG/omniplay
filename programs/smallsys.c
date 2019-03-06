#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE_INPUT 200
int main(){

  char *input = malloc(SIZE_INPUT * sizeof(char));
  fgets( input, SIZE_INPUT, stdin);
  printf("You typed: %s", input);
  free(input);
  //int *val = malloc(sizeof(int));
  //*val = 5;
  //int a = 6;
  //pthread_log__sync_add_and_fetch(val, a); 
  //int i = 
  increment_ignore_flag();
  //printf("here is the flag %d\n", i);
  //free(val);
  //a++;
  getpid();
  //i = 
  decrement_ignore_flag();
  //printf("here is the flag %d\n", i);
  //printf("here what is going on?");
  return 0;

}
