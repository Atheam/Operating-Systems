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


int free_file_list(filePair* filePairList);
int delete_block(block** table,int index);
filePair* create_file_pair(char*,char*);
int add_file_pair(filePair* filePairList, filePair* filePair);
block** create_table(int size);
int free_table(block** table,int size);
int merge_file_sequence(filePair* fileSequence);
block* create_block(filePair* filePair,block** table,int pos);
int add_all_blocks(filePair* filePairList,block** table,int size);
int row_count(block** table,int index);
int print_files(block** table, int size);
int delete_block(block** table,int index);
int delete_row(block* block,int line_index);





#endif