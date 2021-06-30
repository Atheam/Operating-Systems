#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main(int argc, char* argv[]){
    int n;
    if(argc < 2){
        perror("Wrong argument number");
        return -1;
    }

    n = atoi(argv[1]);


    for(int i = 0; i < n;i++){
        pid_t pid = fork();
        if(pid == 0){
            printf("This is process number %d with pid number %d \n",i+1,getpid());
            exit(0);
        }
    }

    for(int i = 0; i < n;i++){
        wait(NULL);
    }



    

    

    return 0;


}