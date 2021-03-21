#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>

#define MAXFILENAME 100

size_t read_line(FILE* file){
    size_t n = 0;
    char c;
    while(fread(&c,sizeof(char),1,file) == 1){
        fwrite(&c,sizeof(char),1,stdout);
        n++;
        if(c == '\n') break;
    }
    return n;
}

int main(int argc, char* argv[]){


    int timeit = 0;
    struct tms start_time;
    struct tms end_time;
    double real,user,sys;
    clock_t start;

    FILE* fileA;
    FILE* fileB;

    char filenameA[MAXFILENAME];
    char filenameB[MAXFILENAME];

    if(argc == 1){
        puts("Podaj dwa pliki do wczytania:");
        scanf("%s",filenameA);
        scanf("%s",filenameB);

    }
    else if(argc == 2){
        strcpy(filenameA,argv[1]);
        puts("Podaj jeszcze jeden plik do wczytania:");
        scanf("%s",filenameB);

    }
    else{
        strcpy(filenameA,argv[1]);
        strcpy(filenameB,argv[2]);
    }

    if(argc >= 4 && strcmp(argv[3],"timeit") == 0){
        timeit = 1;
        times(&start_time);
        start = clock();
    }
    fileA = fopen(filenameA,"r");
    
    if(!fileA){
        perror("Nie udalo sie otworzyc pliku nr 1");
        return 1;
    }
    
    fileB = fopen(filenameB,"r");
    if(!fileB){
        fclose(fileA);
        perror("Nie udalosie otworzyc pliku nr 2");
        return 1;
    }


    size_t line_len_a = -1;
    size_t line_len_b = -1;

    while(line_len_a != 0 || line_len_b != 0){
        line_len_a = read_line(fileA);
        line_len_b =read_line(fileB);
    }

    fclose(fileA);
    fclose(fileB);
    if(timeit == 1){
        clock_t end = clock();
        times(&end_time);
        real = (double)(end-start)/CLOCKS_PER_SEC;
        sys = (double)(end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK);
        user = (double)(end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK);
        puts("");
        puts("LIB TIMES");
        puts("");
        printf("REAL: %f s\n",real);
        printf("USER: %f s\n",user);
        printf("SYS: %f s\n",sys);
    }

    return 0;

}