#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "pcb.h"
#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "memorymanager.h"
#include "DISK_driver.h"

int run(char *filename) {
        FILE *ptr;
        char buffer[1000], buf0[100], buf1[100], buf2[100];
        int result = 0;

        ptr = fopen(filename,"rt");
        if (ptr == NULL) return 4; // file not found

        fgets(buffer,999,ptr);
        while(!feof(ptr)) {
                if (strlen(buffer)>1) result = prompt(buffer);
                if (result == 99) break;
                fgets(buffer,999,ptr);
        }

        fclose(ptr);

        if (result == 99) return 99;

        return 0;
}

int mount_command(char *partitionName, char *number_of_blocks, char *block_size){
	int numblocks = atoi(number_of_blocks);
	int blocksize = atoi(block_size);
	partitionfn(partitionName, blocksize, numblocks);
	mount(partitionName);
	return 0;
}

int write_command(char *filename, char *words){
	int index = openfile(filename);
	if(index == -1) return -1;
	
	int text_length = strlen(words);
	char w[text_length];
	char copy[text_length];
	strcpy(w, words);
	for(int i = 1; i < text_length-1; i++){
		copy[i-1] = w[i];
	}
	char *arg = copy;
	writeBlock(index, arg);
	return 0;
}

int read_command(char *filename, char *variable){
	int index = openfile(filename);
	if(index == -1) return -1;
	variable = readBlock(index);
	return 0;
}

int exec(char *file1, char *file2, char *file3) {
        FILE *p1, *p2, *p3;

        if (strlen(file1)<1) file1 = NULL;
        if (strlen(file2)<1) file2 = NULL;
        if (strlen(file3)<1) file3 = NULL;

        // Determine if there are duplicates and remove
        //if (file1!=NULL && file2!=NULL && strcmp(file1,file2)==0) file2=NULL;
        //if (file1!=NULL && file3!=NULL && strcmp(file1,file3)==0) file3=NULL;
        //if (file2!=NULL && file3!=NULL && strcmp(file2,file3)==0) file3=NULL;

        printf("f1:%s f2:%s f3:%s\n", file1, file2, file3);

        // Add to ready queue each  unique programs
        if (file1!=NULL) {
                p1 = fopen(file1,"rt");
                launcher(p1, file1);
        }
        if (file2!=NULL) {
                p2 = fopen(file2,"rt");
                launcher(p2, file2);
        }
        if (file3!=NULL) {
                p3 = fopen(file3,"rt");
                launcher(p3, file3);
        }

        scheduler();
		
        return 0;
}

int interpreter(char buf0[], char buf1[], char buf2[], char buf3[]) {
        int result = 0; // no errors

        if (strcmp(buf0,"help")==0) {
                printf("Legal commands:\n");
                printf("help              display this help\n");
                printf("quit              exits the shell\n");
                printf("set VAR STRING    assign STRING to VAR\n");
                printf("print VAR         display contents of VAR\n");
                printf("run SCRIPT.TXT    interpret SCRIPT.TXT\n");
                printf("exec P1 P2 P2     can run up to 3 distinct programs\n");

                result = 0;
        }
        else if (strcmp(buf0,"quit")==0) {
                result = 99; // exit shell code
        }
        else if (strcmp(buf0,"set")==0) {
                if (strlen(buf1)<1 || strlen(buf2)<1) return 1; // set error

                add(strdup(buf1), strdup(buf2));
        }
        else if (strcmp(buf0,"print")==0) {
                if (strlen(buf1)<1) return 2; // print error

                printf("%s\n", get(buf1));
        }
        else if (strcmp(buf0,"run")==0) {
                if (strlen(buf1)<1) return 3; // run error

                result = run(buf1);
        }
        else if (strcmp(buf0,"exec")==0) {
                if (strlen(buf1)<1) return 5; // exec error

                result = exec(buf1, buf2, buf3);
        }
		else if (strcmp(buf0,"mount")==0) {
                if (strlen(buf1)<1) return 6; // exec error

                result = mount_command(buf1, buf2, buf3);
        }
		else if (strcmp(buf0,"write")==0) {
                if (strlen(buf1)<1) return 7; // exec error

                result = write_command(buf1, buf2);
        }
		else if (strcmp(buf0,"read")==0) {
                if (strlen(buf1)<1) return 8; // exec error

                result = read_command(buf1, buf2);
        }
        else {
                result = 98; // command does not exist
        }

        return result;
}


