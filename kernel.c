#include<stdlib.h>
#include<stdio.h>

#include "shell.h"
#include "ram.h"
#include "cpu.h"
#include "pcb.h"

int myinit(FILE *p) {
	PCB *pcb;
	int result;

	result = addToRAM(p);

	if (result>=0) {
		pcb = makePCB(p,result);
		if (pcb != NULL) {
			addToReady(pcb);
			return 1;
		}
	}

	return 0;
}

void terminate(PCB *p) {
	clearRAM(p->start);
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
			setCPU(pcb->PC);
			result = runCPU(2);

			if (result == 99) terminate(pcb);
			else addToReady(pcb);
		} 	
	}
}

int main() {
	int result = 0;
	
	initRAM();
	initCPU();

	result = shell();

	return result;
}
