#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct Command{
    char * name;
    char **programs;
    int no_programs;
}Command;

char* strip_white_space(char *str){

    while(isspace((unsigned char) *str )) str++;

    if(*str){

        char *end = str + strlen(str) - 1;

        while(isspace((unsigned char) *end)) end--;

        end[1] = '\0';
    }   

    return str;
}


char** parse_arguments(char * arguments){
    char **res = NULL;
    char *arg = strtok(arguments," ");
    int n = 0;

    while(arg){
        res = realloc(res,sizeof(char*)*++n);
        res[n-1] = arg;
        arg = strtok(NULL," ");
    }

    return res;
}

void execute(char **programs,int no_programs){
    
    int fd[no_programs][2];
    for(int i =0;i<no_programs;i++)pipe(fd[i]);

    for(int i = 0;i<no_programs;i++){
        
        pid_t pid = fork();

        if(pid == 0){
            if(i > 0) dup2(fd[i-1][0],STDIN_FILENO);
            if(i < no_programs -1) dup2(fd[i][1],STDOUT_FILENO);

            for(int j = 0;j < no_programs;j++) close(fd[j][1]);
            
            char **arguments = parse_arguments(programs[i]);

            execvp(arguments[0],arguments);
        }
    }

    for(int i = 0;i < no_programs;i++) close(fd[i][1]);

    for(int i =0;i<no_programs;i++) wait(NULL);

}

void free_commands_memory(Command *commands,int no_commands){
    for(int i =0;i<no_commands;i++){
        free(commands[i].name);
        for(int j=0;j<commands[i].no_programs;j++){
            free(commands[i].programs[j]);
        }
    }
    
}


int main(int argc, char*argv[]){

    if(argc < 2){
        perror("WRONG NUMBER OF ARGUMENTS");
        return -1;
    }

    char* filename = argv[1];
    FILE *file = fopen(filename,"r");
    
    if(file == NULL){
        perror("Couldnt open a file");
        return -1;
    }

    Command *commands = NULL;
    int no_commands = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&line,&len,file)) != -1){
        
        if(strchr(line,'=') != NULL){
            no_commands++;

            commands = realloc(commands,sizeof(Command) * no_commands);

            char *name = strtok(line,"=");       
            commands[no_commands-1].name = malloc(sizeof(name));
            name = strip_white_space(name);
            strcpy(commands[no_commands-1].name,name);

            int no_programs = 0;
            char * argument_line;
            char **programs = NULL;

            while((argument_line = strtok(NULL,"|"))!=NULL) {
                no_programs++;

                programs = realloc(programs,sizeof(char*) * no_programs);
                argument_line = strip_white_space(argument_line);
                programs[no_programs-1] = malloc(sizeof(argument_line));
                strcpy(programs[no_programs-1],argument_line);

                }

            commands[no_commands-1].no_programs = no_programs;
            commands[no_commands-1].programs = programs;
            
        }
    }


    fseek(file,0,SEEK_SET);

    while((read = getline(&line,&len,file)) != -1){
        if(strchr(line,'=') == NULL){
            char **to_exec = NULL;
            int no_to_exec = 0;
            char * name = strtok(line,"|");

            while(name != NULL){
                name = strip_white_space(name);

                for(int i =0;i<no_commands;i++){
                    if(strcmp(name,commands[i].name) == 0){
                        for(int j=0;j<commands[i].no_programs;j++){
                            no_to_exec++;

                            to_exec = realloc(to_exec,no_to_exec *sizeof(char*));
                            to_exec[no_to_exec -1] = malloc(sizeof(commands[i].programs[j]));
                            strcpy(to_exec[no_to_exec-1],commands[i].programs[j]);
                        }
                    }
                }
                name = strtok(NULL,"|");
            }   
            execute(to_exec,no_to_exec);
            free(name);
            free(to_exec);
        }
        
    }
    fclose(file);
    free(line);
    free_commands_memory(commands,no_commands);
    free(commands);
}

