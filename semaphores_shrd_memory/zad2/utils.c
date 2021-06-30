#include "utils.h"

int get_segment(char * pathname){
    int segment_fd = shm_open(pathname,O_RDWR,0644);
    if(segment_fd == -1){
        perror("Error getting shared memeory");
        exit(1);
    }

    return segment_fd;
}

void remove_segment(char * pathname){
    if(shm_unlink(pathname) == -1){
        perror("FREE: Error unlinking shared memory");
        exit(1);
    }
}

