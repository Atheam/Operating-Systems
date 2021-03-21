#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>


int checkDecimal(int n);
int checkSquare(int n);
int read_line(FILE*f, char *buf);


int main(int argc,char* argv[]){
    char* dataFilename = "dane.txt";
    const char* filenameA = "a.txt";
    const char* filenameB = "b.txt";
    const char* filenameC = "c.txt";

    int timeit = 0;
    struct tms start_time;
    struct tms end_time;
    double real,user,sys;
    clock_t start;

    if(argc >= 2 && strcmp(argv[1],"timeit") == 0){
        timeit = 1;
        times(&start_time);
        start = clock();
        dataFilename = "testDane.txt";
    }

    FILE* dataFile = fopen(dataFilename,"r");


    

    if(!dataFile){
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }

    FILE* fileA = fopen(filenameA,"w");
  

    if(!fileA){
        fclose(dataFile);
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }
    FILE* fileB = fopen(filenameB,"w");

    if(!fileB){
        fclose(fileA);
        fclose(dataFile);
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }
    FILE* fileC = fopen(filenameC,"w");

    if(!fileC){
        fclose(fileB);
        fclose(fileA);
        fclose(dataFile);
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }

    unsigned int evenCount =0;
    char buf[128];
    int n;
    int num;
    while((n =read_line(dataFile,buf)) > 0){
        if(n == 1) continue;
        num = atoi(buf);
        if(num%2 ==0) evenCount++;
        if(checkDecimal(num) == 1) fwrite(buf,sizeof(char),n,fileB);
        if(checkSquare(num) == 1) fwrite(buf,sizeof(char),n,fileC);
    }


    char countBuf[128];
    char *msg = "Liczb parzystych jest ";
    fwrite(msg,sizeof(char),strlen(msg),fileA);
    sprintf(countBuf,"%d",evenCount);
    fwrite(countBuf,sizeof(char),strlen(countBuf),fileA);



    fclose(fileB);
    fclose(fileC);
    fclose(fileA);
    fclose(dataFile);

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

}


int checkDecimal(int n){
    int decNum = (n % 100)/10;
    if(decNum == 7 || decNum == 0) return 1;
    else return 0;
}

int checkSquare(int n){
    for(int i = 0; i <= sqrt(n);i++){
        if(i*i == n) return 1;
    }
    return 0;
}

int read_line(FILE* file,char* buf){
    int n = 0;
    while(fread(&buf[n],sizeof(char),1,file) == 1 ){
        n++;
        if(buf[n-1] == '\n') break;
    }
    return n;
        
}
