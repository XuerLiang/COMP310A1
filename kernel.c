#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#include "shellmemory.h"
#include "interpreter.h"
#include "shell.h"

char prompt[100] = {'$', '\0'};  // prompts can be modified
char userInput[1000]; // user's input stored here


int main(int argc, char *argv[]) {
  
  printf("Kernel 1.0 loaded!");
  printf("Welcome to the %s shell!\n", "Xuer Liang");
  printf("Shell Version 2.0 Updated February 2019\n");
  
  printf("%s ", prompt);
  while(fgets(userInput, 999, stdin)) {
    // limit input to array size
    
    int errorCode = parse(userInput);
    if (errorCode == -1) exit(0); // ignore all other errors
    printf("%s ", prompt);
  }
}


