#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //for 'struct sockaddr_in'
#include <strings.h>    //for 'bzero()'
#include <unistd.h>     //for 'read()'
#include <sys/time.h>
#include <signal.h>

#include "game.h"

#define BACKLOG 10
#define SERV_ADDR "127.0.0.1"
#define PORT 3490
#define MAX_PLAYERS 10
#define SIZE 10         //size of the snake
#define PING 10

#define WIDTH 40    //size of the square arena
#define HEIGHT 20

int players[MAX_PLAYERS];
int players_dir[MAX_PLAYERS];
int sockfd;

/**
* \fn int diff(struct timeval big_time, struct timeval small_time);
* \returns The time in ms that separates 'big_time' from 'small_time'
*/
int diff_time(struct timeval big_time, struct timeval small_time){
    int sec = big_time.tv_sec - small_time.tv_sec;
    int usec = big_time.tv_usec - small_time.tv_usec;
    return sec * 1000 + usec / 1000;
}

int count_players(){
    int i;
    for(i = 0; i<MAX_PLAYERS; i++){
        if(players[i] == -1) return i;
    }
    return i;
}

void create_listen_socket(){
    printf("Creating socket...\n");
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    printf("Ok.\n");

    printf("Preparing server adress...\n");
    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    inet_aton(SERV_ADDR, (struct in_addr*) &my_addr.sin_addr.s_addr);
    bzero(&(my_addr.sin_zero), 8);
    printf("Ok.\n");

    printf("Binding adress to socket...\n");
    if(bind(sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr_in)) == -1){
        perror("bind");
        exit(1);
    }
    printf("Ok.\n");

    printf("Making socket a listen socket...\n");
    if(listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Ok.\n");
}

void read_client(int* id){
    direction cur_dir;

    while(1){
        if(read(players[*id], &cur_dir, 1*sizeof(direction)) == -1){
            perror("read on a client"); exit(1);
        }
        if(cur_dir >= 0 && cur_dir <=3 && players_dir[*id] != 4){
            //if player's move is valid and if he's not dead
            players_dir[*id] = cur_dir;
        }
    }

}

void accept_players(int* wait_players){
    int i = 0;
    int newfd;
    struct sockaddr their_addr;
    socklen_t stupid_var = sizeof(struct sockaddr_in);

    while(*wait_players && i < MAX_PLAYERS){
        if((newfd = accept(sockfd, &their_addr, &stupid_var)) == -1){
            perror("accept");
            exit(1);
        }
        players[i] = newfd;
        i++;
        printf("A new player joined the game. Connected players : %i.\n", i);
    }
}

void play_server(config cfg) {
    //creating field
    field* map = new_field(WIDTH, HEIGHT);

    //creating snakes
    int i;
    snake** snakes = malloc(cfg.nb_players*sizeof(snake*));
    for(i = 0; i < cfg.nb_players; i++){
        snakes[i] = new_snake(T_SNAKE, cfg.size, i, map);
    }

    struct timeval last_step_time;
    gettimeofday(&last_step_time, NULL);
    struct timeval now;
    int elapsed_time;       //elapsed time
    int c_dead = 0;         //count dead players
    while(1){
        //SUMMARY
        //1 - let's check if it's time to process inputs
        //2 - let's send everyone the directions
        //3 - let's make snakes move
        //4 - let's update last_step_time
        //5 - let's check if the game has to end
        //--------------------------------------

        //1 - let's check if it's time to retrieve input
        gettimeofday(&now, NULL);
        elapsed_time = diff_time(now, last_step_time); //time elapsed since last round. in ms.
        if(elapsed_time > TIME_STEP - (PING/2)){
            //2 - let's send everyone the directions
            for(i = 0; i<cfg.nb_players; i++){
                write(players[i], &players_dir, cfg.nb_players*sizeof(direction));
            }

            //3 - let's make snakes move
            for(i = 0; i<cfg.nb_players; i++){
                if(players_dir[i] != 4){       //if the player is not dead (dir = 4 => dead player)
                    if(move(snakes[i], players_dir[i], map)){
                        players_dir[i] = 4;
                        printf("Player %i died.\n", i);
                        c_dead++;
                    }
                }
            }

            //4 - let's update last_step_time
            gettimeofday(&last_step_time, NULL);

            //5 - let's check if the game has to end
            if(c_dead >= cfg.nb_players - 1){
                printf("Game has ended, only one player left alive.\n");
            }
        }
        else{
            //sleep 90% of the remaining time
            usleep( (((TIME_STEP - (PING/2)) - elapsed_time) * 0.9) * 1000);
        }
    }

    for(i = 0; i<cfg.nb_players; i++){
        free_snake(snakes[i]);
    }
    free_field(map);
}

void safe_quit(){
    int i;
    close(sockfd);
    for(i = 0; i<MAX_PLAYERS; i++){
        close(players[i]);
    }
}

int main(){
    signal(SIGINT, safe_quit);

    int size = SIZE;
    int i;
    pthread_t my_thread;                //accept_player thread
    pthread_t threads[MAX_PLAYERS];     //read_clients threads
    int wait_players = 1;
    int ok = 1;     //signal to send players
    int ret_cli;
    char c;

    int stupid_array[MAX_PLAYERS];
    for(i = 0; i<MAX_PLAYERS; i++){
        stupid_array[i] = i;
    }

    printf("This is a server for rngesus' snake vs schlanga console game.\n");

    //INITING 'PLAYERS'
    for(i = 0; i<MAX_PLAYERS; i++){
        players[i] = -1;
    }

    //CREATING LISTEN SOCKET
    create_listen_socket();

    //RECIEVING CONNECTIONS
    printf("The server is now open to connections.\nPress ctrl+D when everyone has joined.\n");
    if(pthread_create(&my_thread, 0, (void*) &accept_players, &wait_players) != 0){
        printf("error : could not create thread\n");
        exit(1);
    }

    int nb_players;
    while(1){
        while(read(0, &c, sizeof(char)) != 0){}
        nb_players = count_players();
        if(nb_players < 1){
            printf("No enough player to start a game.\n");
        }
        else{
            break;
        }
    }

    wait_players = 0;   //this will cause the accept_players thread to terminate
    printf("From now on, no new players will be accepted.\npress ctrl+D to start the game.\n");

    //SENDING INFO TO CLIENTS
    for(i = 0; i < nb_players; i++){
        write(players[i], &size,       1*sizeof(int));
        write(players[i], &nb_players, 1*sizeof(int));
        write(players[i], &i,          1*sizeof(int));
    }

    //LAUNCHING THREADS TO READ CLIENTS
    for(i = 0; i<nb_players; i++){
        if( pthread_create(&threads[i], 0, (void*) &read_client, &stupid_array[i]) != 0){
            printf("Couldn't create thread for client %i.\n", i);
        }
    }


    //WAITING TO START THE GAME
    while(read(0, &c, sizeof(char)) != 0){}

    printf("Sending signal to clients...\n");
    //SENDING SIGNAL TO CLIENTS
    for(i = 0; i < nb_players; i++){
        ret_cli = write(players[i], &ok, 1*sizeof(int));
        if(ret_cli < 0){
            perror("write sending signal"); exit(1);
        }
        else if(ret_cli == 0){
            printf("Client %i closed connection.\n", i); exit(1);
        }
    }
    printf("Ok.\n");

    config cfg;
    cfg.size = size;
    cfg.nb_players = nb_players;

    play_server(cfg);

    printf("Press ctrl+D to terminate server.\n");
    while(read(0, &c, sizeof(char)) != 0){}

    //ENDING
    safe_quit();

    return 0;
}