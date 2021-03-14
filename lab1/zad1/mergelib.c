#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mergelib.h"



filePair* create_file_pair(char * filenameA,char* filenameB){
    struct filePair* filePair = calloc(1,sizeof(filePair));
    filePair->next = NULL;
    filePair->filenameA = calloc(1,sizeof(strlen(filenameA)));
    filePair->filenameB = calloc(1,sizeof(strlen(filenameB)));
    strcpy(filePair->filenameA,filenameA);
    strcpy(filePair->filenameB,filenameB);
    return filePair;
}

int add_file_pair(filePair* filePairList, filePair* filePair){
    if(filePairList == NULL) return 0;
    struct filePair* current = filePairList;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = filePair;
    return 1;
}


block** create_table(int size){
    struct block** table = (block**) calloc(size, sizeof(block*));    
    return table;
}

int free_table(block** table,int size){
    if(table == NULL) return 0;
    for(int i =0;i < size;i++){
        delete_block(table,i);
    }
    free(table);
    table = NULL;
    return 1;
}

int merge_file_sequence(filePair* fileSequence){
    
    filePair* currentPair = fileSequence;
    while(currentPair!=NULL){

    FILE *fileA = fopen(currentPair->filenameA,"r");
    FILE *fileB = fopen(currentPair->filenameB,"r");
    if(fileA == NULL || fileB == NULL){
        puts("Error while opening files");
        return 0;
    }

    FILE *temporary = tmpfile();
    
    char *line = NULL;
    size_t len = 0;
    while(1){
        if(getline(&line,&len,fileA) != -1) fputs(line,temporary);
        else break;
        if(getline(&line,&len,fileB) != -1) fputs(line,temporary);
        else break;
    }

    while(getline(&line,&len,fileA) != -1) fputs(line,temporary);
    while(getline(&line,&len,fileB) != -1) fputs(line,temporary);
    
    currentPair->temporaryFile = temporary;

    rewind(currentPair->temporaryFile);
    fclose(fileA);
    fclose(fileB);

    currentPair = currentPair->next;
    }
    return 1;
    
}


block* create_block(filePair* filePair){
    struct block* block = calloc(1,sizeof(block));
    
    char c;
    int line_count = 0;
    while((c = fgetc(filePair->temporaryFile))!= EOF){
        if(c == '\n') line_count++;
    }
    rewind(filePair->temporaryFile);
    
    block->size = line_count;
    block->lines = (char**) calloc(line_count,sizeof(char*));
    
    int i = 0;
    size_t len = 0;
    while(getline(&(block->lines[i]),&len,filePair->temporaryFile) != -1) i++;

    return block;
}


int add_all_blocks(filePair* filePairList,block** table,int size){
   if(filePairList == NULL) return 0;
   if(table == NULL) return 0;
   filePair* currentPair = filePairList; 
   for(int i = 0;i < size;i++){
       table[i] = create_block(currentPair);
       currentPair = currentPair->next; 
   }
   return 1;
}

int row_count(block** table,int index){
    return table[index]->size;
}

int delete_row(block* block,int line_index){
    if(block == NULL) return 0;
    free(block->lines[line_index]);
    block->lines[line_index] = NULL;
    return 1;
}

int print_files(block** table, int size){
    if(table == NULL) return 0;
    for(int i =0; i < size;i++){
        if(table[i] == NULL){
            printf("BLOK NR %d ZOSTAL USUNIETY\n",i);
            continue;
        }
        for(int j = 0;j<table[i]->size;j++){
            if(table[i]->lines[j] == NULL){
                printf("WIERSZ NR %d ZOSTAL USUNIETY\n",j);
                continue;
            }
            printf("%s",table[i]->lines[j]);

        }
    }
    return 1;
}

int delete_block(block** table,int index){
    if(table == NULL) return 0;
    for(int i = 0;i < table[index]->size;i++){
        delete_row(table[index],i);
    }
    free(table[index]);
    table[index] = NULL;
    return 1;
}

