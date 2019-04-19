typedef struct PCB_REC {
        FILE *PC;
	struct PCB_REC *next;
	int pageTable[10];
	int PC_page, PC_offset, pages_max;
} PCB;

extern PCB *head;
extern PCB *tail;
void addToReady(PCB *p);
PCB *getFromReady();
PCB *makePCB(FILE *p);
PCB *getHead();
void printPCB();

