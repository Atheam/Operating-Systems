#include "utils.h"

int get_segment_id(char * pathname, int proj_id, int mem_size){
    key_t key = ftok(pathname,proj_id);
    if(key == -1){
        perror("Error getting segment id");
        exit(1);
    }

    int segment_id = shmget(key,mem_size,0644);

    return segment_id;
}

void remove_segment(char * pathname, int proj_int, int mem_size){
    int segment_id = get_segment_id(pathname,proj_int,mem_size);
    if(shmctl(segment_id,IPC_RMID,NULL) == -1){
        perror("Error removing segment");
    }
}

void lock(int sem_id, int sn){
    struct sembuf * sembuf = malloc(sizeof(struct sembuf));
    sembuf->sem_op = -1;
    sembuf->sem_flg = 0;
    sembuf->sem_num = sn;
    if(semop(sem_id,sembuf,1) == -1){
        perror("Error locking semaphore");
        exit(1);
    }
    
}

void unlock(int sem_id, int sn){
    struct sembuf * sembuf = malloc(sizeof(struct sembuf));
    sembuf->sem_op = 1;
    sembuf->sem_flg = 0;
    sembuf->sem_num = sn;
    if(semop(sem_id,sembuf,1) == -1){
        perror("Error unlocking semaphore");
        exit(1);
    }
}