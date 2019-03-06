#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE_INPUT 200
int main(){

  char *input = malloc(SIZE_INPUT * sizeof(char));
  fgets( input, SIZE_INPUT, stdin);
  printf("You typed: %s", input);
  free(input);
  return 0;

}
