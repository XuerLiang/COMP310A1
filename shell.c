#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#include "shellmemory.h"
#include "interpreter.h"


// not a new line char
int notNewline(char c) {
  return c != '\n' && c != '\r';
}

int parse(char ui[]) {
  char tmp[1000];
  char *words[1000];
  int a,b;
  int len = 0; // words array length
  
  for(a=0; ui[a]==' ' && a<1000; a++); // skip white spaces
  
  while(ui[a] != '\0' && notNewline(ui[a]) && a<1000) {
    for(b=0; ui[a]!='\0' && ui[a]!=' ' && notNewline(ui[a]) && a<1000; a++, b++)
      tmp[b] = ui[a]; // extract a word
    if (b > 0) {
      tmp[b] = '\0';
      words[len++] = strdup(tmp);
    }
    a++;
  }
  
  return interpreter(words, len); // assumes: cmd args
}


