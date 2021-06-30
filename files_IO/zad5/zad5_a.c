#include <stdio.h>
#include <time.h>
#include <string.h>
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


    FILE* fileA = fopen(argv[1],"r");
    if(!fileA){
        perror("Blad przy otwieraniu pliku");
        return -1;
    }
    FILE* fileB = fopen(argv[2],"w");
    if(!fileB){
        fclose(fileA);
        perror("Blad przy otwieraniu pliku");
        return -1;
    }

    size_t n = 0;
    char c;
    while(fread(&c,sizeof(char),1,fileA) == 1){
        fwrite(&c,sizeof(char),1,fileB);
        n++;
        if(c != '\n' && n == 50){
            n = 0;
            c = '\n';
            fwrite(&c,sizeof(char),1,fileB);
        }
        if(c == '\n') n = 0;
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