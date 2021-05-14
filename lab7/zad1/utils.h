#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
 #include <sys/sem.h>
#include <fcntl.h>
#include <sys/shm.h>


#define OVEN_PATH "cook.c"
#define OVEN_ID 1
#define OVEN_MEM_SIZE 1024


#define TABLE_PATH "delivery.c"
#define TABLE_ID 1
#define TABLE_MEM_SIZE 1024

#define SEM_PATH "utils.c"
#define SEM_ID 1
#define SEM_COUNT 2
#define OVEN_SEM 0
#define TABLE_SEM 1

#define MAX_SLOTS 5

#define EMPTY_SLOT -1


int get_segment_id(char *, int, int);
void remove_segment(char * , int , int );
void lock(int, int);
void unlock(int,int);

struct oven{
    int slots[MAX_SLOTS];
    int slot_ptr;
    int available_slots;
};

struct table{
    int slots[MAX_SLOTS];
    int free_ptr;
    int pizza_ptr;
    int available_slots;
};

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
} arg;

#endif

