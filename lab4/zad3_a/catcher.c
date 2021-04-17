#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int COUNT = 0;

void sig1_handler(int signum){
    COUNT++;
}

void sig2_handler(int signum,siginfo_t *siginfo, void* context){
    
    for(int i = 0;i<COUNT;i++){
        kill(siginfo->si_pid,SIGUSR1);
    }
    kill(siginfo->si_pid,SIGUSR2);
    printf("Number of received signals by catcher: %d\n", COUNT);
    exit(0);
    
}

void sig2_RT_handler(int signum,siginfo_t * siginfo,void*context){
     for(int i = 0;i<COUNT;i++){
        kill(siginfo->si_pid,SIGRTMIN+0);
    }
    kill(siginfo->si_pid,SIGRTMIN+1);
    printf("Number of received signals by catcher: %d\n", COUNT);
    exit(0);
    

}

void sig_queue_handler(int signum,siginfo_t *siginfo,void * context){
    
    union sigval val;
    for(int i =0;i<COUNT;i++){
        val.sival_int = i+1;
        sigqueue(siginfo->si_pid,SIGUSR1,val);
    }
    val.sival_int = COUNT;
    sigqueue(siginfo->si_pid,SIGUSR2,val);
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

    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    if(strcmp(argv[1],"KILL") == 0){
        signal(SIGUSR1,sig1_handler);
        act.sa_sigaction = sig2_handler;
        sigaction(SIGUSR2,&act,NULL);
    }
    if(strcmp(argv[1],"SIGQUEUE") == 0){
        signal(SIGUSR1,sig1_handler);
        struct sigaction act;
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction = sig_queue_handler;
        sigaction(SIGUSR2,&act,NULL);
    }
    if(strcmp(argv[1],"SIGRT") == 0){
        signal(SIGRTMIN+0,sig1_handler);
        act.sa_sigaction = sig2_RT_handler;
        sigaction(SIGRTMIN+1,&act,NULL);
    }

    while(1){
        sigsuspend(&mask);
    }

}