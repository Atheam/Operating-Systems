#include "vals.h"
struct client clients[CLIENT_MAX];

void send_msg(struct messege*,int);
void handle_msg(struct messege*);
void listen(int);
void init_client(struct messege*);
void init_clients_slots();
void list(struct messege*);
int check_client_id(int id);
void connect(struct messege*);
void disconnect(struct messege*);
void stop(struct messege*);

int server_queue;


void send_msg(struct messege *msg,int queue){
    if(msgsnd(queue,msg,sizeof(struct messege) - sizeof(long),0) == - 1){
        printf("COULDNT SEND A MSG\n");

    } 
}


int check_client_id(int id){
    for(int i =0;i<CLIENT_MAX;i++){
        if(clients[i].client_id == id){
            if(clients[i].status == UNINITIALIZED) return -1;
            else return i;
        }        
    }
    return -1;
}


void stop(struct messege* msg){
    int client_slot = check_client_id(msg->minfo.client_id);
    struct messege response;
    response.mtype = STOP;
    if(clients[client_slot].status == CONNECTED){
        sprintf(response.mtext,"You need to disconnect first");
    }
    else{
        clients[client_slot].status = UNINITIALIZED;
        sprintf(response.mtext,"Client stopped");
        printf("Client with id: %d has been stopped\n",clients[client_slot].client_id);
    }

    send_msg(&response,clients[client_slot].queue);
}


void disconnect(struct messege* msg){
    int pair_slot,client_slot;
    struct messege client_response;
    client_response.mtype = DISCONNECT;

    client_slot = check_client_id(msg->minfo.client_id);
    if((pair_slot = check_client_id(msg->minfo.pair_id)) == -1){

        sprintf(client_response.mtext,"Specified client is not initialized");
    
    }
    else if(clients[pair_slot].status != CONNECTED){
        
        sprintf(client_response.mtext,"Specified client is not connected");
    
    }
    else if(clients[client_slot].status != CONNECTED){

        sprintf(client_response.mtext,"You are not connected");
        
    }
    else{
        clients[client_slot].status = AVAILABLE;
        clients[pair_slot].status = AVAILABLE;
        printf("Clients id: %d and id: %d disconnected\n", clients[client_slot].client_id,clients[pair_slot].client_id);


        client_response.mtype = DISCONNECT;
        sprintf(client_response.mtext,"Disconnected");
        
        struct messege pair_response;
        pair_response.mtype = DISCONNECT;
        sprintf(pair_response.mtext,"Disconnected");
        send_msg(&pair_response,clients[pair_slot].queue);
    }
    send_msg(&client_response,clients[client_slot].queue);
    
}


void connect(struct messege* msg){
    int pair_slot,client_slot;
    struct messege client_response;
    client_response.mtype = CONNECT;
    client_response.minfo.pair_queue = -1; // -1 indicates error 

    client_slot = check_client_id(msg->minfo.client_id);
    if(clients[client_slot].status == CONNECTED){
        sprintf(client_response.mtext,"You are alread connected");

    }

    else if((pair_slot = check_client_id(msg->minfo.pair_id)) == -1){

        
        sprintf(client_response.mtext,"Specified client is not initialized");

    }
    else if(clients[pair_slot].status == CONNECTED){
        sprintf(client_response.mtext,"Specified client is busy");

    }
    
    else{
        clients[client_slot].status = CONNECTED;
        clients[pair_slot].status = CONNECTED;
        printf("Connection established between clients id: %d and id: %d\n",
        clients[client_slot].client_id,clients[pair_slot].client_id);

        
        sprintf(client_response.mtext,"Connected");
        client_response.minfo.pair_queue = clients[pair_slot].queue;
        client_response.minfo.pair_id = clients[pair_slot].client_id;
        
        struct messege pair_response;
        pair_response.mtype = CONNECT;
        pair_response.minfo.pair_id = clients[client_slot].client_id;
        pair_response.minfo.pair_queue = clients[client_slot].queue;
        sprintf(pair_response.mtext,"Connected");
        send_msg(&pair_response,clients[pair_slot].queue);

    }

    send_msg(&client_response,clients[client_slot].queue);
    
}




void list(struct messege* msg){
    int client_slot;
    char text[MAX_TEXT_LEN];
    client_slot = check_client_id(msg->minfo.client_id);

    sprintf(text,"-----CLIENTS LIST-----");

    for(int i =0;i<CLIENT_MAX;i++){
        if(clients[i].status != UNINITIALIZED){
            char buff[MAX_TEXT_LEN];
            char *status = clients[i].status == AVAILABLE ? "available" : "connected";
            sprintf(buff,"\nClient id: %d status: %s",clients[i].client_id,status);
            strcat(text,buff);  
        }
          
    }    
    struct messege response;
    response.mtype = LIST;
    strcpy(response.mtext,text);
    response.minfo.client_id = msg->minfo.client_id;    
    send_msg(&response,clients[client_slot].queue);
}








void handle_msg(struct messege *msg){
    
    switch(msg->mtype){
        case INIT:
            init_client(msg);
            break;
        case LIST:
            list(msg);
            break;
        case CONNECT:
            connect(msg);
            break;
        case DISCONNECT:
            disconnect(msg);
            break;
        case STOP:
            stop(msg);
            break;
    }
}

void listen(int server_queue){
    struct messege msg;
    
    while(1){
        if((msgrcv(server_queue,&msg,sizeof(struct messege),0,0)) == -1){
            perror("SERVER ERROR: while receiving msg");
            exit(1);
        }
        else{
            handle_msg(&msg);  
        }       
        
    }

}

void handler(){
    msgctl(server_queue,IPC_RMID,NULL);
    exit(0);

}



void init_client(struct messege *msg){
    int id = 0;

    //finding first uninitialized client slot
    while(clients[id].status != UNINITIALIZED) id++;
    struct messege response;
    response.mtype = INIT;

    if(id == CLIENT_MAX){
        printf("MAX CLIENTS REACHED\n");
        sprintf(response.mtext,"ERROR WHILE INITIALIZING");
        response.minfo.client_id = -1;
        send_msg(&response,clients[id].queue);

        return;
    }

    clients[id].status = AVAILABLE;
    clients[id].queue = msg->minfo.client_queue;

    printf("Client with id: %d has been initialized\n",id);
    
    response.minfo.client_id = id;
    sprintf(response.mtext,"YOU HAVE BEEN INITIALIZED");
    send_msg(&response,clients[id].queue);

}

void init_clients_slots(){
    for(int i =0;i < CLIENT_MAX;i++){
        clients[i].client_id = i;
        clients[i].status = UNINITIALIZED;
    }
}


int main(){

    key_t server_key;

    init_clients_slots();
    

    if((server_key = ftok($HOME,QUEUEID)) == - 1){
        perror("SERVER ERROR: while generating ftok key");
        return -1;
    }

    if((server_queue = msgget(server_key, IPC_CREAT | 0777)) == -1){
        perror("SERVER ERROR: while creaing server");
        return -1;
    }

    signal(SIGINT,handler);

    printf("SERVER LISTENING\n");
    listen(server_queue);

    msgctl(server_queue,IPC_RMID,NULL);

}