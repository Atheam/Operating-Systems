#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>


int checkDecimal(int n);
int checkSquare(int n);
int read_line(int f, char *buf);


int main(int argc, char* argv[]){
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

    int dataFile = open(dataFilename,O_RDONLY);

    if(dataFile == -1){
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }

    int fileA = open(filenameA,O_WRONLY|O_CREAT);
  
    if(fileA == -1){
        close(dataFile);
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }
    int fileB = open(filenameB,O_WRONLY|O_CREAT);

    if(fileB == -1){
        close(fileA);
        close(dataFile);
        perror("Nie udalo sie otworzyc pliku");
        return 1;
    }
    int fileC = open(filenameC,O_WRONLY|O_CREAT);

    if(fileC == -1){
        close(fileB);
        close(fileA);
        close(dataFile);
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
        if(checkDecimal(num) == 1) write(fileB,buf,sizeof(char) * n);
        if(checkSquare(num) == 1) write(fileC,buf,sizeof(char) * n);
    }

    char countBuf[128];
    char *msg = "Liczb parzystych jest ";
    write(fileA,msg,strlen(msg)*sizeof(char));
    sprintf(countBuf,"%d",evenCount);
    write(fileA,countBuf,strlen(countBuf)*sizeof(char));


    close(fileB);
    close(fileC);
    close(fileA);
    close(dataFile);

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

int read_line(int file,char* buf){
    int n = 0;
    while(read(file,&buf[n],sizeof(char)) == 1 ){
        n++;
        if(buf[n-1] == '\n') break;
    }
    return n;
        
}
