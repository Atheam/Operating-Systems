#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>


void read_replace(FILE* fileA, FILE* fileB, char* n1,char* n2){
    char c;
    while(fread(&c,sizeof(char),1,fileA) > 0){
        if(c == n1[0]){
            int n = 1;
            char next_c;
            while(n < strlen(n1) && fread(&next_c,sizeof(char),1,fileA) == 1 && next_c == n1[n]) n++;
            if(n == strlen(n1)) fwrite(n2,sizeof(char),strlen(n2),fileB);
            else{
                fwrite(&c,sizeof(char),1,fileB);
                fseek(fileA,-n ,SEEK_CUR);
            }
        }
        else {
            fwrite(&c,sizeof(char),1,fileB);
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
        return 1;
    }

    if(argc >= 6 && strcmp(argv[5],"timeit") == 0){
        timeit = 1;
        times(&start_time);
        start = clock();
    }

    FILE* fileA = fopen(argv[1],"r");
    if(!fileA){
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }
    FILE* fileB = fopen(argv[2],"w");

    if(!fileB){
        fclose(fileA);
        perror("Nie udało sie otworzyc pliku");
        return 1;
    }
    char* n1 = argv[3];
    char* n2 = argv[4];

    read_replace(fileA,fileB,n1,n2);

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

