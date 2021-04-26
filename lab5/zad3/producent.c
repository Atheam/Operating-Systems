#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int read_n(FILE* from,FILE* to,int n){
    int cnt = 0;
    char c;
    while(fread(&c,sizeof(char),1,from) == 1 && cnt != n){
        if(c != '\n'){
        cnt++;
        fwrite(&c,sizeof(char),1,to);
        }
    }
    return cnt;
}


int main(int argc,char* argv[]){
    if(argc != 5){
        perror("WRONG NUMBER OF ARGUMENTS!");
        return -1;
    }    
    FILE* fd = fopen(argv[1],"w");
    if(fd == NULL){
        perror("Producent couldnt open FIFO");
        return -1;
    }
    int row_num = atoi(argv[2]);
    FILE* txt_file = fopen(argv[3],"r");
    if(txt_file == NULL){
        
        perror("Producent couldnt open txt file");
        return -1;

    }

    int N = atoi(argv[4]);
    
    do{
        fwrite(&row_num,sizeof(row_num),1,fd);
    }
    while(read_n(txt_file,fd,N) == N);

    
    
    fclose(fd);
    fclose(txt_file);
    

    return 0;
    



}