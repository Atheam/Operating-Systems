#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


int SA_COUNTER = 0;

void sig1_handler(int signum){
    SA_COUNTER++;
}

void sig2_handler(int signum){
    printf("Numbers of signals received back by sender: %d\n", SA_COUNTER);
    exit(0);
}

void sigqueue_handler(int signum,siginfo_t *siginfo, void* context){
    SA_COUNTER++;
    printf("Signals sent by catcher: %d, signals received from catcher: %d\n", siginfo->si_value.sival_int,SA_COUNTER);
}


int main(int argc,char* argv[]){

    if(argc < 4){
        perror("Wrong number of args\n");
        return 1;
    }
    if(atoi(argv[1]) < 0){
        perror("Wrong catcher PID\n");
        return 1;
    }
    pid_t catcher_pid = atoi(argv[1]);
    if(atoi(argv[2]) < 0){
        perror("Number of signals must be a positive number\n");
        return 1;
    }
    int no_to_send = atoi(argv[2]);

    
   
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGUSR2);
    sigdelset(&mask,SIGINT);
    sigdelset(&mask,SIGRTMIN+0);
    sigdelset(&mask,SIGRTMIN+1);
    sigprocmask(SIG_SETMASK,&mask,NULL);
  
    if(strcmp(argv[3],"KILL") == 0){
        signal(SIGUSR1,sig1_handler);
        signal(SIGUSR2,sig2_handler);
        for(int i = 0;i<no_to_send;i++)kill(catcher_pid,SIGUSR1);
        printf("Number of signals sent by sender: %d\n",no_to_send);
        kill(catcher_pid,SIGUSR2);
        
    }

    if(strcmp(argv[3],"SIGQUEUE") == 0){
        struct sigaction act;
        act.sa_sigaction = sigqueue_handler;
        act.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1,&act,NULL);
        signal(SIGUSR2,sig2_handler);
        
        for(int i = 0;i<no_to_send;i++){
            union sigval val;
            val.sival_int = i;
            sigqueue(catcher_pid,SIGUSR1,val);
        }
        union sigval val;
        val.sival_int = no_to_send;
        printf("Number of signals sent by sender: %d\n",no_to_send);
        sigqueue(catcher_pid,SIGUSR2,val);
        
    }
    if(strcmp(argv[3],"SIGRT") == 0){
        signal(SIGRTMIN+0,sig1_handler);
        signal(SIGRTMIN+1,sig2_handler);
        for(int i = 0;i<no_to_send;i++)kill(catcher_pid,SIGRTMIN+0);
        printf("Number of signals sent by sender: %d\n",no_to_send);
        kill(catcher_pid,SIGRTMIN+1);
        
    }

    while(1){
        sigsuspend(&mask);
    }
    


    return 0;

}