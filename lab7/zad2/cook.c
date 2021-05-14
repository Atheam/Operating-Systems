#include <stdio.h>

#include "utils.h"

int main(){


    int oven_fd = get_segment(OVEN_PATH);    

    struct oven *oven = mmap(NULL,OVEN_MEM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,oven_fd,0);

    if(oven == NULL){
        perror("COOK: Error mapping oven");
        return -1;
    }

    int table_fd = get_segment(TABLE_PATH);    

    struct table *table = mmap(NULL,TABLE_MEM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,table_fd,0);

    if(table == NULL){
        perror("Cook: Error mapping table");
        return -1;
    }

    
    sem_t * oven_sem = sem_open(OVEN_PATH,O_RDWR,0644);

    if(oven_sem == (void *) -1){
        perror("Error getting oven semaphore");
        exit(1);
    } 


    sem_t * table_sem = sem_open(TABLE_PATH,O_RDWR,0644);

    if(table_sem == (void *) -1){
        perror("Error getting table semaphore");
        exit(1);
    } 


   

    srand(time(NULL)* getpid()); 

    while(1){
        int pizza = rand() % 9;
        printf("Cook %d making pizza %d\n", getpid(),pizza);
        sleep(2);
        
        sem_wait(oven_sem);
        while(oven->available_slots == 0){
                sem_post(oven_sem);
                sleep(0.5);
                sem_wait(oven_sem);
        }
    
        int slot_id = oven->slot_ptr;
        oven->slots[oven->slot_ptr] = pizza;
        oven->slot_ptr = (oven->slot_ptr+1) % MAX_SLOTS;
        oven->available_slots--;
        printf("Cook %d added pizza %d, pizzas on the stove %d\n", getpid(),pizza,MAX_SLOTS-oven->available_slots);

        sem_post(oven_sem);


        sleep(5);

        sem_wait(oven_sem);
        oven->slots[slot_id] = EMPTY_SLOT;
        oven->available_slots++;
        sem_post(oven_sem);
        printf("Cook %d, took out pizza %d, pizzas on the stove %d\n",getpid(),pizza,MAX_SLOTS-oven->available_slots);

        sem_wait(table_sem);
        while(table->available_slots == 0){
             sem_post(table_sem);
             sleep(0.5);
             
             sem_wait(table_sem);
        }
        
        table->slots[table  ->free_ptr] = pizza;
        table->free_ptr = (table->free_ptr+1) % MAX_SLOTS;
        table->available_slots--;
        sem_post(table_sem);
        printf("Cook %d, put pizza %d on the table, pizzas on the table %d\n",getpid(),pizza,MAX_SLOTS-table->available_slots);
         

    }

    return 0;
}