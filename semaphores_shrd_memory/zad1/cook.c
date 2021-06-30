#include <stdio.h>

#include "utils.h"

int main(){

    int oven_id = get_segment_id(OVEN_PATH, OVEN_ID, OVEN_MEM_SIZE);
    int table_id = get_segment_id(TABLE_PATH,TABLE_ID,TABLE_MEM_SIZE);

    struct oven *oven = shmat(oven_id,NULL,0);

    if(oven == NULL){
        perror("COOK: Error attaching oven");
        return -1;
    }

    struct table *table = shmat(table_id,NULL,0);

    if(table == NULL){
        perror("Cook: Error attaching table");
        return -1;
    }


    int sem_key = ftok(SEM_PATH,SEM_ID);
    if(sem_key == -1){
        perror("Cook: Error creating semaphore key");
        exit(1);
    }
    int sem_id = semget(sem_key,SEM_COUNT,0);
    if(sem_id == -1){
        perror("Cook: Error getting semaphore");
        exit(1);
    }

    srand(time(NULL)* getpid()); 

    while(1){
        int pizza = rand() % 9;
        printf("Cook %d making pizza %d\n", getpid(),pizza);
        sleep(2);
        
        lock(sem_id,OVEN_SEM);
        while(oven->available_slots == 0){
                unlock(sem_id,OVEN_SEM);
                sleep(0.5);
                lock(sem_id,OVEN_SEM);
        }
    
        int slot_id = oven->slot_ptr;
        oven->slots[oven->slot_ptr] = pizza;
        oven->slot_ptr = (oven->slot_ptr+1) % MAX_SLOTS;
        oven->available_slots--;
        printf("Cook %d added pizza %d, pizzas on the stove %d\n", getpid(),pizza,MAX_SLOTS-oven->available_slots);

        unlock(sem_id,OVEN_SEM);


        sleep(5);

        lock(sem_id,OVEN_SEM); 
        oven->slots[slot_id] = EMPTY_SLOT;
        oven->available_slots++;
        unlock(sem_id,OVEN_SEM);
        printf("Cook %d, took out pizza %d, pizzas on the stove %d\n",getpid(),pizza,MAX_SLOTS-oven->available_slots);

        lock(sem_id,TABLE_SEM);
        while(table->available_slots == 0){
             unlock(sem_id,TABLE_SEM);
             sleep(0.5);
             
             lock(sem_id,TABLE_SEM);
        }
        
        table->slots[table  ->free_ptr] = pizza;
        table->free_ptr = (table->free_ptr+1) % MAX_SLOTS;
        table->available_slots--;
        unlock(sem_id,TABLE_SEM);
        printf("Cook %d, put pizza %d on the table, pizzas on the table %d\n",getpid(),pizza,MAX_SLOTS-table->available_slots);
         

    }

    return 0;
}