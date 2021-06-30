#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){


    if(argc != 2 && argc != 4){
        perror("WRONG NUMBER OF ARGUMENTS");
        return -1;
    }


    if(argc == 2){
        char *to_run;
        if(strcmp(argv[1],"nadawca") == 0) to_run = "mail -H | sort -k 3";
        if(strcmp(argv[1],"data") == 0) to_run = "mail -H ";

        FILE* mail_out = popen(to_run,"r");

        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        while((read = getline(&line,&len,mail_out)) != -1) printf("%s",line);

        pclose(mail_out);
    }
    if(argc == 4){
        char *receiver = argv[1];
        char *topic = argv[2];
        char *content = argv[3];
        char to_run[1024];

        sprintf(to_run,"echo %s | mail -s %s %s",content,topic,receiver);
        FILE* mail_out = popen(to_run,"r");
        pclose(mail_out);


    }







}