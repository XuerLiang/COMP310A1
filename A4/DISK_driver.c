#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct PARTITION {
	int total_blocks;
	int block_size;
} partition;

struct FAT{
	char *filename;
	int file_length;
	int blockPtrs[10];
	int current_location;
}fat[20];

char *block_buffer;
FILE *fp[5];
char *partition_name;
char partition_path[1000];
int file_in_process[5];
int block_index; // global pointer used when writing new file to the data blocks of the partition


void initIO(){
	partition.total_blocks = 0;
	partition.block_size = 0;
	for(int i = 0; i < 20; i++){
		fat[i].filename = "";
		fat[i].file_length = 0;
		fat[i].current_location = -1;
		for(int j = 0; j < 10; j++){
			fat[i].blockPtrs[j] = -1;
		}
	}
	block_buffer = "";
	for(int i = 0; i < 5; i++){
		fp[i] = NULL;
	}
	partition_name = "";
	for(int i = 0; i < 5; i++){
		file_in_process[i] = -1;
	}
	block_index = 0;
}

int partitionfn(char *name, int blocksize, int totalblocks){
	initIO();
	//malloc block_buffer
	
	//create directory PARTITION
	char command1[20];
	strcpy(command1, "mkdir PARTITION");
	system(command1);
	
	//copy file name
	int name_length = strlen(name);
	char fn[name_length];
	strcpy(fn, name);
	
	//create new file with file_name in PARTITION
	char command2[30+name_length];
	strcpy(command2, "touch PARTITION/");
	strncat(command2, fn, name_length+1);
	system(command2);
	    
    // open file for writing
	char path[20+name_length];
	strcpy(path, "PARTITION/");
	strncat(path, fn, name_length+1);
	FILE *part; 
    part = fopen (path, "w"); 
    if (part == NULL){ 
        fprintf(stderr, "\nError open file\n"); 
        return -1; 
    }
	
	//create the partition header
	fwrite(&partition, sizeof(struct PARTITION), 1, part); 
	for(int i = 0; i < 20; i++) fwrite(&fat[i], sizeof(struct FAT), 1, part);
	//write '0' to the whole data block section
	char str[] = "0";
	for(int i = 0; i < totalblocks*blocksize; i++){
		fwrite(str, 1, 1, part);
	}
	fclose(part);
	
	return 0;
}

int mount(char *name){
	partition_name = name;
	//copy file name
	int name_length = strlen(name);
	char fn[name_length];
	strcpy(fn, name);
	char path[20+name_length];
	strcpy(path, "PARTITION/");
	strncat(path, fn, name_length+1);
	
	strcpy(partition_path, path);
	
	FILE *part;
	// Open file 
    part = fopen (path, "r"); 
    if (part == NULL) 
    { 
        fprintf(stderr, "\nError opening file\n"); 
        return -1; 
    } 
    
	//initialize the partition
	struct PARTITION temp;	
    fread(&temp, sizeof(struct PARTITION), 1, part);
	partition.total_blocks = temp.total_blocks;
	partition.block_size = temp.block_size;

	for(int i = 0; i < 20; i++){
		fread(&fat[i], sizeof(struct FAT), 1, part);
	}
  
    // close file 
    fclose (part);
	
	block_buffer = malloc(sizeof(char)*partition.block_size);
	
	return 0;
}

int openfile(char *name){
	FILE *part;
	// Open partition 
    part = fopen (partition_path, "r+"); 
    if (part == NULL) 
    { 
        fprintf(stderr, "\nError opening file\n"); 
        return -1; 
    }
	int index = 0;
	int blocknum = 0;
	for(; index < 20; index++){
		if(fat[index].filename == name){
			blocknum = fat[index].blockPtrs[0];
			int offset = sizeof(partition)+sizeof(fat)+blocknum*partition.block_size;
			for(int i = 0; i < 5; i++){
				if(file_in_process[i] == index) return index;
			}
			for(int i = 0; i < 5; i++){
				if(fp[i] == NULL) {
					fseek(part, offset, SEEK_SET);
					fp[i] = part;
					fat[index].current_location = 0;
					file_in_process[i] = index;
					return index;
				}
				if(i == 4) return -1;
			}
		}	
	}

	//if file name not found in fat, create new FAT
	for(int i = 0; i < 20; i++){
		if(fat[i].current_location == -1){
			
			//initialize a new FAT
			struct FAT *temp = (struct FAT*)malloc(sizeof(struct FAT));
			temp->filename = name;
			temp->file_length = 0;
			temp->current_location = 0;
			
			fat[i] = *temp;
			return i;
		}
		if(i == 19) return -1;
	}
	fclose(part);
	
	return -1;
}

char* readBlock(int file){
	int index = -1;
	for(int i = 0; i < 5; i++){
		if(file_in_process[i] == file){
			index = i;
			break;
		}
	}
	//int blocknum = fat[file].blockPtrs[current_location];
	//int offset = sizeof(partition)+sizeof(fat)+blocknum*partition.block_size;
	if(!feof(fp[index])) fread(block_buffer, partition.block_size, 1, fp[index]);
	fat[file].current_location++;
	return block_buffer;
}

int writeBlock(int file, char *data){
	if(file == -1) return -1;
	int index = 0;
	for(int i = 0; i < 5; i++){
		if(file_in_process[i] == file){
			index = 1;
			break;
		}
	}
	fwrite(data, partition.block_size, 1, fp[index]);
	fat[file].current_location++;
}

