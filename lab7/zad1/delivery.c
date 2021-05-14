#include <stdio.h>
#include "utils.h"


int main(){

    int table_id = get_segment_id(TABLE_PATH,TABLE_ID,TABLE_MEM_SIZE);
    if(table_id == -1){
        perror("DELIVERY: Error attaching table");
        return -1;
    }
    struct table *table = shmat(table_id,NULL,0);

    int sem_key = ftok(SEM_PATH,SEM_ID);
    if(sem_key == -1){
        perror("Delivery: Error creating semaphore key");
        exit(1);
    }
    int sem_id = semget(sem_key,SEM_COUNT,0);
    if(sem_id == -1){
        perror("Delivery: Error getting semaphore");
        exit(1);
    }


    while(1){
        lock(sem_id,TABLE_SEM);
        while(table->available_slots == MAX_SLOTS){
                unlock(sem_id,TABLE_SEM);
                sleep(0.5);
                lock(sem_id,TABLE_SEM);

        }
        int pizza = table->slots[table->pizza_ptr];
        table->pizza_ptr = (table->pizza_ptr+1) % MAX_SLOTS;
        table->available_slots++;
        unlock(sem_id,TABLE_SEM);
        printf("Delivery %d took pizza %d, pizzas left on the table %d\n",getpid(),pizza,MAX_SLOTS-table->available_slots);


        sleep(4);

        printf("Delivery %d delivered pizza %d\n",getpid(),pizza);

        sleep(4);


    }



    return 0;
}