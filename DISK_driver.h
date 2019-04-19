void initIO();
int partitionfn(char *name, int blocksize, int totalblocks);
int mount(char *name);
int openfile(char *name);
char* readBlock(int file);
int writeBlock(int file, char *data);

