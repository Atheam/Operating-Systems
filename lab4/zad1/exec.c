#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define SIGNAL SIGUSR1

int main(int argc,char *argv[]){
    if(strcmp(argv[1],"ignore") == 0){
        printf("Signal handling in exec:\n");
        raise(SIGNAL);
        printf("Signal ignored in exec\n");
    }
    if(strcmp(argv[1],"mask")== 0){
        printf("Signal handling in exec:\n");
        sigset_t mask;
        raise(SIGNAL);
        sigemptyset(&mask);
        sigpending(&mask);
        if(sigismember(&mask,SIGNAL)) printf("Signal blocked in exec\n");
        else printf("Signal not blocked in exec\n");
    }
    if(strcmp(argv[1],"pending")== 0){
        printf("Singal handling in exec:\n");
        sigset_t w_mask;
        sigpending(&w_mask);
        if(sigismember(&w_mask,SIGNAL)) printf("Signal pending in exec\n");
        else printf("Signal not pending in exec\n");
    }
}