#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


int SA_COUNTER = 0;
int SA_SENT = 0;
int no_to_send;
pid_t catcher_pid;


void sig1_handler(int signum){
    SA_COUNTER++;
    if(SA_SENT >= no_to_send){
        kill(catcher_pid,SIGUSR2);
    }
    else{
        kill(catcher_pid,SIGUSR1);
        SA_SENT++;
    }
    
}
void sig2_handler(int signum){
    printf("Number of signals sent by sender: %d\n",no_to_send);
    printf("Numbers of signals received back by sender: %d\n", SA_COUNTER);
    exit(0);
}

void sig1_queue_handler(int signum){
    union sigval val;
    SA_COUNTER++;
    if(SA_SENT >= no_to_send){
        sigqueue(catcher_pid,SIGUSR2,val);
    }
    else{
        sigqueue(catcher_pid,SIGUSR1,val);
        SA_SENT++;
    }

}

void sig1_RT_handler(int signum){
    SA_COUNTER++;
    if(SA_SENT >= no_to_send){
        kill(catcher_pid,SIGRTMIN+1);
    }
    else{
        kill(catcher_pid,SIGRTMIN+0);
        SA_SENT++;
    }
    
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
    catcher_pid = atoi(argv[1]);
    if(atoi(argv[2]) < 0){
        perror("Number of signals must be a positive number\n");
        return 1;
    }
    no_to_send = atoi(argv[2]);

    
   
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
        kill(catcher_pid,SIGUSR1);
        SA_SENT++;
    }
    if(strcmp(argv[3],"SIGQUEUE") == 0){
        signal(SIGUSR1,sig1_queue_handler);
        signal(SIGUSR2,sig2_handler);
        kill(catcher_pid,SIGUSR1);
        SA_SENT++;
    }
    if(strcmp(argv[3],"SIGRT") == 0){
        signal(SIGRTMIN+0,sig1_RT_handler);
        signal(SIGRTMIN+1,sig2_handler);
        kill(catcher_pid,SIGRTMIN+0);
        SA_SENT++;

    }



    while(1){
        sigsuspend(&mask);
    }
    


    return 0;

}