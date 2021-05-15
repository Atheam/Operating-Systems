#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_SCALE 255

int **img;
int no_threads;
int col_num,row_num;

void read_img(FILE * file){
    if(file == NULL){
        perror("READ IMG: error with in file");
        exit(1);
    }

    for(int i =0;i<row_num;i++){
        for(int j =0;j<col_num;j++){
            if(fscanf(file,"%d",&img[i][j]) != 1){
                perror("READ IMG: Image corrupted");
                exit(1);
            }
        }
    }
}

void save_img(FILE * file){
    if(file == NULL){
        perror("SAVE IMG: error with out file");
        exit(1);
    }
    
    fprintf(file,"%s\n%d %d\n%d\n","P2",col_num,row_num,MAX_SCALE);
    
    for(int i =0;i<row_num;i++){
        for(int j= 0;j<col_num-1;j++){
            fprintf(file,"%d ",img[i][j]);
        }
        fprintf(file,"%d\n",img[i][col_num-1]);
    }
}

void * compose_block(void *arg){
    
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time,NULL);
    
    
    int thread_num = *(int*) arg + 1;

    for(int i =0;i<row_num;i++){

        int left = (thread_num -1) * (col_num/no_threads);
        int right = thread_num * (col_num/no_threads) ;

        for(int j = left;j<right;j++){
            img[i][j] = MAX_SCALE - img[i][j];
        }
    }

    gettimeofday(&end_time,NULL);
    

    double *time = (double * ) malloc(sizeof(time));

    *time = ((end_time.tv_sec - start_time.tv_sec) * 1000000) + (end_time.tv_usec - start_time.tv_usec);
    pthread_exit((void * ) time);
}

void * compose_numbers(void *arg){
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time,NULL);
    
    int thread_num = *(int*) arg ;

    for(int i =0;i<row_num;i++){
        for(int j = 0;j<col_num;j++){
            if(thread_num * (MAX_SCALE/no_threads) <= img[i][j] && img[i][j] <= thread_num+1 * (MAX_SCALE/no_threads)){
                img[i][j] = MAX_SCALE - img[i][j];
            }
                
        }
    }
    
    
    gettimeofday(&end_time,NULL);
    

    double *time = (double * ) malloc(sizeof(time));

    *time = ((end_time.tv_sec - start_time.tv_sec) * 1000000) + (end_time.tv_usec - start_time.tv_usec);
    pthread_exit((void * ) time);
    
}

int main(int argc, char *argv []){

    if(argc < 5 ){
        perror("Wrong argument number");
        return -1;
    }

    no_threads = atoi(argv[1]);
    FILE *in_file = fopen(argv[3],"r");
    FILE *out_file = fopen(argv[4],"w+");

    if(in_file == NULL){
        perror("Couldn't open in file");
        return -1;
    }

    if(out_file == NULL){
        perror("Couldn't open out file");
        return -1;
    }


    //skip first line
    char buff[100];
    fgets(buff,sizeof(buff),in_file);

    if(fscanf(in_file,"%d %d\n",&col_num,&row_num) != 2){
        perror("Error reading img size");
        return -1;
    }

    //skip third line
    fgets(buff,sizeof(buff),in_file);
    

    //creating img that threads will work on
    img =  malloc(sizeof(int*) *row_num);
    for(int i = 0;i<row_num;i++){
        img[i] = malloc(sizeof(int)* col_num);
    }

    read_img(in_file);

    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time,NULL);
    
    pthread_t *threads = malloc(sizeof(pthread_t) * no_threads);
    

    if(strcmp(argv[2],"block") == 0){
        for(int i =0;i<no_threads;i++){

            //need to create new int to represent thread num so it doesnt get updated 
            int *thread_num = malloc(sizeof(int*));
            *thread_num = i;
            
            if(pthread_create(&threads[i],NULL,compose_block,thread_num) != 0){
                perror("Error creating thread");
                return -1;
            }
        }
    }

    else if(strcmp(argv[2],"numbers") == 0){
        for(int i =0;i<no_threads;i++){

            //need to create new int to represent thread num so it doesnt get updated 
            int *thread_num = malloc(sizeof(int*));
            *thread_num = i;
            
            if(pthread_create(&threads[i],NULL,compose_numbers,thread_num) != 0){
                perror("Error creating thread");
                return -1;
            }
        }
    }

    else{
        perror("Invalid second argument");
        return -1;
    }

     
    for(int i =0;i<no_threads;i++){
        double *time;
        pthread_join(threads[i],(void * ) &time);
        printf("Thread %d took: %lf microseconds\n",i+1,*time);
    } 

    
    gettimeofday(&end_time,NULL);
    double total = ((end_time.tv_sec - start_time.tv_sec) * 1000000) + (end_time.tv_usec - start_time.tv_usec);
    
    printf("Total time: %lf microseconds \n\n",total);

    save_img(out_file);

    return 0;
}