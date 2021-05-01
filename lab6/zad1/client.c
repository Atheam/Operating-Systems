#include "vals.h"


int pair_queue, client_queue,server_queue;
int client_id,pair_id;
int mode;

void init_client();
void send_msg(struct messege*,int);
void *listen(void *arg);
void handle_msg(struct messege*);
void rcv_list(struct messege*);
void rcv_disconnect(struct messege*);
void rcv_stop(struct messege*);
void rcv_connect(struct messege*);
void rcv_init(struct messege*);
void rcv_text(struct messege*);
void list_request(int);
void connect_request(int ,int);
void disconnect_request(int ,int);
void stop_request(int);
void text_request(char *);


void send_msg(struct messege *msg,int queue){
    if(msgsnd(queue,msg,sizeof(struct messege) - sizeof(long),0) == - 1) printf("COULDNT SEND A MSG\n"); 
}

void init_client(int server_queue,int client_queue){
    printf("----INITIALIZING CLIENT----\n");
    struct messege msg;
    msg.mtype = INIT;
    msg.minfo.client_queue = client_queue;
    send_msg(&msg,server_queue);
}

void handler(){
    if(mode == WRITE) disconnect_request(server_queue,pair_id);
    sleep(0.5);
    stop_request(server_queue);
    sleep(0.5);
    msgctl(client_queue,IPC_RMID,NULL);
    exit(0);
}

void handle_msg(struct messege *msg){
    
    switch(msg->mtype){
        case INIT:
            rcv_init(msg);
            break;
        case LIST:
            rcv_list(msg);
            break;
        case CONNECT:
            rcv_connect(msg);
            break;
        case DISCONNECT:
            rcv_disconnect(msg);
            break;
        case STOP:
            rcv_stop(msg);
            break;
        case TEXT:
            rcv_text(msg);
            break;
    }
}


void *listen(void *arg){
    while(1){
        struct messege msg;
        if((msgrcv(client_queue,&msg,sizeof(struct messege),0,0)) == -1){
            perror("CLIENT ERROR: while receiving msg");
            exit(1);
        }
        handle_msg(&msg);
    }
}


void rcv_init(struct messege *msg){
    printf("%s\n",msg->mtext);
    if(msg->minfo.client_id != -1) client_id = msg->minfo.client_id;

}
void rcv_list(struct messege *msg){
    printf("%s\n",msg->mtext);
}
void rcv_connect(struct messege *msg){
    printf("%s\n",msg->mtext);
    if(msg->minfo.pair_queue != -1){
        pair_queue = msg->minfo.pair_queue;
        pair_id = msg->minfo.pair_id;
        mode = WRITE;

    } 
}

void rcv_disconnect(struct messege *msg){
    printf("%s\n",msg->mtext);
    mode = COMMAND;

}
void rcv_stop(struct messege *msg){
    printf("%s\n",msg->mtext);
    msgctl(client_queue,IPC_RMID,NULL);
    exit(0);
}

void rcv_text(struct messege *msg){
    printf("%s",msg->mtext);
}

void list_request(int server_queue){
    
    struct messege msg;
    msg.minfo.client_id = client_id;
    msg.mtype = LIST;
    send_msg(&msg,server_queue);

}

void connect_request(int server_queue,int pair_id){
    struct messege msg;
    msg.minfo.client_id = client_id;
    msg.minfo.pair_id = pair_id;
    msg.mtype = CONNECT;
    send_msg(&msg,server_queue);
}

void disconnect_request(int server_queue,int pair_id){
    struct messege msg;
    msg.minfo.client_id = client_id;
    msg.minfo.pair_id = pair_id;
    msg.mtype = DISCONNECT;
    send_msg(&msg,server_queue);
}

void stop_request(int server_queue){
    struct messege msg;
    msg.minfo.client_id = client_id;
    msg.mtype = STOP;
    send_msg(&msg,server_queue);
}

void text_request(char * text){
    struct messege msg;
    msg.minfo.client_id = client_id;
    msg.mtype = TEXT;
    strcpy(msg.mtext,text);
    send_msg(&msg,pair_queue);
}

int main(){
    key_t server_key,client_key;

    if((server_key = ftok($HOME,QUEUEID)) == - 1){
         perror("CLIENT ERROR: while generating ftok key");
         return -1;
     }

     if((server_queue = msgget(server_key,0)) == -1){
         perror("CLIENT ERROR: while getting server queue");
         return -1;
     }

     if((client_key = ftok($HOME,getpid())) == - 1){
         perror("CLIENT ERROR: while generating ftok key");
         return -1;
     }

     if((client_queue = msgget(client_key,IPC_CREAT|0777)) == -1){
         perror("CLIENT ERROR: while creaing client queue");
         return -1;
     }
     signal(SIGINT,handler);

     pthread_t thread_id;
     pthread_create(&thread_id,NULL,listen,NULL);

     init_client(server_queue,client_queue);
     
     char *line = NULL;
     size_t len = 0;
     ssize_t nread = 0;
     

     mode = COMMAND;
     while((nread = getline(&line,&len,stdin)) != -1){
         if(mode == COMMAND){
            if(strcmp(line,"LIST\n") == 0) list_request(server_queue);

            else if(strcmp(line,"CONNECT\n") == 0){
                printf("Enter id:");
                char buff[16];
                fgets(buff,16,stdin);
                connect_request(server_queue,atoi(buff));
            }
            else if(strcmp(line,"STOP\n") == 0) stop_request(server_queue);
            else{
                printf("UNRECOGNIZED COMMAND\n");
            }
         }
         else if(mode == WRITE){
            if(strcmp(line,"DISCONNECT\n") == 0) disconnect_request(server_queue,pair_id);
            else text_request(line);
         }
         
     }
     free(line);

     pthread_join(thread_id,NULL);

     
}