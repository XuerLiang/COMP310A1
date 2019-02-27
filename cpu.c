#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#include "shell.h"
#include "pcb.h"

typedef struct cipiu{
	FILE *IP;
	char IR[1000];
}cpu;

cpu CPU;

int runPCB(pcb* p) {
	CPU.IP = p->PC;
        if(feof(CPU.IP)) return -1;
        //run line 1
	int errCode = 0;
        fgets(CPU.IR, 999, CPU.IP);
	//printf("exec line1 %s\n",CPU.IR);
        errCode = parse(CPU.IR);
	//run line 2
        if(feof(CPU.IP)) return -1;
        fgets(CPU.IR, 999, CPU.IP);
	//printf("exec line1 %s\n",CPU.IR);
        errCode = parse(CPU.IR);
		
        if(feof(CPU.IP)) return -1;
        return errCode;
}

int runPCBspecial(pcb* p){
	CPU.IP = p->PC;
        int errCode = 0;
        while(!feof(CPU.IP)) {
                fgets(CPU.IR, 999, CPU.IP);
		//printf("exec special %s\n",CPU.IR);
                errCode = parse(CPU.IR);
                if (errCode != 0) break;
        }
        return errCode;
}
