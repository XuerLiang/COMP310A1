#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "pcb.h"
#include "shell.h"
#include "ram.h"
#include "cpu.h"


// Global data structures representing hardware
//
FILE *ram[10];

int myinit(FILE *p) {
        PCB *pcb;
        int result;

        //result = addToRAM(p);

        pcb = makePCB(p);
        if (pcb != NULL) {
           addToReady(pcb);
           return 1;
        }
        

        return 0;
}

void terminate(PCB *p) {
        //clearRAM(p->start);
        free(p);
}

void scheduler() {
        PCB *pcb;
        int result;

        // initialize CPU
        initCPU();

        // execute the processes
        while(getHead() != NULL) {
                // printPCB(); // for debugging

                pcb = getFromReady();

                if (pcb != NULL) {
                        setCPU(pcb);
			//printf("before runCPU\n");
                        result = runCPU(2, pcb);

                        if (result == 99) terminate(pcb);
                        else addToReady(pcb);
                }
        }
}

void boot(){
	initRAM();
	system("rm -r BackingStore");
	system("mkdir BackingStore");
}

int main() {
        int result = 0;

        boot();
        initCPU();

        result = shell();
        return result;
}

