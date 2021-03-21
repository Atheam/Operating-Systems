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

int read_line(int file, char* buf);
int consists(char c,char* in);


int main(int argc, char* argv[]){



    int timeit = 0;
    struct tms start_time;
    struct tms end_time;
    double real,user,sys;
    clock_t start;

    char c;

    if(argc < 3){
        perror("Niepoprawna liczba argumentow");
        return 1;
    }

    if(argc >= 4 && strcmp(argv[3],"timeit") == 0){
        timeit = 1;
        times(&start_time);
        start = clock();
    }

    c = argv[1][0];

    int file = open(argv[2],O_RDONLY);
    if(file == -1){
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }

    char buf[256];
    int n;
    while((n = read_line(file,buf)) > 0){
        if(consists(c,buf) == 1) write(STDOUT_FILENO,buf,sizeof(char)*n);
    }


    close(file);

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



}

int consists(char c,char* in){
    for(int i = 0; in[i] != '\0';i++){
        if(in[i] == c) return 1;
        }
    return 0;
}

int read_line(int file,char* buf){
    int n = 0;
    while(read(file,&buf[n],sizeof(char)) == 1 ){
        n++;
        if(buf[n-1] == '\n') break;
    }
    return n;
        
}
