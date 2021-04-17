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
        printf("Signal handling in parent process:\n");
        raise(SIGNAL);
        
        printf("Signal ignored in parent process\n");
        execl("./exec","./exec","ignore",NULL);
    }
    else if(strcmp(argv[1],"mask") == 0){
        sigset_t onesig;
        sigset_t mask;
        sigemptyset(&onesig);
        sigemptyset(&mask);
        sigaddset(&onesig,SIGNAL);
        sigprocmask(SIG_SETMASK,&onesig,NULL);
        printf("Signal handling in parent process: \n");
        raise(SIGNAL);
        sigpending(&mask);
        if(sigismember(&mask,SIGNAL)) printf("Signal blocked in parent process\n");
        else printf("Signal not blocked in parent process\n");
        execl("./exec","./exec","mask",NULL);

    }
    else if(strcmp(argv[1],"pending") == 0){
        sigset_t onesig;
        sigset_t mask;
        sigemptyset(&onesig);
        sigemptyset(&mask);
        sigaddset(&mask,SIGNAL);
        sigprocmask(SIG_SETMASK,&mask,NULL);
        raise(SIGNAL);
        printf("Signal handling in parent process: \n");
        sigset_t w_mask;
        sigpending(&w_mask);
        if(sigismember(&mask,SIGNAL)) printf("Signal pending in parent process\n");
        execl("./exec","./exec","pending",NULL);
    }
    return 0;





}