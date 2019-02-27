#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#include "shellmemory.h"

// Model 3: Linked list
typedef struct node {
  char *var;
  char *value;
  struct node *next;
} node_t;

// sentinel node, doesn't hold real value
node_t *sentinel = NULL;

void init() {
  if (sentinel == NULL) {
    // init sentinel
    sentinel = malloc(sizeof(node_t));
    sentinel->next = NULL;
  }
}

// public functions
void setMem(char *var, char *value) {
  init();
  
  node_t *pre = sentinel;
  for (; pre->next != NULL; pre = pre->next) {
    // if exists, overwrites the value
    if (strcmp(pre->next->var, var) == 0) {
      free(pre->next->value);
      pre->next->value = strdup(value);
      return;
    }
  }
  // if not exist
  pre->next = malloc(sizeof(node_t));
  pre->next->var = strdup(var);
  pre->next->value = strdup(value);
  pre->next->next = NULL;
}

char *getMem(char *var) {
  init();
  
  node_t *pre = sentinel;
  for (; pre->next != NULL; pre = pre->next) {
    // if found
    if (strcmp(pre->next->var, var) == 0) {
      return pre->next->value;
    }
  }
  // var not found
  return "Variable does not exist";
}

// free the memory, only called when quit
void freeMem() {
  if (sentinel == NULL) return;
  node_t *current = sentinel->next;
  while (current != NULL) {
    node_t *next = current->next;
    free(current->var);
    free(current->value);
    free(current);
    current = next;
  }
  free(sentinel);
}

