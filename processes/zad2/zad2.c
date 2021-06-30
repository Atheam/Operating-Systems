#include <stdio.h>
#include "mergelib.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>



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
    
    struct tms start_time;
    struct tms end_time;
    double real,user,sys;
    times(&start_time);
    clock_t start = clock();
    fflush(stdout);

    for(int i =0;i<pairsNumber-1;i++){
        filePair* filePair= create_file_pair(test_filename1,test_filename2);
        
        if(fork() == 0){
        merge_file_sequence(filePair);
        exit(0);
        }
        free_file_list(filePair);
    }

    for(int i = 0;i<pairsNumber;i++){
        wait(NULL);
    }

    times(&end_time);
    clock_t end = clock();
    real = (double)(end - start) / CLOCKS_PER_SEC;
    sys = (double)(end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK);
    user = (double)(end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK);


    printf("\nMERGING TIME\n");
    printf("REAL: %f s\n",real);
    printf("USER: %f s\n",user);
    printf("SYS: %f s\n",sys);
    
}

void print(filePair* filePair){
    if(filePair->temporaryFile){
        char* line = NULL;
        size_t len = 0;
        while(getline(&line,&len,filePair->temporaryFile) != -1 ){
            fprintf(stdout,"%s",line);
        }
    }
}


int main(int argc, char* argv[]){



     if(argc < 2){
        perror("Wrong argument number");
        return -1;
    }

    if(strcmp(argv[1],"timeit")==0){
        puts("2 PAIRS");
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

        puts("100 PAIRS");
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
        puts("1000 PAIRS");
        puts("------------------------------------");
        puts("");
        puts("10 ROWS");
        timeit(1000,10);
        puts("");
        puts("");
        puts("100 ROWS");
        timeit(1000,100);
        puts("");
        puts("");
        puts("1000 ROWS");
        timeit(1000,1000);
        puts("");
        puts("");
        return 0;
    }


    int n;
    if(argc < 4){
        perror("Wrong argument number");
        return -1;
    }

    n = atoi(argv[1]);
    
    for(int i =0;i<n;i++){
        filePair* filePair = create_file_pair(argv[2],argv[3]);
        if(fork() == 0){
            merge_file_sequence(filePair); 
            print(filePair);
            exit(0);
        }
        free_file_list(filePair);
    }
    for(int i = 0;i<n;i++){
        wait(NULL);
    }
    
    


}