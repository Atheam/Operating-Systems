#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


#define RENIFERS_N 9
#define ELVES_N 10
#define END_PRESENT_N 3
#define PROBLEM_ELVES_TRIGGER 3

int no_elves_waiting = 0;
int available_renifers = 0;
int idle = 0;
int elves_waiting[ELVES_N];


pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t renifers_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elves_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t renifers_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t elves_cond = PTHREAD_COND_INITIALIZER;


void *santa(void * arg){
    int shipped = 0;
    
    while(shipped != END_PRESENT_N){
        pthread_mutex_lock(&santa_mutex);
        while(no_elves_waiting < PROBLEM_ELVES_TRIGGER && available_renifers < RENIFERS_N){
            pthread_cond_wait(&santa_cond,&santa_mutex);
        }
        pthread_mutex_unlock(&santa_mutex);

        pthread_mutex_lock(&renifers_mutex);
        if(available_renifers == RENIFERS_N){
            puts("Mikolaj: dostarczam zabawki");
            idle = 0;
            srand(time(NULL));
            int sleep_time = (rand()* pthread_self()) % (4 - 2 + 1) + 2;
            sleep(sleep_time);
            shipped++;
            available_renifers = 0;
            pthread_cond_broadcast(&renifers_cond);
        }
        pthread_mutex_unlock(&renifers_mutex);


        if(shipped == END_PRESENT_N){
            puts("UDALO SIE DOSTARCZYC WSZYSTKIE ZABAWKI!");
            exit(0);
        }
        
        pthread_mutex_lock(&elves_mutex);
        if(no_elves_waiting == PROBLEM_ELVES_TRIGGER){
            printf("Mikolaj rozwiazuje problemy elfow %d, %d, %d,\n",elves_waiting[0],elves_waiting[1],elves_waiting[2]);
            srand(time(NULL));
            int sleep_time = (rand()* pthread_self()) % (2 - 1 + 1) + 1;
            sleep(sleep_time);
            no_elves_waiting = 0;
            pthread_cond_broadcast(&elves_cond);
        }
        pthread_mutex_unlock(&elves_mutex);
        

        puts("Zasypiam");
    }
    return NULL;
}

void* renifer(void * arg){
    int *ID = malloc(sizeof(int));
    *ID = *(int*) arg;
    while(1){

        pthread_mutex_lock(&renifers_mutex);
        while(idle == 1){
            pthread_cond_wait(&renifers_cond,&renifers_mutex);
        }
        
        pthread_mutex_unlock(&renifers_mutex);

        srand(time(NULL));
        int sleep_time = (rand()* pthread_self()) % (10 - 5 + 1) + 5;
        sleep(sleep_time);

        pthread_mutex_lock(&renifers_mutex);
        available_renifers++;
        idle = 1;
        printf("Renifer: czeka %d reniferow na mikolaja ID : %d\n",available_renifers,*ID);

        if(available_renifers == RENIFERS_N){
            printf("Renifer: wybudzam mikolaja, ID: %d\n",*ID);
            pthread_mutex_lock(&santa_mutex);
            pthread_cond_broadcast(&santa_cond);
            pthread_mutex_unlock(&santa_mutex);
        }

        pthread_mutex_unlock(&renifers_mutex);
    }
    return NULL;
}

void * elf(void * arg){
    int *ID = malloc(sizeof(int));
    *ID = * (int*) arg;
    
    while(1){
        srand(time(NULL));
        int sleep_time = (rand()* pthread_self()) % (5 - 2 + 1) + 2;
        sleep(sleep_time);

        pthread_mutex_lock(&elves_mutex);
        while(no_elves_waiting >= PROBLEM_ELVES_TRIGGER){
            printf("Elf: czeka na powrot elfow ID: %d\n",*ID);
            pthread_cond_wait(&elves_cond,&elves_mutex);
        }
        
        elves_waiting[no_elves_waiting++] = *ID;
        printf("Elf : czeka %d elfow na mikolaja ID: %d\n", no_elves_waiting,*ID);
        if(no_elves_waiting == PROBLEM_ELVES_TRIGGER){
            pthread_mutex_lock(&santa_mutex);
            printf("Elf: wybudzam mikolaja, ID: %d\n",*ID);
            pthread_cond_broadcast(&santa_cond);
            pthread_mutex_unlock(&santa_mutex);
        }

        pthread_mutex_unlock(&elves_mutex);
    }
    return NULL;
}

int main(){

    pthread_t elves_th[ELVES_N];
    pthread_t renifers_th[RENIFERS_N];
    pthread_t santa_th;


    pthread_create(&santa_th,NULL,&santa,NULL);

    
    for(int i = 0;i<RENIFERS_N;i++){
        int *ID = malloc(sizeof(int));
        *ID = i+1;
        pthread_create(&renifers_th[i],NULL,&renifer,ID);
    }
    
    for(int i = 0;i<ELVES_N;i++){
        int *ID = malloc(sizeof(int));
        *ID = i+1;
        pthread_create(&elves_th[i],NULL,&elf,ID);
    }
    pthread_join(santa_th,NULL);

    for(int i =0;RENIFERS_N;i++){
        pthread_join(renifers_th[i],NULL);
    }

    for(int i =0;i<ELVES_N;i++){
        pthread_join(elves_th[i],NULL);
    }
    
    return 0;
}