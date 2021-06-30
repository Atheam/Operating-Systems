
#include "utils.h"


void init_sh(){

    int oven_fd = shm_open(OVEN_PATH,O_RDWR | O_CREAT,0644);
    if(oven_fd == -1){
        perror("INIT: Error opening oven shared memory");
        exit(1);
    }

    int table_fd = shm_open(TABLE_PATH,O_RDWR | O_CREAT,0644);
    if(table_fd == -1){
        perror("INIT: Error opening table shared memory");
        exit(1);
    }

    if(ftruncate(oven_fd,OVEN_MEM_SIZE) < 0){
        perror("INIT: Error truncating oven shared memory");
        exit(1);
    }

    if(ftruncate(table_fd,TABLE_MEM_SIZE) < 0){
        perror("INIT: Error truncating table shared memory");
        exit(1);
    }
    
    struct oven *oven = mmap(NULL,OVEN_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,oven_fd,0);
    oven->available_slots = MAX_SLOTS;
    oven->slot_ptr = 0;
    for(int i =0;i < MAX_SLOTS;i++) oven->slots[i] = EMPTY_SLOT;

    struct table *table = mmap(NULL,OVEN_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,table_fd,0);

    table->available_slots = MAX_SLOTS;
    table->free_ptr = 0;
    table->pizza_ptr = 0;
    for(int i =0;i < MAX_SLOTS;i++) table->slots[i] = EMPTY_SLOT;
    
}


void free_sh(){
    remove_segment(OVEN_PATH);
    remove_segment(TABLE_PATH);
}


void init_sem(){

    sem_t * oven_sem = sem_open(OVEN_PATH,O_RDWR | O_CREAT,0644,1);

    if(oven_sem == (void *) -1){
        perror("Error creating oven semaphore");
        exit(1);

    } 

    sem_close(oven_sem);

    sem_t * table_sem = sem_open(TABLE_PATH,O_RDWR | O_CREAT,0644,1);

    if(table_sem == (void *) -1){
        perror("Error creating table semaphore");
        exit(1);
    } 

    sem_close(table_sem);

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
    



    return 0;
}