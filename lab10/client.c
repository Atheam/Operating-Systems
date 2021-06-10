#include "data.h"


int connect_local(char* path) {
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, path);

  int sock;
  if((sock =socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
    perror("Error creating socket");
    return -1;
  }
  if(connect(sock, (struct sockaddr*) &addr, sizeof addr) == -1){
    perror("Error connecting to socket");
    return -1;
  }

  return sock;
}

int connect_network(char* ipv4, int port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ipv4, &addr.sin_addr) <= 0) {
    puts("Invalid address\n");
    exit(0);
  }

  int sock;
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) == - 1){
    perror("Error creating socket");
    return -1;
  }
  if(connect(sock, (struct sockaddr*) &addr, sizeof addr) == -1){
    perror("Error connecting to scoket");
    return -1;
  }
   
  return sock;
}


int sock;


int main(int argc, char** argv) {
  if(argc != 4){
    perror("Wrong number of arguments");
  }

  if (strcmp(argv[2], "network") == 0){
    
    char * address = strtok(argv[3],":");
    char * port = strtok(NULL," ");
    sock = connect_network(address, atoi(port));

  } 
  else if (strcmp(argv[2], "local") == 0) sock = connect_local(argv[3]);
  
  char* name = malloc(strlen(argv[1]));
  strcpy(name,argv[1]);
  status.names[0] = malloc(strlen(argv[1]));
  strcpy(status.names[0],argv[1]);
  write(sock, name, strlen(name));
  
  int epoll_fd;
   if((epoll_fd = epoll_create1(0)) == -1){
     perror("Erorr creating epoll");
     return -1;
   }
  
  struct epoll_event stdin_event = { .events = EPOLLIN | EPOLLPRI, .data = { .fd = STDIN_FILENO } };
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_event);

  struct epoll_event socket_event = { .events = EPOLLIN | EPOLLPRI | EPOLLHUP,.data = { .fd = sock }};
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &socket_event);

  int c;
  struct epoll_event events[2];
  while(1){
    int nread ;
    if((nread = epoll_wait(epoll_fd, events, 2, 1)) == -1 ){
      perror("Error waiting on epoll");
      return -1;
    }
    for(int i =0;i<nread;i++) {
      if (events[i].data.fd == STDIN_FILENO) {
        scanf("%d",&c);
        
        c--;

        if (status.game.board[c] == '-') {
          
          message msg;
          msg.type = MOVE;
          msg.move = c;
          write(sock, &msg, sizeof msg);
        }
      } else {
        message msg;
        read(sock, &msg, sizeof msg);
        if (msg.type == WAIT) {
        } else if (msg.type == PLAY) {
          status.names[1] = msg.name;
          status.symbols[1] = msg.symbol == 'o' ? "x" : "o";
          status.symbol = msg.symbol;
          status.symbols[0] = msg.symbol == 'o' ? "o" : "x";
          status.score[0] = status.score[1] = 0;
        } 
        else if (events[i].events & EPOLLHUP) {
          exit(0);
        } else if (msg.type == PING) 
          write(sock, &msg, sizeof msg);
        else if (msg.type == STATUS) {
          memcpy(&status.game, &msg.state, sizeof status.game);
          printf("\n   %c │ %c │ %c  \n  ───┼───┼─── \n   %c │ %c │ %c \n  ───┼───┼───\n   %c │ %c │ %c \n\n",status.game.board[0] == '-' ? '1' : status.game.board[0]
                                                                                                             ,status.game.board[1] == '-' ? '2' : status.game.board[1]
                                                                                                             ,status.game.board[2] == '-' ? '3' : status.game.board[2]
                                                                                                             ,status.game.board[3] == '-' ? '4' : status.game.board[3]
                                                                                                             ,status.game.board[4] == '-' ? '5' : status.game.board[4]
                                                                                                             ,status.game.board[5] == '-' ? '6' : status.game.board[5]
                                                                                                             ,status.game.board[6] == '-' ? '7' : status.game.board[6]
                                                                                                             ,status.game.board[7] == '-' ? '8' : status.game.board[7]
                                                                                                             ,status.game.board[8] == '-' ? '9' : status.game.board[8]);
                                                                                                             
          printf("Your symbol is %s\n",status.symbols[0]);
          
        } else if (msg.type == WIN) {
          if (msg.win == status.symbol) puts("You won");
          else if (msg.win == '-') puts("It's a draw");
          else puts("You lost");
          close(sock);
          exit(0);
        }
      }
    }
  }
}