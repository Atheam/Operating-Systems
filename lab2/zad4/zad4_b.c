#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>


void read_replace(int fileA, int fileB, char* n1,char* n2){
    char c;
    while(read(fileA,&c,sizeof(char)) > 0){
        if(c == n1[0]){
            int n = 1;
            char next_c;
            while(n < strlen(n1) && read(fileA,&next_c,sizeof(char)) == 1 && next_c == n1[n]) n++;
            if(n == strlen(n1)) write(fileB,n2,sizeof(char)*strlen(n2));
            else{
                write(fileB,&c,sizeof(char));
                lseek(fileA,-n,SEEK_CUR);
            }
        }
        else {
            write(fileB,&c,sizeof(char));
        }
    }
}



int main(int argc, char * argv[]){

    int timeit = 0;
    struct tms start_time;
    struct tms end_time;
    double real,user,sys;
    clock_t start;

    if(argc < 5){
        perror("Niepoprawna liczba parametrów");
        return -1;
    }
    if(argc >= 6 && strcmp(argv[5],"timeit") == 0){
        timeit = 1;
        times(&start_time);
        start = clock();
    }

    int fileA = open(argv[1],O_RDONLY);
    if(fileA == -1){
        perror("Nie udalo sie otworzyc pliku");
        return -1;
    }
    int fileB = open(argv[2],O_WRONLY);

    if(fileB == -1){
        close(fileA);
        perror("Nie udało sie otworzyc pliku");
        return -1;
    }
    char* n1 = argv[3];
    char* n2 = argv[4];

    read_replace(fileA,fileB,n1,n2);

    close(fileA);
    close(fileB);


    if(timeit == 1){
        clock_t end = clock();
        times(&end_time);
        real = (double)(end-start)/CLOCKS_PER_SEC;
        sys = (double)(end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK);
        user = (double)(end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK);
        puts("");
        puts("SYS TIMES");
        puts("");
        printf("REAL: %f s\n",real);
        printf("USER: %f s\n",user);
        printf("SYS: %f s\n",sys);
    }

    return 0;


}

