#ifndef mergelib
#define mergelib
#include <stdio.h>

typedef struct filePair{
    struct filePair* next;
    char* filenameA;
    char* filenameB;
    FILE* temporaryFile;
}filePair;


typedef struct block{
    char ** lines;
    int size;
}block;


int delete_block(block** table,int index);
filePair* create_file_pair(char*,char*);





#endif