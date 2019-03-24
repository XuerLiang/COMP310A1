#include <stdio.h>
#include "pcb.h"
#include "shell.h"
#include "memorymanager.h"

// Global data structures simulating hardware
//

struct CPU {
        FILE *IP;
        char IR[1000];
        int quanta;
	int offset;
} cpu;

void initCPU() {
        cpu.quanta = 2;
        cpu.IP     = NULL;
	cpu.offset = 0;
        //cpu.IR[0]  = '\0';
}

void setCPU(PCB *pcb) {
        cpu.IP    = pcb->PC;
	cpu.offset = pcb->PC_offset;
        //cpu.IR[0] = '\0';
}

int runCPU(int quanta, PCB *pcb) {
        int result;
        char *p;

        cpu.quanta = quanta;

        while(cpu.quanta > 0 && cpu.offset < 4) {
                p = fgets(cpu.IR, 999, pcb->PC);
		//if(p == NULL) printf("p is NULL\n");
                if (p == NULL || feof(pcb->PC)) return 99; // end of program

                //printf(">>>%s\n",cpu.IR); // debug code

                result = prompt(cpu.IR);
                if (result != 0) return result;
				
		cpu.offset++;
                cpu.quanta--;
        }
		
		if(cpu.offset == 4) {
			return pageFault(pcb);
		}else{
			pcb->PC_offset = cpu.offset;
		}
		
        return 0; // no errors
}

