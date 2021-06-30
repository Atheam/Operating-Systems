#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#define MAXPATHLEN 1024
#define MAXPATTERNLEN 512

int START_PATH_LEN;

void search_dir(char*,char*,int);
int pattern_check(char*,char*);

int main(int argc, char* argv[]){
    if(argc < 4){
        perror("Wrong argument number");
        return -1;
    }

    char start_dir[MAXPATHLEN];
    char pattern[MAXPATTERNLEN];

    int n;
    
    strcpy(start_dir,argv[1]);
    strcpy(pattern,argv[2]);
    
    START_PATH_LEN = strlen(start_dir);

    n = atoi(argv[3]);

    search_dir(start_dir,pattern,n);

    return 0;
}

void search_dir(char* path,char* pattern,int n){
    char * CURR_PATH = strlen(path) > START_PATH_LEN + 1 ? path + START_PATH_LEN + 1  : ".";
    printf("Directory %s is searched by process with PID %d\n",CURR_PATH , (int)getpid());

    int process_count = 0;

    DIR* dir = opendir(path);
    if(dir == NULL) exit(0);

    struct dirent* dirent;
    dirent = readdir(dir);

    while(dirent != NULL){
        if(strcmp(dirent->d_name,".") == 0 || strcmp(dirent->d_name,"..") == 0 ){
            dirent = readdir(dir);
            continue;
        }

        if(dirent->d_type == DT_DIR && n > 0){
            process_count++;

            if(fork() == 0){
                char new_path[MAXPATHLEN];

                strcpy(new_path,path);
                strcat(new_path,"/");
                strcat(new_path,dirent->d_name);

                closedir(dir);

                search_dir(new_path,pattern,--n);
                exit(0);
            }
        }
        else if(dirent->d_type == DT_REG && strlen(dirent->d_name)>=4 && strcmp(dirent->d_name + strlen(dirent->d_name) - 4,".txt") == 0){
            char file_path[MAXPATHLEN];

            strcpy(file_path,path);
            strcat(file_path,"/");
            strcat(file_path,dirent->d_name);
            
            char * F_PATH  = file_path + START_PATH_LEN + 1;
            if(pattern_check(file_path,pattern) == 1) printf("Pattern: %s has been found in file: %s by process with PID: %d\n",pattern,F_PATH,(int)getpid());
        }
        dirent = readdir(dir);
    }
    for(int i = 0;i < process_count;i++){
        wait(NULL);
    }
}

int pattern_check(char* filename,char* pattern){
    
    FILE* file = fopen(filename,"r");
    if(file == NULL){
        return -1;
    }

    size_t len = 0;
    char* line = NULL;

    while(getline(&line,&len,file) != -1){
        if(strstr(line,pattern) != NULL) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    return 0;


}