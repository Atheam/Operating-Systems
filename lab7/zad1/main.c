
#include "utils.h"


void init_sh(){

    key_t oven_key = ftok(OVEN_PATH,OVEN_ID);
    if(oven_key == -1){
        perror("INIT: Error creating oven key");
        exit(1);
    }

    key_t table_key = ftok(TABLE_PATH,TABLE_ID);

    if(table_key == - 1){
        perror("INIT: Error creating table key");
        exit(1);
    }

    int oven_id = shmget(oven_key,OVEN_MEM_SIZE,IPC_CREAT | 0644);

    if(oven_id == -1 ){
        perror("INIT: Error getting oven shared memory id");
        exit(1);
    }

    int table_id = shmget(table_key,OVEN_MEM_SIZE,IPC_CREAT | 0644);

    if(oven_id == -1 ){
        perror("INIT: Error getting table shared memory id");
        exit(1);
    }

    struct oven *oven = shmat(oven_id,NULL,0);

    oven->available_slots = MAX_SLOTS;
    oven->slot_ptr = 0;
    for(int i =0;i < MAX_SLOTS;i++) oven->slots[i] = EMPTY_SLOT;

    struct table *table = shmat(table_id,NULL,0);

    table->available_slots = MAX_SLOTS;
    table->free_ptr = 0;
    table->pizza_ptr = 0;
    for(int i =0;i < MAX_SLOTS;i++) table->slots[i] = EMPTY_SLOT;
    

}


void free_sh(){
    remove_segment(OVEN_PATH,1,OVEN_MEM_SIZE);
    remove_segment(TABLE_PATH,1,TABLE_MEM_SIZE);
}


void init_sem(){
    int sem_key = ftok(SEM_PATH,SEM_ID);
    if(sem_key == -1){
        perror("INIT: Error creating semaphore key");
        exit(1);
    }
    int sem_id = semget(sem_key,SEM_COUNT, 0644 | IPC_CREAT);
    if(sem_id == -1){
        perror("INIT: Error creating semaphore set");
        exit(1);
    }

    union semun arg;
    arg.val = 1;

    for(int i =0;i<SEM_COUNT;i++){
        semctl(sem_id, i, SETVAL, arg);
    }

}


void free_sem(){
    int sem_key = ftok(SEM_PATH,SEM_ID);
    if(sem_key == -1){
        perror("FREE: Error creating semaphore key");
        exit(1);
    }

    int sem_id = semget(sem_key,SEM_COUNT, 0);
    if(sem_id == -1){
        perror("FREE: Error getting semaphore set");
        exit(1); 
    }

    for(int i = 0;i < SEM_COUNT;i++){
        semctl(sem_id, i, IPC_RMID, NULL);

    }
    

}


int main(int argc, char *argv[]){

    if(argc < 3){
        perror("Wrong arguments");
        return -1;
    }

    const int N = atoi(argv[1]);
    const int M = atoi(argv[2]);

    init_sh();
    init_sem();

    for(int i = 0;i<N;i++){
        if(fork() == 0){
            execlp("./cook","./cook",NULL);
        }
    }

    for(int i = 0;i<M;i++){
        if(fork() == 0){
            execlp("./delivery","./delivery",NULL);
        }
    }

    for(int i =0;i<N+M;i++){
        wait(NULL);
    }


    free_sh();
    free_sem();



    return 0;
}