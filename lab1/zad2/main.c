#include <stdio.h>
#include "mergelib.h"
#include <string.h>



int main(int argc,char**argv){
    block** table=NULL;
    filePair* filePairList;
    int size;

    for(int i =1; i < argc;i++){
        if(strcmp(argv[i],"create_table") == 0){
             size = atoi(argv[++i]);
             table = create_table(size);
        }
        else if(argv[i],"merge_files"){
            for(int j = 0;j<size;j++){
                char* filenameA = strtok(argv[i+1+j],":");
                char* filenameB = strtok(NULL,":");
                if(j ==0) filePairList = create_file_pair(filenameA,filenameB);
                else add_file_pair(filePairList,create_file_pair(filenameA,filenameB));
            }
            merge_file_sequence(filePairList);
            add_all_blocks(filePairList,table,size);
        }
        else if(strcmp(argv[i],"remove_block")){
            delete_block(table,atoi(argv[++i]));
                
        }
        
    }

   
}
