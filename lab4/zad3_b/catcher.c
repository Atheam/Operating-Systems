#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int COUNT = 0;

void sig1_handler(int signum,siginfo_t *siginfo, void* context){
    kill(siginfo->si_pid,SIGUSR1);
    COUNT++;
}

void sig2_handler(int signum,siginfo_t *siginfo, void* context){
    kill(siginfo->si_pid,SIGUSR2);
    printf("Number of received signals by catcher: %d\n", COUNT);
    exit(0);
}

void sig1_queue_handler(int signum,siginfo_t *siginfo, void* context){
    union sigval val;
    sigqueue(siginfo->si_pid,SIGUSR1,val);
    COUNT++;
}

void sig2_queue_handler(int signum,siginfo_t *siginfo, void* context){
    union sigval val;
    sigqueue(siginfo->si_pid,SIGUSR2,val);
    printf("Number of received signals by catcher: %d\n", COUNT);
    exit(0);
}

void sig1_RT_handler(int signum,siginfo_t *siginfo, void* context){
    kill(siginfo->si_pid,SIGRTMIN+0);
    COUNT++;
}

void sig2_RT_handler(int signum,siginfo_t *siginfo, void* context){
    kill(siginfo->si_pid,SIGRTMIN+1);
    printf("Number of received signals by catcher: %d\n", COUNT);
    exit(0);
}



int main(int argc,char* argv[]){

    if(argc < 2){
        perror("Wrong number of args");
        return 1;
    }

    printf("PID of catcher program: %d im waiting for signals.....\n",getpid());
    
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGUSR2);
    sigdelset(&mask,SIGINT);
    sigdelset(&mask,SIGRTMIN+0);
    sigdelset(&mask,SIGRTMIN+1);
    sigprocmask(SIG_SETMASK,&mask,NULL);
    struct sigaction sig1_act;
    sig1_act.sa_flags = SA_SIGINFO;
    struct sigaction sig2_act;
    sig2_act.sa_flags = SA_SIGINFO;
    
    if(strcmp(argv[1],"KILL") == 0){
        sig1_act.sa_sigaction = sig1_handler;
        sigaction(SIGUSR1,&sig1_act,NULL);
        sig2_act.sa_sigaction = sig2_handler;
        sigaction(SIGUSR2,&sig2_act,NULL);
    }
    if(strcmp(argv[1],"SIGQUEUE") == 0){
        sig1_act.sa_sigaction = sig1_queue_handler;
        sigaction(SIGUSR1,&sig1_act,NULL);
        sig2_act.sa_sigaction = sig2_queue_handler;
        sigaction(SIGUSR2,&sig2_act,NULL);
    }
    if(strcmp(argv[1],"SIGRT") == 0){
        sig1_act.sa_sigaction = sig1_RT_handler;
        sigaction(SIGRTMIN+0,&sig1_act,NULL);
        sig2_act.sa_sigaction = sig2_RT_handler;
        sigaction(SIGRTMIN+1,&sig2_act,NULL);

    }
   
    while(1){
        sigsuspend(&mask);
    }

}