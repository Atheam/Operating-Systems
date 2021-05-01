#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>


#define QUEUEID 100
#define $HOME getpwuid(getuid())->pw_dir
#define MAX_TEXT_LEN 1024

#define CLIENT_MAX 64

#define LIST 1
#define CONNECT 2
#define DISCONNECT 3
#define STOP 4
#define INIT 5
#define TEXT 6

#define COMMAND 20
#define WRITE 30


#define UNINITIALIZED 11
#define AVAILABLE 12
#define CONNECTED 13


struct info{
    int client_queue;
    int client_id;
    int pair_id;
    int pair_queue;
};

struct messege{
    long mtype;
    char mtext[MAX_TEXT_LEN];
    struct info minfo;
};

struct client{
    int client_id;
    int status;
    int queue;
};