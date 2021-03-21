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



int main(int argc, char* argv[]){

    int timeit = 0;
    struct tms start_time;
    struct tms end_time;
    double real,user,sys;
    clock_t start;


    if(argc < 3){
        perror("Niepoprawna ilosc parametrow");
        return -1;
    }

    if(argc >= 4 && strcmp(argv[3],"timeit") == 0){
        timeit = 1;
        times(&start_time);
        start = clock();
    }


    int fileA = open(argv[1],O_RDONLY);
    if(fileA == -1){
        perror("Blad przy otwieraniu pliku");
        return -1;
    }
    int fileB = open(argv[2],O_WRONLY|O_CREAT);
    if(fileB == -1){
        close(fileA);
        perror("Blad przy otwieraniu pliku");
        return -1;
    }

    size_t n = 0;
    char c;
    while(read(fileA,&c,sizeof(char)) == 1){
        write(fileB,&c,sizeof(char));
        n++;
        if(c != '\n' && n == 50){
            n = 0;
            c = '\n';
            write(fileB,&c,sizeof(char));
        }
        if(c == '\n') n = 0;
    }


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