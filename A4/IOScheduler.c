#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "pcb.h"

struct NODE {
	char *data;
	PCB *ptr;
	int cmd;
};

struct NODE* queue[10];
int queue_head = 0;
int queue_tail = 0;

char* IOscheduler(char *data, PCB *ptr, int cmd){
	struct NODE *nd = (struct NODE*)malloc(sizeof(struct NODE));
	nd->data = data;
	nd->ptr = ptr;
	nd->cmd = cmd;
	queue[queue_tail] = nd;
	queue_tail = (queue_tail+1)%10;
}
