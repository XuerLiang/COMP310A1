#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#include "pcb.h"
#include "cpu.h"

typedef struct readyqueue{
        pcb* head;
        pcb* tail;
}readyQueue;

FILE *ram[10];
int ramPointer = 0;
readyQueue rq;

int loadToRam(FILE *fp){
        ram[ramPointer] = fp;
        ramPointer++;
        //printf("ramPointer %d \n",ramPointer);
}

void createPCB(){
        for(int i = 0; i < ramPointer; i++){
                FILE *pcbPointer = ram[i];
                pcb *p = (pcb *)malloc(sizeof(pcb));
                p->PC = pcbPointer;
                p->next = NULL;
                p->ramIndex = i;
                if(rq.head == NULL){
                        rq.head = p;
                }else if(i == 1){
                        rq.head->next = p;
                        rq.tail = p;
                }else{
                        rq.tail->next = p;
                        rq.tail = p;
                }
        }
}


int runProgram(){
        createPCB();
        int errcode = 0;
        while(rq.head != NULL){
                pcb *a = rq.head;
                errcode = runPCB(a);
                if(errcode == -1){
                        int index = a->ramIndex;
                        rq.head = a->next;
                        a->next = NULL;
                        fclose(ram[index]);
                        ram[index] = NULL;
                        ramPointer--;
                        //printf("ramPointer after decrement %d\n",ramPointer);
                        if(ramPointer == 1) return runPCBspecial(rq.head);
                }else{
                        rq.head = a->next;
                        (rq.tail)->next = a;
                        rq.tail = a;
                        a->next = NULL;
                }
        }
	for(int i = 0; i < 3; i++){
		ram[i] = NULL;
	}
	ramPointer = 0;
	rq.head = NULL;
	rq.tail = NULL;
        return 0;
}

