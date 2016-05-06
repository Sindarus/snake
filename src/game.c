/**
* \file game.c
* \brief Functions related to the game
* \details This file is separated in 3 parts :
*          1 - functions related to the game per se
*          2 - functions related to input/output management
*          3 - functions related to the display
*/

#include <stdbool.h>        //for 'bool' type
#include <stdio.h>          //for 'printf()'
#include <stdlib.h>         //for 'exit()'
#include <termios.h>        //for 'struct termios'
#include <unistd.h>         //for 'usleep()'

#include "types.h"
#include "AI.h"

#include "game.h"

/**
* \fn config new_config();
* \brief Returns a newly created 'config' variable, with 'players[i]' = 0.
*/
config new_config(){
    config cfg;

    for(int i; i<MAX_PLAYERS; i++){
        cfg.players[i] = 0;
    }

    return cfg;
}

// Game ================================================================
/**
* \fn void play(config cfg);
* \brief Function that launches a game and makes it run.
* \param cfg a variable of type 'config' containing options for the game.
* \details This function cares for the creation of the snakes and field,
*          the passing of the time, the collision management and so on.
*/
void play(config cfg) {
    clear();
    //creating field
    field* map = new_field();
    if (cfg.size > map->height/2) {
        free_field(map);
        clear();
        printf("Please increase your window size or decrease your snake size\n");
        return;
    }

    //creating snakes
    coord start_pos = new_coord(map->height/2, map->width/5); // Starting position of snake depending on size of the window
    coord start_pos2 = new_coord(map->height/2, 4*map->width/5); // Starting position of snake depending on size of the window
    snake* s = new_snake(T_SNAKE, cfg.size, start_pos, map); // Create snake with size 10 at start_pos on map
    snake* schlanga = new_snake(T_SCHLANGA, cfg.size, start_pos2, map); // Create snake with size 10 at start_pos on map

    char c;    // key that is pressed
    int ret;   // value returned by 'read()', 0 if no new key was pressed
    direction stack[MAX_INPUT_STACK];    //stack of input
    direction cur_dir;

    int random_item; // Random integer deciding if an item pops or not
    mode_raw(1);

    // Main loop
    while(1){
        usleep(TIME_STEP * 1000 - map->speed);

        // Input/Output management, choosing snake's direction
        while( (ret = read(0, &c, sizeof(char))) != 0){  //while there is input to treat
            if(ret == -1){
                perror("read in 'play()'");
                exit(1);
            }
            if(c == C_QUIT){                    //if user pushed quit button
                mode_raw(0);
                clear();
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
            else{
                cur_dir = s->dir;
            }

            //if stack empty
            // else {                  //if user hasn't hit the keyboard
            //     cur_dir = s->dir;
            // }
        }

        //move snake
        if (map->freeze_snake > 0) {
			map->freeze_snake--;
		} else {
			if(move(s, cur_dir, map)){
				free_all(map, s, schlanga);
				mode_raw(0);
				clear();
				print_msg(MSG_LOOSE);
				return;
			}
		}

        // choose schlanga direction
        switch(cfg.AI_version){
            case 1:
                cur_dir = rngesus(schlanga);
                break;
            case 2:
                cur_dir = rngesus2(schlanga, map);
                break;
            case 3:
                cur_dir = spread(schlanga, map);
                break;
            case 4:
                cur_dir = aggro_dist(schlanga, map, s);
                break;
            default:
                free_all(map, s, schlanga); mode_raw(0); clear();
                printf("In 'move()' : AI_version not recognized.\n");
                exit(1);
        }

        //move schlanga
        if (map->freeze_schlanga > 0) {
			map->freeze_schlanga--;
		} else {
			if(move(schlanga, cur_dir, map)){
				free_all(map, s, schlanga);
				mode_raw(0);
				clear();
				print_msg(MSG_WIN);
				return;
			}
		}

		random_item = rand() % 10;
		if (random_item == 0) {
			pop_item(map);
		}
        fflush(stdout);
    }// end while(1)
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
    if(s->type == T_SNAKE){
        print_to_pos(s->body[s->head], 's');
    }
    if(s->type == T_SCHLANGA){
        print_to_pos(s->body[s->head], '$');
    }

    //COLLISIONS
    square temp_square = get_square_at(map, get_head_coord(s));
    //~ if (temp_square != EMPTY) {
        //~ return 1;
	int collision;
	int popwall;
	//~ coord pos_wall;
    switch(temp_square) {
		case WALL:
			//~ write(2, "hit a wall\n", 11*sizeof(char));
			collision = 1;
			break;
		case SCHLANGA:
			//~ write(2, "hit schlanga\n", 13*sizeof(char));
			collision = 1;
			break;
		case SNAKE:
			//~ write(2, "hit snake\n", 10*sizeof(char));
			collision = 1;
			break;
		case FOOD:
			collision = 0;
			break;
		case POPWALL:
			for (popwall = 0; popwall < 5; popwall++) {
				coord pos_wall = new_coord(1 + rand() % (map->height-1), 1 + rand() % (map->width-1));
				if (get_square_at(map, pos_wall) == EMPTY) {
					set_square_at(map, pos_wall, WALL);
					print_to_pos_colored(pos_wall, '#', RED);
				}
			}
			collision = 0;
			break;
		case HIGHSPEED:
			collision = 0;
			map->speed += ADD_SPEED;
			break;
		case LOWSPEED:
			collision = 0;
			map->speed -= ADD_SPEED;
			break;
		case FREEZE:
			collision = 0;
			if(s->type == T_SNAKE){
				map->freeze_schlanga = FREEZING_TIME;
			} else if(s->type == T_SCHLANGA){
				map->freeze_snake = FREEZING_TIME;
			}
			break;
		case EMPTY:
			//~ write(2, "hit ???\n", 7*sizeof(char));
			collision = 0;
			break;
    }

	if (collision == 0) {
		// UPDATE FIELD
		set_square_at(map, get_head_coord(s), s->type);
		set_square_at(map, c_tail, EMPTY);
		return 0;
	} else {
		return 1;
	}
}

// Items ===============================================================
/**
* \fn void pop_item(field* map);
* \brief adds a random item to the field.
*/
void pop_item(field* map) {
    coord pos_item;
    square item;
    char item_char;
    int dir = rand() % 4;

    do {
        pos_item = new_coord(1 + rand() % (map->height-1), 1 + rand() % (map->width-1));
    } while (get_square_at(map, pos_item) != EMPTY);

    switch (dir) {
        case 0:
            item = FREEZE;
            item_char = '*';
            break;
        case 1:
			if (map->speed >= 5*ADD_SPEED) {
				item = -1;
			} else {
				item = HIGHSPEED;
				item_char = 'Z';
			}
            break;
        case 2:
			if (map->speed <= -5*ADD_SPEED) {
				item = -1;
			} else {
				item = LOWSPEED;
				item_char = 'Q';
			}
            break;
        case 3:
			item = POPWALL;
			item_char = 'W';
            break;
        default:
            item = -1;
            break;
    }

    if (item > 0) {
        set_square_at(map, pos_item, item);
        print_to_pos(pos_item, item_char);
    }
}

// Input/Output ========================================================
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
    #ifndef DO_NOT_DISPLAY
    printf("\033[%d;%dH%c", pos.x, pos.y, c);
    #endif
}

/**
* \fn void print_to_pos(coord pos, char c, char* color);
* \brief prints the character 'c' at the given position in chosen color
* \details this functions sets the cursor to [x,y] pos and
*          prints the 'c' param with chosen color
*/
void print_to_pos_colored(coord pos, char c, char* color) {
    #ifndef DO_NOT_DISPLAY
    printf("%s \033[%d;%dH%c" RESET, color, pos.x, pos.y, c);
    #endif
}

/**
* \fn void mode_raw(int activate);
* \brief Use mode_raw(1) to enable raw mode, mode_raw(0) to disable.
*        See detailed description for more info.
* \details When you call 'mode_raw(1)', the terminal switches into raw mode, if it was
*          not already. Cursor is hidden. User input is hidden. User input is
*          readable with a call to 'read()' without the user having to press "enter".
*          The call to 'read()' is not blocking, and it will return 0 if there was
*          less characters to read in the buffer than the requested number.
*          When you call 'mode_raw(0)', STDIN will be flushed, the cursor will show
*          again, and the terminal will switch back to it's original state,
*          whatever it was.
*/
void mode_raw(int activate)
{
    //"static" variables are inited only once, and then shared between calls of this function.
    static bool first_run = true;       //1 if this is the first time this function is run
    static bool raw_activated = false;  //true if the raw mode is activated
    static struct termios term_save;    //configuration to apply to have the terminal back to its initial mode
    static struct termios term_raw;     //configuration to apply to have raw-mode

    char trash;     //we need this variable to flush STDIN

    //PREPARING NEEDED
    if(first_run){
        first_run = false;

        //in this block, we build 'term_save' and 'term_raw' once and for all
        //BUILDING 'term_save'
        tcgetattr(STDIN_FILENO, &term_save);    //save the current terminal config.

        //BUILDING 'term_raw'
        term_raw = term_save;

        //the line "flags &= ~flag" has the effect of removing 'flag' from 'flags'
        term_raw.c_lflag &= ~ICANON;        //set non-canonical mode (see termios(3))
        term_raw.c_lflag &= ~ECHO;          //do not echo user input !

        //make 'read()' a "polling read" (see termios(3))
        term_raw.c_cc[VMIN] = 0;
        term_raw.c_cc[VTIME] = 0;
    }

    //BULK OF THE FUNCTION :
    if(activate){   //the user wants to activate the raw mode
        if(raw_activated) return;   //if it's already activated, then do nothing

        tcsetattr(STDIN_FILENO, TCSANOW, &term_raw); //apply raw-mode configuration
        printf("\e[?25l"); // Hide cursor

        raw_activated = true;
    }
    else{           //the user wants to disactivate the raw mode
        if(!raw_activated) return;  //if it was not activated, then do nothing

        //flush STDIN
        while(read(0, &trash, sizeof(char)) != 0){}

        tcsetattr(STDIN_FILENO, TCSANOW, &term_save); //apply original configuration
        printf("\e[?25h");  //unhide cursor

        raw_activated = false;
    }
}

/**
* \fn void print_msg(int msg);
* \brief prints a specific message onto the screen.
* \param msg Can be one of thoses : MSG_LOOSE, MSG_WIN or MSG_DRAW
* \details Warning : console has to be working in the usual way for theses messages
*          to be printed correctly. Use 'mode_raw(0)' to get the console back
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
