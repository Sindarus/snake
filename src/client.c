#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //for 'struct sockaddr_in'
#include <stdlib.h>     //for 'exit()'
#include <unistd.h>     //for 'read()'
#include <pthread.h>
#include <sys/time.h>

#include "types.h"
#include "game.h"
#include "queue.h"

//#define SERV_ADDR "192.168.0.38"
#define SERV_ADDR "127.0.0.1"
#define PORT 3490
#define PING 10

#define WIDTH 40
#define HEIGHT 20

int sockfd;

int safe_quit(int return_value){
    mode_raw(0);
    close(sockfd);
    exit(return_value);
}

/**
* \fn int diff(struct timeval big_time, struct timeval small_time);
* \returns The time in ms that separates 'big_time' from 'small_time'
*/
int diff_time(struct timeval big_time, struct timeval small_time){
    int sec = big_time.tv_sec - small_time.tv_sec;
    int usec = big_time.tv_usec - small_time.tv_usec;
    return sec * 1000 + usec / 1000;
}

void free_all_client(field* map, snake** snakes, int size){
    free_field(map);
    int i;
    for(i = 0; i<size; i++){
        free_snake(snakes[i]);
    }
}

void play_client(config cfg) {
    //time of the last step that occured. in ticks.
    int ok;               //signal we're waiting for before begining
    char c;               //key that is pressed
    int ret;              //value returned by 'read(0)', 0 if no new key was pressed
    int ret_serv;
    queue p1_queue = new_queue(MAX_INPUT_STACK);    //queue used to stack player input
    direction cur_dir;
    direction* players_dir = malloc(cfg.nb_players*sizeof(direction));

    //let's wait for server's signal
    ret_serv = read(sockfd, &ok, 1*sizeof(int));
    if(ret_serv < 0){
        perror("handle_server read waiting for signal");
    }
    else if(ret_serv == 0){
        printf("Server on socket %i closed connection.\n", sockfd); clear(); safe_quit(1);
    }

    if(ok != 1){
        printf("Wrong signal recieved from server. Received %i.\n", ok); safe_quit(1);
    }
    printf("Signal received. Starting now\n");
    //sleep(3);

    //GO
    clear();
    mode_raw(1);

    //creating field
    field* map = new_field(WIDTH, HEIGHT);

    //creating snakes
    int i;
    snake** snakes = malloc(cfg.nb_players*sizeof(snake*));
    for(i = 0; i < cfg.nb_players; i++){
        snakes[i] = new_snake(T_SNAKE, cfg.size, i, map);
    }

    fflush(stdout);

    struct timeval last_step_time;
    gettimeofday(&last_step_time, NULL);
    struct timeval now;
    int elapsed_time;       //elapsed time
    while(1){
        //SUMMARY
        //1 - let's check if it's time to retrieve input
        //2 - let's retrieve and sort every input
        //3 - let's send the server our direction
        //4 - let's wait for server's directions
        //5 - let's make snakes move
        //6 - let's update last_step_time
        //--------------------------------------

        //1 - let's check if it's time to retrieve input
        gettimeofday(&now, NULL);
        elapsed_time = diff_time(now, last_step_time); //time elapsed since last round. in ms.
        if(elapsed_time > TIME_STEP - PING){
            //2 - let's retrieve and sort every input
            while((ret = read(0, &c, sizeof(char))) != 0){
                if(ret == -1){
                    perror("read in 'play()'"); safe_quit(1);
                }

                if(c == C_QUIT){
                    mode_raw(0);
                    clear();
                    free_queue(&p1_queue);
                    free_all_client(map, snakes, cfg.nb_players);
                    return;
                }
                else if(key_is_p1_dir(c)){
                    //if the key is a move key for player 1:
                    if(! queue_full(&p1_queue)){
                        enqueue(&p1_queue, key_to_dir(c));
                    }
                }
                else{
                    //key pressed was a useless key. Do nothing.
                }
            }

            //3 - let's send the server our direction
            cur_dir = (! queue_empty(&p1_queue)) ? dequeue(&p1_queue) : snakes[cfg.id]->dir;
            cur_dir = (cur_dir == opposite(snakes[cfg.id]->dir)) ? snakes[cfg.id]->dir : cur_dir;
            if(write(sockfd, &cur_dir, 1*sizeof(direction)) < 0){
                perror("handle_server write"); safe_quit(1);
            }
            switch(cur_dir){
                case 0:
                    write(2, "sent 0\n", 7*sizeof(char));
                    break;
                case 1:
                    write(2, "sent 1\n", 7*sizeof(char));
                    break;
                case 2:
                    write(2, "sent 2\n", 7*sizeof(char));
                    break;
                case 3:
                    write(2, "sent 3\n", 7*sizeof(char));
                    break;
            }

            //4 - let's wait for server's directions
            ret_serv = read(sockfd, players_dir, cfg.nb_players*sizeof(direction));
            if(ret_serv == -1){
                perror("handle_server read"); safe_quit(1);
            }
            else if(ret_serv == 0){
                clear(); printf("Server closed connection.\n"); safe_quit(1);
            }

            //5 - let's make snakes move
            for(i = 0; i<cfg.nb_players; i++){
                if(players_dir[i] != 4){       //if the player is not dead (dir = 4 => dead player)
                    move(snakes[i], players_dir[i], map);
                }
            }
            fflush(stdout);

            //6 - let's update last_step_time
            gettimeofday(&last_step_time, NULL);
        }
        else{
            //sleep 90% of the remaining time
            usleep( (((TIME_STEP - PING) - elapsed_time) * 0.9) * 1000);
        }
    }
}

int main(){
    int id;
    int size;
    int nb_players;
    struct sockaddr_in serv;
    int ret_serv;
    config cfg;

    printf("Creating socket...\n");
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket"); safe_quit(1);
    }
    printf("OK.\n");

    printf("Preparing serv address...\n");
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    inet_aton(SERV_ADDR, (struct in_addr*) &serv.sin_addr.s_addr);
    bzero(&(serv.sin_zero), 8);
    printf("Ok.\n");

    printf("Connecting to server...\n");
    if (connect(sockfd,(struct sockaddr *) &serv, sizeof(serv)) < 0) {;
        perror("connect"); safe_quit(1);
    }
    printf("Ok.\n");

    printf("Waiting for server to send info aout the game.\n");
    ret_serv = read(sockfd, &size, 1*sizeof(int));
    if(ret_serv == -1){ perror("read"); safe_quit(1); }
    if(ret_serv == 0){ clear(); printf("Server closed connection.\n"); safe_quit(1);}
    printf("Snakes will be size %i.\n", size);

    ret_serv = read(sockfd, &nb_players, 1*sizeof(int));
    if(ret_serv == -1){ perror("read"); safe_quit(1);}
    if(ret_serv == 0){ clear(); printf("server closed connection.\n"); safe_quit(1);}
    printf("There are %i players in the game.\n", nb_players);

    ret_serv = read(sockfd, &id, 1*sizeof(int));
    if(ret_serv == -1){ perror("read"); safe_quit(1); }
    if(ret_serv == 0){ clear(); printf("server closed connection.\n"); safe_quit(1);}
    printf("You have the id : %i.\n", id);

    printf("Waiting for the server to start the game.\n");

    cfg.size = size;
    cfg.nb_players = nb_players;
    cfg.id = id;

    play_client(cfg);

    return 0;
}