#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#define SOCKET 0
#define PLAYER 1

#define UNINIT 0
#define INIT 1
#define WAITING 2
#define PLAYING 3

#define DISCONNECT 0
#define PLAY 1
#define MOVE 2 
#define STATUS 3
#define WIN 4 
#define PING 5
#define FULL 6 
#define WAIT 7


struct game { 
  char move; 
  char board[9]; 
  };

typedef struct message {
  int type;
  char name[16]; 
  char symbol; 
  int move;
  struct game state;
  char win;
} message;

typedef struct player{
  int socket;
  int status;
  struct player* connection;
  char name[16];
  char symbol;
  struct game* game;
  bool active;
}player;

typedef struct msg_data {
  int type;
  player* player; 
  int socket;
} msg_data;

struct status {
  int score[2];
  char* names[2];
  char* symbols[2];
  char symbol;
  struct game game;
} status;

