#include <stdio.h>
#include <stdlib.h>
#define SIZE_INPUT 200

int main(){
  int count=0;
  char *counter;
  char *input = malloc(SIZE_INPUT * sizeof(char));
  fgets(input, SIZE_INPUT, stdin);
  for (counter = input; *counter != '\0'; counter++){
    //printf("%s ", counter);
    count++;
  } 
  //printf("You typed: %s", input);
  //printf("AND it was %d characters long!\n", count-1);
  return 0;
}
