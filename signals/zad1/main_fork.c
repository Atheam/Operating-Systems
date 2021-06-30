#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define SIGNAL SIGUSR1


void handler(int signum){
    printf("Received a signal with number: %d\n",signum);
}

int main(int argc, char* argv[]){
    
    if(argc < 2 ){
        perror("Additional argument not given");
        return 1;
    }
    if(strcmp(argv[1], "ignore") == 0){
        
        signal(SIGNAL,SIG_IGN);
        
        if(fork() != 0){
            printf("Signal handling in parent process: \n");
            raise(SIGNAL);
            printf("Signal ignored in a parent\n");
            wait(NULL);
        }
        else{
            printf("Signal handling in child process: \n");
            raise(SIGNAL);
            printf("Signal ignored in a child\n");
        }
    }
    else if(strcmp(argv[1],"handler") == 0){
        signal(SIGNAL,handler);
    
        if(fork() !=0){
            printf("Signal handling in parent process: \n");
            raise(SIGNAL);
            wait(NULL);
        }
        else{
            printf("Signal handling in child process: \n");
            raise(SIGNAL);
        }
    }
    else if(strcmp(argv[1],"mask") == 0){
        sigset_t onesig;
        sigemptyset(&onesig);
        sigaddset(&onesig,SIGNAL);
        sigprocmask(SIG_SETMASK,&onesig,NULL);
        if(fork() != 0 ){
            printf("Signal handling in parent process: \n");
            raise(SIGNAL);
            sigset_t mask;
            sigpending(&mask);
            if(sigismember(&mask,SIGNAL)) printf("Signal blocked in parent\n");
            wait(NULL);
        }
        else{
            printf("Signal handling in child process: \n");
            raise(SIGNAL);
            sigset_t mask;
            sigpending(&mask);
            if(sigismember(&mask,SIGNAL)) printf("Signal blocked in child\n");
            else printf("Signal not blocked in child");
        }

    }
    else if(strcmp(argv[1],"pending") == 0){
        sigset_t onesig;
        sigset_t mask;
        sigemptyset(&onesig);
        sigemptyset(&mask);
        sigaddset(&onesig,SIGNAL);
        sigprocmask(SIG_SETMASK,&onesig,NULL);
        raise(SIGNAL);
        
        if(fork() != 0){
            sigset_t w_mask;
            printf("Signal handling in parent process: \n");
            sigpending(&w_mask);
            if(sigismember(&w_mask,SIGNAL)) printf("Signal pending in parent process\n");
            else printf("Signal not pending in parent process\n");
            wait(NULL);
        }
        else{
            sigset_t w_mask;
            printf("Signal handling in child process: \n");
            sigpending(&w_mask);
            if(sigismember(&w_mask,SIGNAL)) printf("Signal pending in child process\n");
            else printf("Signal not pending in child process\n");
        }
    }
    return 0;





}