#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "ram.h"


// private fields
char *commands = "COMMAND\t\t\tDESCRIPTION\nhelp\t\t\tDisplays all the commands\nquit\t\t\tExits / terminates the shell with \"Bye!\"\nset VAR STRING\t\tAssigns a value to shell memory\nprint VAR\t\tPrints the STRING assigned to VAR\nrun SCRIPT.TXT\t\tExecutes the file SCRIPT.TXT\n";

// private functions
int help() {
  printf("%s", commands);
  return 0;
}

int quit() {
  // free the memory
  freeMem();
  printf("Bye!\n");
  return -1;
}

int set(char *var, char *string) {
  setMem(var, string);
  return 0;
}

int print(char *var) {
  printf("%s\n", getMem(var));
  return 0;
}

int run(char *filename) {
  int errCode = 0;
  char line[1000];
  FILE *fp = fopen(filename, "rt");
  // if failed to open
  if (!fp) {
    printf("Script not found\n");
    return 1;
  };
  fgets(line, 999, fp);
  while(!feof(fp)) {
    errCode = parse(line);
    if (errCode != 0) break;
    fgets(line, 999, fp);
  }
  fclose(fp);
  return errCode;
}

int unknown() {
  printf("Unknown command\n");
  return 1;
}

int exec(char *words[], int len) {
  int errCode = 0;
  char line[1000];
  
  for(int i = 1; i < len; i++){
	printf("%s \n",words[i]);
	FILE *fp = fopen(words[i], "rt");
	
	if (!fp) { // if failed to open
		printf("Script not found\n");
		return 1;
	}else{
		loadToRam(fp);
	}   
  }
  return runProgram();
  
}

// public functions
int interpreter(char *words[], int len) {
  // assumes: cmd args
  // assumes: words[0] is cmd
  char *cmd = words[0];
  int errCode = 0;
  
  // The user is asking to execute a single command
  if (len == 1 && strcmp(cmd, "help") == 0) errCode = help();
  else if (len == 1 && strcmp(cmd, "quit") == 0) errCode = quit();
  else if (len == 3 && strcmp(cmd, "set") == 0) errCode = set(words[1], words[2]);
  else if (len == 2 && strcmp(cmd, "print") == 0) errCode = print(words[1]);
  else if (len == 2 && strcmp(cmd, "run") == 0) errCode = run(words[1]);
  else if (len == 2 && strcmp(cmd, "exec") == 0) errCode = run(words[1]);
  else if (strcmp(cmd, "exec") == 0) errCode = exec(words, len);
  else errCode = unknown();
  return errCode;
}

