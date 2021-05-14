#include <stdio.h>
#include "utils.h"


int main(){

    int table_fd = get_segment(TABLE_PATH);    

    struct table *table = mmap(NULL,TABLE_MEM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,table_fd,0);

    if(table == NULL){
        perror("Cook: Error mapping table");
        return -1;
    }

    sem_t * table_sem = sem_open(TABLE_PATH,O_RDWR,0644);

    if(table_sem == (void *) -1){
        perror("Error getting table semaphore");
        exit(1);
    } 


    while(1){
        sem_wait(table_sem);
        while(table->available_slots == MAX_SLOTS){
                sem_post(table_sem);
                sleep(0.5);
                sem_wait(table_sem);

        }
        int pizza = table->slots[table->pizza_ptr];
        table->pizza_ptr = (table->pizza_ptr+1) % MAX_SLOTS;
        table->available_slots++;
        sem_post(table_sem);
        printf("Delivery %d took pizza %d, pizzas left on the table %d\n",getpid(),pizza,MAX_SLOTS-table->available_slots);


        sleep(4);

        printf("Delivery %d delivered pizza %d\n",getpid(),pizza);

        sleep(4);


    }



    return 0;
}