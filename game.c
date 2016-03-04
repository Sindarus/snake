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
    snake* s = new_snake(size, start_pos); // Create snake with size 10 at start_pos on map
    snake* schlanga = new_snake(size, start_pos2); // Create snake with size 10 at start_pos on map
    
    char c;         // key that is pressed
    direction cur_dir;

    mode_raw(1); // Disable display of user input
    //display(map, s, schlanga);

    // Main loop
    while(1){
        usleep(TIME_STEP * 1000);

        // Input/Output management
        if (kbhit()) {  // Check if user hits keyboard
            c = getchar(); // Get input

            if(c == C_QUIT){
                normal_console();
                free_all(map, s, schlanga);

                return;
            }
        
            // Move snake
            if(key_is_dir(c)) {
                cur_dir = key_to_dir(c);

                //if the key pressed is the opposite direction of the snake
                if(cur_dir == opposite(s->dir)){
                    move(s, s->dir);    //then keep going
                }
                else{
                    move(s, cur_dir);
                }
            }
        }
        else {
            move(s, s->dir);
        }
        
        // Move schlanga
        cur_dir = AI_random_v3(schlanga, map);
        move(schlanga, cur_dir);

        // Collisions
        switch(collisions(map, s, schlanga)){
            case 0:     //no collisions
                //display(map, s, schlanga);
                break;
            case SNAKE_DEAD:
                normal_console();
                print_msg(MSG_LOOSE);
                free_all(map, s, schlanga);
                return;
            case SCHLANGA_DEAD:
                normal_console();
                print_msg(MSG_WIN);
                free_all(map, s, schlanga);
                return;
            case BOTH_DEAD:
                normal_console();
                print_msg(MSG_DRAW);
                free_all(map, s, schlanga);
                return;
        }
        
        fflush(stdout);
    }//end while(1)
}

/**
* \fn void move(snake* s, direction d);
* \brief operates on a snake structure to make it move one step with the 'd'
*        direction. This function does not protect the snake from going into its neck.
*/
void move(snake* s, direction d) {
    //Will hold the old coordinates of the head and tail.
    coord c_head = s->body[s->head];
    coord c_tail = s->body[s->tail];
    
    s->head = s->tail; // Index of head becomes index of tail.
                       // We then replace the coordinates of the old tail
                       // with the coordinates of the new head
    s->tail = (s->head+1) % (s->size); // update index of tail

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

    // Display
    print_to_pos(c_tail, ' ');
    print_to_pos(s->body[s->head], 's');
    
    return;
}

/**
* \fn int collisions(field* map, snake* s1, snake* s2);
* \brief checks collision between objects in the field
* \details As a result of the fixing of the data redundancy there was when the position
*          of every snake part was also recorded in the 'field' variable, the collision
*          checking becomes a bit more complex. But also more precise : now, you can
*          know what killed a given snake.
* \returns 0 if no collisions. 'SNAKE_DEAD', 'SCHLANGA_DEAD', 'BOTH_DEAD' (constants)
*            in other cases.
*/
int collisions(field* map, snake* s1, snake* s2){
    int i;
    bool s1_dead = 0;
    bool s2_dead = 0;

    //snake1 collisions
        //collision with wall
    if (get_square_at(map, get_head_coord(s1)) == WALL){
        s1_dead = 1;
    }
        //collision with snake2
    for(i=0; i<s2->size; i++){
        if( are_equal(get_head_coord(s1), s2->body[i]) ){
            s1_dead = 1;
        }
    }
        //colision with self body
    for(i=0; i<s1->size; i++){
        if(i == s1->head) continue;
        if( are_equal(get_head_coord(s1), s1->body[i]) ){
            s1_dead = 1;
        }
    }

    //snake2 collisions
        //collision with wall
    if (get_square_at(map, get_head_coord(s2)) == WALL){
        s2_dead = 1;
    }
        //collision with snake1
    for(i=0; i<s1->size; i++){
        if( are_equal(get_head_coord(s2), s1->body[i]) ){
            s2_dead = 1;
        }
    }
        //colision with self body
    for(i=0; i<s2->size; i++){
        if(i == s2->head) continue;
        if( are_equal(get_head_coord(s2), s2->body[i]) ){
            s2_dead = 1;
        }
    }

    //returning
    if(s1_dead && s2_dead)  return BOTH_DEAD;
    else if(s2_dead)        return SCHLANGA_DEAD;
    else if(s1_dead)        return SNAKE_DEAD;
    else                    return 0;
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

/**
* \fn void display(field* map, snake* s1, snake* s2);
* \brief Displays the map and the snakes. The printing mechanism is too slow and
*        makes this function unusable.
*/
// void display(field* map, snake* s1, snake* s2){
//     int a, b;

//     //display field
//     for(a = 0; a < map->height; a++){
//         for(b = 0; b < map->width; b++){
//             switch(map->f[a][b]){
//                 case EMPTY:
//                     print_to_pos(new_coord(a, b), ' ');
//                     break;
//                 case WALL:
//                     print_to_pos(new_coord(a, b), '#');
//                     break;
//             }
//         }
//     }

//     //display first snake
//     for(a=0; a<s1->size; a++){
//         print_to_pos(s1->body[a], 's');
//     }

//     //display second snake
//     for(a=0; a<s2->size; a++){
//         print_to_pos(s2->body[a], 's');
//     }
// }
