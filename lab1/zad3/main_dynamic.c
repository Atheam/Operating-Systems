#include <stdio.h>
#include "mergelib.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>


#include <dlfcn.h>
  

int (*free_file_list_dl)(filePair* filePairList);
filePair* (*create_file_pair_dl)(char*,char*);
int (*add_file_pair_dl)(filePair*, filePair* );
block** (*create_table_dl)(int);
int (*free_table_dl)(block** ,int );
int (*merge_file_sequence_dl)(filePair*);
block* (*create_block_dl)(filePair*);
int (*add_all_blocks_dl)(filePair* ,block** ,int );
int (*print_files_dl)(block** , int );
int (*delete_block_dl)(block** ,int );
int (*delete_row_dl)(block*  ,int );


void timeit(int pairsNumber,int rowsNumber){

    


    char test_filename1[] = "test_file1.txt";
    char test_filename2[] = "test_file2.txt";

    FILE* test_file1 = fopen(test_filename1,"w+");
    FILE* test_file2 = fopen(test_filename2,"w+");

    const char test_text1[] = "abcdefghijklmoprstuwxyz123456789\n";
    const char test_text2[] = "987654321zyxwutsrpomlkjihgfedcba\n";


    for(int i =0;i< rowsNumber;i++){
        fputs(test_text1,test_file1);
        fputs(test_text2,test_file2);
    }

    fclose(test_file1);
    fclose(test_file2);
    
    filePair* filePairList = create_file_pair_dl(test_filename1,test_filename2);
    for(int i =0;i<pairsNumber-1;i++){
        add_file_pair_dl(filePairList,create_file_pair_dl(test_filename1,test_filename2));
    }

    block** table = create_table_dl(pairsNumber);

    struct tms start_time;
    struct tms end_time;
    double real,user,sys;
    
    times(&start_time);
    clock_t start = clock();
    //merge
    
    merge_file_sequence_dl(filePairList);

    times(&end_time);
    clock_t end = clock();
    real = (double)(end - start) / CLOCKS_PER_SEC;
    sys = (double)(end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK);
    user = (double)(end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK);


    printf("\nMERGING TIME\n");
    printf("REAL: %f s\n",real);
    printf("USER: %f s\n",user);
    printf("SYS: %f s\n",sys);
    

    //saving blocks
    times(&start_time);
    start = clock();


    add_all_blocks_dl(filePairList,table,pairsNumber);
    
    end = clock();  
    times(&end_time);
    real = (double)(end - start) / CLOCKS_PER_SEC;
    sys = (double)(end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK);
    user = (double)(end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK);
    
    

    printf("\nSAVING TIME\n");
    printf("REAL: %f s\n",real);
    printf("USER: %f s\n",user);
    printf("SYS: %f s\n",sys);


    //delete blocks
    times(&start_time);
    start = clock();

    free_table_dl(table,pairsNumber);

    times(&end_time);
    end = clock();
    real = (double)(end - start) / CLOCKS_PER_SEC;
    sys = (double)(end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK);
    user = (double)(end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK);


    printf("\nDELETE TIME\n");
    printf("REAL: %f s\n",real);
    printf("USER: %f s\n",user);
    printf("SYS: %f s\n",sys);

    
    
    

    //add then delete twice
    
    start = clock();
    times(&start_time);

    table = create_table_dl(pairsNumber);
    
    merge_file_sequence_dl(filePairList);
    
    
    add_all_blocks_dl(filePairList,table,pairsNumber);
    
    
    free_table_dl(table,pairsNumber);
    
    table = create_table_dl(pairsNumber);
    merge_file_sequence_dl(filePairList);
    
    add_all_blocks_dl(filePairList,table,pairsNumber);
    
    free_table_dl(table,pairsNumber);
    
     
    times(&end_time);
    end = clock();
    real = (double)(end - start) / CLOCKS_PER_SEC;
    sys = (double)(end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK);
    user = (double)(end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK);

    printf("\nSAVE AND DELETE TIME\n");
    printf("REAL: %f s\n",real);
    printf("USER: %f s\n",user);
    printf("SYS: %f s\n",sys); 
    
   
}



int main(int argc,char**argv){





    
    
    void* handle  = dlopen("./libmerge.so",RTLD_NOW);
    if(!handle){
    puts("No library found");
    return 1;
    }

    free_file_list_dl = dlsym(handle,"free_file_list");
    delete_block_dl = dlsym(handle,"delete_block");
    create_file_pair_dl = dlsym(handle,"create_file_pair");
    add_file_pair_dl = dlsym(handle,"add_file_pair");
    create_table_dl = dlsym(handle,"create_table");
    free_table_dl = dlsym(handle,"free_table");
    merge_file_sequence_dl = dlsym(handle,"merge_file_sequence");
    create_block_dl = dlsym(handle,"create_block");
    add_all_blocks_dl = dlsym(handle,"add_all_blocks");
    print_files_dl = dlsym(handle,"print_files");
    delete_block_dl = dlsym(handle,"delete_block");
    delete_row_dl = dlsym(handle,"delete_row");

     
    if(strcmp(argv[1],"timeit")==0){
        puts("2 BLOCKS");
        puts("------------------------------------");
        puts("");
        puts("10 ROWS");
        timeit(2,10);
        puts("");
        puts("");
        puts("100 ROWS");
        timeit(2,100);
        puts("");
        puts("");
        puts("1000 ROWS");
        timeit(2,1000);
        puts("");
        puts("");

        puts("10 BLOCKS");
        puts("------------------------------------");
        puts("");
        puts("10 ROWS");
        timeit(10,10);
        puts("");
        puts("");
        puts("100 ROWS");
        timeit(10,100);
        puts("");
        puts("");
        puts("1000 ROWS");
        timeit(10,1000);
        puts("");
        puts("");

        puts("100 BLOCKS");
        puts("------------------------------------");
        puts("");
        puts("10 ROWS");
        timeit(100,10);
        puts("");
        puts("");
        puts("100 ROWS");
        timeit(100,100);
        puts("");
        puts("");
        puts("1000 ROWS");
        timeit(100,1000);
        puts("");
        puts("");
        dlclose(handle);
        return 0;
    }

    block** table=NULL;
    filePair* filePairList = NULL;
    int size = 0;


    for(int i =1; i < argc;i++){
        if(strcmp(argv[i],"create_table") == 0){
             
             size = atoi(argv[++i]);
             table = create_table_dl(size);
             

              
        }
        else if(strcmp(argv[i],"merge_files")== 0){
            char* filenameA = strtok(argv[++i],":");
            char* filenameB = strtok(NULL,":");
            filePairList = create_file_pair_dl(filenameA,filenameB);
            for(int j = 0;j<size-1;j++){
                filenameA = strtok(argv[i+1+j],":");
                filenameB = strtok(NULL,":");
                add_file_pair_dl(filePairList,create_file_pair_dl(filenameA,filenameB));
                
            }
            merge_file_sequence_dl(filePairList);
            add_all_blocks_dl(filePairList,table,size);
        }
        else if(strcmp(argv[i],"remove_block")==0){
            int block_idx = atoi(argv[++i]);
            if(block_idx >= size){
                puts("Block index is too high");
                continue;
            }
            delete_block_dl(table,block_idx);
        }
        else if(strcmp(argv[i],"remove_row")==0){
            int row_idx = atoi(argv[++i]);
            block* block = table[row_idx];
            if(row_idx >= block->size){
                puts("Row index is too high");
                i++;
                continue;
            }
            delete_row_dl(block,atoi(argv[++i]));
        }    
    }
    print_files_dl(table,size);

    

    free_file_list_dl(filePairList);
    free_table_dl(table,size);
    
    #ifdef dynamic
    dlclose(handle);
    #endif
    

}
