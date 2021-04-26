#include <stdio.h>
#include <stdlib.h>



void write_to_nth_row(FILE* file,int row_num,char* str,int N){
    char c;
    int line_num = 0;
    fseek(file,0,SEEK_END);
    size_t sz = ftell(file);
    fseek(file,0,SEEK_SET);

    while(fread(&c,sizeof(char),1,file) == 1){
        if(c == '\n') {
            line_num++;
            if(line_num == row_num){
                fseek(file,-1,SEEK_CUR);
                char buff[sz];
                size_t read = fread(buff,sizeof(char),sz,file);
                fseek(file,-read,SEEK_CUR);
                fwrite(str,sizeof(char),N,file);
                fwrite(buff,sizeof(char),read,file);
                break;
            }
            }
    }
    if(line_num < row_num){
        char c = '\n';
        for(int i =0;i<row_num-line_num-1;i++){
            fwrite(&c,sizeof(char),1,file);
        }
        fwrite(str,sizeof(char),N,file);
        fwrite(&c,sizeof(char),1,file);
    }


}



int main(int argc,char* argv[]){

    if(argc != 4){
        perror("WRONG NUMBER OF ARGUMENTS");
        return -1;
    }
    FILE* fd = fopen(argv[1],"r");
    if(fd == NULL){
        perror("Consumer couldnt open FIFO");
        return -1;
    }
    FILE* txt_file = fopen(argv[2],"wr+");
    if(txt_file == NULL){
        perror("Producent couldnt open txt file");
        return -1;
    }
    int N = atoi(argv[3]);
    
    char* buff = malloc(sizeof(char) * N);
    int row_num;
    while(fread(&row_num,sizeof(int),1,fd) == 1 && fread(buff,sizeof(char),N,fd) == N){
        write_to_nth_row(txt_file,row_num,buff,N);
    }

    free(buff);
    fclose(fd);
    fclose(txt_file);
    return 0;   
}