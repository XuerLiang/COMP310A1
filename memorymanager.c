#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <fcntl.h>

#include "pcb.h"
#include "kernel.h"

char path[50];

int countTotalPages(FILE *p){
	//FILE *counter = fdopen(dup(fileno(p)), "r");
	FILE *counter = fopen(path, "rt");
	char head[1000];
	int count = 0;

	//fgets(head, 999, p);
	//printf("before count page first line %s\n", head);

	if(counter == NULL) return 0;
	
	while(1){
        	fgets(head, 999, counter);
            	count++;
		if(feof(counter)) break;
        }

	count--;

	int countPage = count/4;
	
	//printf("count lines %d\n", count);
	//printf("count page %d\n", countPage);
	
	fclose(counter);

	//fgets(head, 999, p);
	//printf("after count page first line %s\n", head);

	if(count%4 != 0){
		return countPage+1;
	}else{
		return countPage;

	}
	
}

FILE *findPage(int pageNumber, FILE *f){
	if(f == NULL) return NULL;
	FILE *fp = fopen(path, "rt");
	int num_of_lines = 4 * pageNumber;
	char line[1000];
	//fgets(line, 999, f);
	//printf("first line is %s\n", line);
	int i = 0;
	//printf("num_of_lines %d\n", num_of_lines);
	for(; i < num_of_lines; i++){
		if(feof(fp)) break;
		fgets(line, 999, fp);
		//printf("line is %s\n", line);
	}
	if(i < num_of_lines) return NULL;
	return fp;
}

int findFrame(FILE *page){
	int index = 0;
	while(index < 10 && ram[index] != NULL){
		index++;
	}
	if(index == 10) return -1;
	return index;
}

int findVictim(PCB *p){
	int random = rand()%10;
	while(1){
		int i = 0;
		for(; i < 10; i++){
			if(p->pageTable[i] == random) break;
		}
		if(i != 10) {
			random = (random+1)%10;
		}else{
			return random;
		}
	}
	return -1;
}

int updateFrame(int frameNumber, int victimFrame, FILE *page){
	if(frameNumber != -1){
		ram[frameNumber] = page;
	}else{
		ram[victimFrame] = page;
	}
	return 0;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame){
	if(frameNumber != -1) {
		p->pageTable[pageNumber] = frameNumber;
		//p->PC_page = pageNumber+1;
		//p->PC_offset = 0;
	}else{
		p->pageTable[pageNumber] = victimFrame;
		PCB *rqpointer = p->next;
		if(rqpointer == NULL) rqpointer = head;
		while(rqpointer != p){
			int i = 0;
			for(; i < 10; i++){
				if(rqpointer->pageTable[i] == victimFrame){
					rqpointer->pageTable[i] = -1;
					break;
				}
			}
			if(i == 10) {
				rqpointer = rqpointer->next;
			}else{
				break;
			}
		}
	}
	return 0;
}

int pageFault(PCB *pcb){
	pcb->PC_page++;
	pcb->PC_offset = 0;
	
	if(pcb->PC_page >= pcb->pages_max){
		return 99;
	} 
	
	int framenum = pcb->pageTable[pcb->PC_page];
	if(framenum != -1){
		pcb->PC = ram[framenum];
	}else{
		FILE *page = findPage(pcb->PC_page, pcb->PC);
		pcb->PC = page;
		int newframe = findFrame(page);
		int victimframe;
		if(newframe == -1) victimframe = findVictim(pcb);
		updateFrame(newframe, victimframe, page);
		updatePageTable(pcb, pcb->PC_page, newframe, victimframe);
	}
	
	return 0;
}


int launcher(FILE *p, char *file_name){
	//move target file to BackingStore
	int file_name_length = strlen(file_name);
	int command_length = file_name_length + 20;
	char command[command_length];
	strcpy(command, "cp ");
	char fn[file_name_length];
	strcpy(fn, file_name);
	strncat(command, fn, file_name_length+1);
	char directory[20];
	strcpy(directory, " BackingStore");
	strncat(command, directory, 14);
	system(command); //command = "cp file_name BackingStore"
	
	//fclose(p);
	
	//open the file in backing store
	//char path[50];
	strcpy(path, "./BackingStore/");
	strncat(path, fn, file_name_length+1);
	FILE *backingStorePointer = fopen(path, "rt");
	if(feof(backingStorePointer)) printf("right after open feof\n");
	myinit(backingStorePointer);
	if(feof(backingStorePointer)) printf("right after myinit feof\n");
	//char test[1000];
	//fgets(test, 999, backingStorePointer);
	//printf("after myinit %s\n", test);
	//load two pages of program to RAM
	int pn = 0;
	while(pn < 2){
		FILE *page = findPage(pn, backingStorePointer);
		if(page == NULL) break;
		if(pn == 0) tail->PC = page;
		//tail->PC_page = pn+1;
		int frame = findFrame(page);
		updateFrame(frame, -1, page);
		updatePageTable(tail, pn, frame, -1);
		//printf("page number is %d\n", pn);
		pn++;
	}
	return 0;

}






