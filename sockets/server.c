#include "data.h"

#define MAX_PLAYERS 10
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int epoll;
player players[MAX_PLAYERS]; 
player *to_pair = NULL;

void remove_player(player* player) {
  if (player == to_pair) {
    to_pair = NULL;
  }
  if (player->connection) {
    player->connection->connection = NULL;
    player->connection->game = NULL;
    remove_player(player->connection);
    free(player->game);
    player->connection = NULL;
    player->game = NULL;
  }
  player->status = UNINIT;
  player->name[0] = 0;
  epoll_ctl(epoll, EPOLL_CTL_DEL, player->socket, NULL);
  close(player->socket);
}

void send_state(player* player) {
  message msg;
  msg.type = STATUS;
  memcpy(&msg.state, player->game, sizeof (struct game));
  strcpy(msg.state.board,player->game->board);
  msg.state.move = player->game->move;
  write(player->socket, &msg, sizeof msg);
}

bool check_game(player* player) {
  
  char* board = player->game->board;
  char c = player->symbol;
  if(board[0] == c && board[1] == c && board[2] == c) return true;
  if(board[3] == c && board[4] == c && board[5] == c) return true;
  if(board[6] == c && board[7] == c && board[8] == c) return true;
  if(board[0] == c && board[3] == c && board[6] == c) return true;
  if(board[1] == c && board[4] == c && board[7] == c) return true;
  if(board[2] == c && board[5] == c && board[8] == c) return true;
  if(board[0] == c && board[4] == c && board[8] == c) return true;
  if(board[2] == c && board[4] == c && board[6] == c) return true;

  return false;
}

bool check_draw(player* player) {
  for (int i = 0; i < 9; i++)
    if (player->game->board[i] == '-') return false;
  return true;
}

void join_players(player* player1, player* player2) {
  player1->status = PLAYING;
  player2->status = PLAYING;
  player1->connection = player2;
  player2->connection = player1;

  player1->game = player2->game = malloc(sizeof player1->game);
  for(int i=0;i<9;i++) player1->game->board[i] = '-';
  
  message msg;
  msg.type = PLAY;
  strcpy(msg.name, player2->name);
  player1->symbol = 'x';
  msg.symbol = 'x';
  write(player1->socket, &msg, sizeof msg);

  strcpy(msg.name, player1->name);
  player2->symbol = 'o';
  msg.symbol = 'o';
  write(player2->socket, &msg, sizeof msg);

  player1->game->move = player1->symbol;
  send_state(player1);
  send_state(player2);
}

void action(player* player) {
  if (player->status == INIT) {
    pthread_mutex_lock(&mutex);
      player->name[read(player->socket, player->name, sizeof player->name - 1)] = '\0';
      if (to_pair) {
        if (rand() % 2 == 0) join_players(player, to_pair);
        else join_players(to_pair, player);
        to_pair = NULL;
      } else {
        message msg;
        msg.type = WAIT;
        write(player->socket, &msg, sizeof msg);
        to_pair = player;
        player->status = WAITING;
      }
    pthread_mutex_unlock(&mutex);
  }
  else {
    message msg;
    read(player->socket, &msg, sizeof msg);
    if (msg.type == PING) {
      pthread_mutex_lock(&mutex);
      player->active = true;
      pthread_mutex_unlock(&mutex);
    } else if (msg.type == DISCONNECT) {
      pthread_mutex_lock(&mutex);
      remove_player(player);
      pthread_mutex_unlock(&mutex);
    } else if (msg.type == MOVE) {
      int move = msg.move;
      if (player->game->move == player->symbol 
        && player->game->board[move] == '-'
        && 0 <= move && move <= 8) {
        player->game->board[move] = player->symbol;
        player->game->move = player->connection->symbol;
        send_state(player);
        send_state(player->connection);
        if (check_game(player)) {
          msg.type = WIN;
          msg.win = player->symbol;
        }
        else if (check_draw(player)) {
          msg.type = WIN;
          msg.win = '-';
        }
        if (msg.type == WIN) {
          player->connection->connection = NULL;
          write(player->connection->socket, &msg, sizeof msg);
          write(player->socket, &msg, sizeof msg);
        }
      } 
      else send_state(player);
    }
  }
}

void init_socket(int socket, void* addr, int addr_size) {
  if(bind(socket, (struct sockaddr*) addr, addr_size) == -1){
      perror("Error binding socket");
      exit(1);
  }

  if(listen(socket, MAX_PLAYERS) == -1){
      perror("Error listening on socket");
      exit(1);
  }
  struct epoll_event event = { .events = EPOLLIN | EPOLLPRI };
  msg_data* data = event.data.ptr = malloc(sizeof *data);
  data->type = SOCKET;
  data->socket = socket;
  epoll_ctl(epoll, EPOLL_CTL_ADD, socket, &event);
}

player* new_player(int player_socket) {
  pthread_mutex_lock(&mutex);
  int i;
  for(i = 0; i < MAX_PLAYERS;i++){
      if(players[i].status == UNINIT) {
          break;
      }
  }

  players[i].socket = player_socket;
  players[i].status = INIT;
  players[i].active = true;
  pthread_mutex_unlock(&mutex);
  return &players[i];
}

void* ping(void* arg) {
  static message msg;
  msg.type = PING;
  while(1) {
    sleep(10);
    pthread_mutex_lock(&mutex);
    puts("PINGING");
    for (int i = 0; i < MAX_PLAYERS; i++) {
      if (players[i].status != UNINIT) {
        if (players[i].active) {
          players[i].active = false;
          write(players[i].socket, &msg, sizeof msg);
        }
        else remove_player(&players[i]);
      }
    }
    pthread_mutex_unlock(&mutex);
  }
}

int main(int argc, char** argv) {
  if (argc != 3) {
    perror("Wrong number of arguments");
    exit(0);
  }

  int port = atoi(argv[1]);
  char* path = argv[2];

  if((epoll = epoll_create1(0)) == -1){
      perror("Error creating epoll");
      return -1;
  }

  struct sockaddr_un local_address;
  local_address.sun_family = AF_UNIX;
  strcpy(local_address.sun_path, path);

  struct sockaddr_in network_addr;
  network_addr.sin_family = AF_INET;
  network_addr.sin_port = htons(port);
  network_addr.sin_addr.s_addr = INADDR_ANY;
  unlink(path);
  int local_socket;
   if((local_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
       perror("Error creating socket");
       return -1;
   }

  init_socket(local_socket, &local_address, sizeof local_address);

  int network_socket;
  if((network_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
      perror("Error creating socket");
      return -1;
  }

  init_socket(network_socket, &network_addr, sizeof network_addr);

  pthread_t ping_thread;
  pthread_create(&ping_thread, NULL, ping, NULL);

  puts("SERVER LISTENING");

  struct epoll_event events[10];
  while(1){
    int nread;
    if((nread = epoll_wait(epoll, events, 10, -1)) == -1){
        perror("Error on epoll wait");
        return -1;
    }
    for(int i =0;i<nread;i++){
      msg_data* data = events[i].data.ptr;
      if (data->type == SOCKET) { 
        int player_socket = accept(data->socket, NULL, NULL);
        player* player = new_player(player_socket);
    
        msg_data* data = malloc(sizeof(msg_data));
        data->type = PLAYER;
        data->player = player;
        struct epoll_event event = { .events = EPOLLIN | EPOLLET | EPOLLHUP, .data = { data } };

        if(epoll_ctl(epoll, EPOLL_CTL_ADD, player_socket, &event) == -1 ){
            perror("Error on epoll_ctl");
            return -1;
        }
      } else if (data->type == PLAYER) {
        if (events[i].events & EPOLLHUP) {
          pthread_mutex_lock(&mutex);
          remove_player(data->player);
          pthread_mutex_unlock(&mutex);
        }
        else action(data->player);
      }
    }
  }
}