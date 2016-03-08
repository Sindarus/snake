/*
    \file game.c
    \brief Functions related to the game
    \details This file is separated in 3 parts :
             1 - functions related to the game per se
             2 - functions related to input/output management
             3 - functions related to the display
*/

#include <stdbool.h>        //for 'bool' type
#include <stdio.h>          //for 'printf()'
#include <stdlib.h>         //for 'exit()'
#include <termios.h>        //for 'struct termios'
#include <unistd.h>         //for 'usleep()'

#include "types.h"
#include "AI.h"

#include "game.h"

// Game ================================================================
/**
* \fn void play(int size);
* \brief Function that launches a game and makes it run.
* \param size size of the snakes
* \details This function cares for the creation of the snakes and field,
*          the passing of the time, the collision management and so on.
*/
void play(int size) {
    clear(); // Clear screen
    printf("\e[?25l"); // Hide cursor

    //creating field
    field* map = new_field();
    if (size > map->height/2) {
        free_field(map);
        clear();
        printf("Please increase your window size or decrease your snake size\n");
        exit(1);
    }

    //creating snakes
    coord start_pos = new_coord(map->height/2, map->width/5); // Starting position of snake depending on size of the window
    coord start_pos2 = new_coord(map->height/2, 4*map->width/5); // Starting position of snake depending on size of the window
    snake* s = new_snake(size, start_pos, map); // Create snake with size 10 at start_pos on map
    snake* schlanga = new_snake(size, start_pos2, map); // Create snake with size 10 at start_pos on map
    
    char c;         // key that is pressed
    direction cur_dir;

    mode_raw(1); // Disable display of user input

    // Main loop
    while(1){
        usleep(TIME_STEP * 1000);

        // Input/Output management, choosing snake's direction
        if (kbhit()) {          // Check if user hits keyboard
            c = getchar();      // Get input

            if(c == C_QUIT){                    //if user pushed quit button
                normal_console();
                free_all(map, s, schlanga);
                return;
            }
        
            if(key_is_dir(c)) {                 //if user pushed a direction
                cur_dir = key_to_dir(c);        //retrieves the direction

                //if the key pressed is the opposite direction of the snake
                if(cur_dir == opposite(s->dir)){
                    cur_dir = s->dir;
                }
            }
        }
        else {                  //if user hasn't hit the keyboard
            cur_dir = s->dir;
        }

        //move snake
        if(move(s, cur_dir, map)){
            free_all(map, s, schlanga);
            normal_console();
            print_msg(MSG_LOOSE);
            return;
        }
        
        // choose schlanga direction
        cur_dir = rngesus2(schlanga, map);

        //move schlanga
        if(move(schlanga, cur_dir, map)){
            free_all(map, s, schlanga);
            normal_console();
            print_msg(MSG_WIN);
            return;
        }
        
        fflush(stdout);
    }//end while(1)
}

/**
* \fn int move(snake* s, direction d, field* map);
* \brief operates on a snake structure to make it move one step with the 'd'
*        direction. This function does not protect the snake from going into its neck.
*        This function also cares for collision management.
* \return Number corresponding to an event : 0 if snake/schlanga moves peacefully
*                                            1 if snake/schlanga dies
*/
int move(snake* s, direction d, field* map) {
    //MOVING SNAKE
    //Will hold the old coordinates of the head and tail.
    coord c_head = get_head_coord(s);
    coord c_tail = get_tail_coord(s);
    
    s->head = get_tail(s); // Index of head becomes index of old tail.
                       // We then replace the coordinates of the old tail
                       // with the coordinates of the new head

    // Updating snake's head coordinates
    s->dir = d;
    if (d == UP) {
        s->body[s->head].x = c_head.x-1;
        s->body[s->head].y = c_head.y;
    } else if (d == LEFT) {
        s->body[s->head].x = c_head.x;
        s->body[s->head].y = c_head.y-1;
    } else if (d == RIGHT) {
        s->body[s->head].x = c_head.x;
        s->body[s->head].y = c_head.y+1;
    } else if (d == DOWN) {
        s->body[s->head].x = c_head.x+1;
        s->body[s->head].y = c_head.y;
    }

    //DISPLAY
    print_to_pos(c_tail, ' ');
    print_to_pos(s->body[s->head], 's');
    
    //COLLISIONS
    if (get_square_at(map, get_head_coord(s)) != EMPTY) {
        return 1;
    }
    
    //UPDATE FIELD
    set_square_at(map, get_head_coord(s), SNAKE);
    set_square_at(map, get_tail_coord(s), EMPTY);

    return 0;
}

// Input/Output ========================================================
/**
* \fn int kbhit(void);
* \brief Emulates kbhit() function on Windows which detects keyboard input
*/
int kbhit(void) { 
    struct timeval tv = { 0, 0 }; 
    fd_set readfds; 
  
    FD_ZERO(&readfds); 
    FD_SET(STDIN_FILENO, &readfds); 
  
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1; 
}

/**
* \fn bool key_is_dir(char c);
* \return 1 if the given char 'c' corresponds to a direction key. 0 otherwise.
*/
bool key_is_dir(char c){
    return (c == C_UP || c == C_DOWN || c == C_LEFT || c == C_RIGHT);
}

/**
* \fn direction key_to_dir(char c);
* \return If the given char 'c' correspond to a direction, this function returns
*         the direction.
*/
direction key_to_dir(char c){
    switch(c){
        case C_UP:
            return UP;
            break;
        case C_DOWN:
            return DOWN;
            break;
        case C_LEFT:
            return LEFT;
            break;
        case C_RIGHT:
            return RIGHT;
            break;
        default:
            printf("in key_to_dir() : given char is not a direction");
            exit(1);
            break;
    }
}

// Display =============================================================
/**
* \fn void print_to_pos(coord pos, char c);
* \brief prints the character 'c' at the given position
* \details this functions sets the cursor to [x,y] pos and
*          prints the 'c' param
*/
void print_to_pos(coord pos, char c) {
    printf("\033[%d;%dH%c", pos.x, pos.y, c);
}

/**
* \fn void mode_raw(int activate);
* \brief Use mode_raw(1) to disable displaying user input.
*/
void mode_raw(int activate) 
{ 
    static struct termios cooked; 
    static int raw_actif = 0; 
  
    if (raw_actif == activate) 
        return; 
  
    if (activate) 
    { 
        struct termios raw; 
  
        tcgetattr(STDIN_FILENO, &cooked); 
  
        raw = cooked; 
        cfmakeraw(&raw); 
        tcsetattr(STDIN_FILENO, TCSANOW, &raw); 
    } 
    else 
        tcsetattr(STDIN_FILENO, TCSANOW, &cooked);
  
    raw_actif = activate; 
}

/**
* \fn void normal_console()
* \brief Gets the console back to normal : user input is displayed and the cursor
*        is back.
*/
void normal_console(){
    mode_raw(0);        //activates displaying user input
    clear();
    printf("\e[?25h");  //unhide cursor
}

/**
* \fn void print_msg(int msg);
* \brief prints a specific message onto the screen.
* \param msg Can be one of thoses : MSG_LOOSE, MSG_WIN or MSG_DRAW
* \details Warning : console has to be working in the usual way for theses messages
*          to be printed correctly. Use 'normal_console()' to get the console back
*          back to normal.
*/
void print_msg(int msg){
    switch(msg){
        case MSG_LOOSE:
            printf("----------------------------\n");
            printf("|        GAME_OVER         |\n");
            printf("----------------------------\n");
            break;
        case MSG_WIN:
            printf("----------------------------\n");
            printf("|          YOU_WIN         |\n");
            printf("----------------------------\n");
            break;
        case MSG_DRAW:
            printf("----------------------------\n");
            printf("|        IT'S A DRAW       |\n");
            printf("----------------------------\n");
            break;
        default:
            break;
    }
}
