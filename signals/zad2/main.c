#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>


#define SIGNAL SIGUSR1

void info_handler(int signum, siginfo_t *siginfo,void *context){
    printf("Handling signal number: %d\n",signum);
    printf("SIGINFO content\n");
    printf("Signal number: %d\n",siginfo->si_signo);
    printf("Sending process PID: %d\n",siginfo->si_pid);
    printf("Signal code: %d\n",siginfo->si_code);
    printf("Seding process real user ID: %d\n",siginfo->si_uid);
}

void reset_handler(int signum){
    printf("Handling signal number: %d\n", signum);
}

void stop_handler(int signum){
    printf("Handling signal number: %d\n",signum);
}



int main(){
    printf("-----HANDLING SINGAL WITH SIGINFO FLAG-----\n");
    struct sigaction act_info;
    act_info.sa_flags = SA_SIGINFO;
    act_info.sa_sigaction = info_handler;
    sigaction(SIGNAL,&act_info,NULL);
    raise(SIGNAL);
   

    printf("\n\n-----HANDLING SIGNAL WITH NOCLDSTOP FLAG-----\n");
    printf("First case when flag is not set\n");
    
    struct sigaction act_stop;
    act_stop.sa_handler = stop_handler;
    sigaction(SIGCHLD,&act_stop,NULL);
    pid_t pid;

    if((pid = fork())== 0){
        while(1);
    }
    else{
        printf("SIGSTOP sent: \n");
        kill(pid,SIGSTOP);
        sleep(1);
    }
    
    printf("Second case when the flag is set: \n");

    act_stop.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD,&act_stop,NULL);

    if((pid = fork()) == 0){
        while(1);
    }
    else{
        printf("SIGSTOP sent: \n");
        kill(pid,SIGSTOP);
        sleep(1);
    }
    

    printf("\n\n-----HANDLING SIGNAL WITH RESETHAND FLAG-----\n");
    struct sigaction act_reset;
    act_reset.sa_flags = SA_RESETHAND;
    act_reset.sa_handler = reset_handler;
    sigaction(SIGNAL,&act_reset,NULL);
    printf("FIRST CALL\n");
    raise(SIGNAL);
    printf("SECOND CALL\n");
    raise(SIGNAL);

    return 0;

    


}