/**
* \file game.h
*/

#ifndef H_GAME
#define H_GAME

#include "types.h"

// CONSTANTS ============================================================
#define C_UP 122        /**< id number of key to move up. 122 = z      */
#define C_DOWN 115      /**< number of key to move down. 115 = s    */
#define C_LEFT 113      /**< number of key to move left. 113 = q    */
#define C_RIGHT 100     /**< number of key to move right. 100 = d   */
#define C_QUIT 27       /**< number of key to quit. 27 = esc      */

#define MIN_WINDOW_WIDTH 15
#define MIN_WINDOW_HEIGHT 15

#define TIME_STEP 150   /**< time between two time steps. In msec. */

#define MSG_LOOSE 1
#define MSG_WIN 2
#define MSG_DRAW 3

#define NB_ITEMS 5 /**< number of items in game */
#define FREEZING_TIME 10 /**< number of iterations during which a snake will be freezed */
#define ADD_SPEED 25000 /**< add x seconds to usleep */
#define MAX_PLAYERS 10
#define MAX_INPUT_STACK 10 /**< maximum number of valid move-input that can stack */

#define RED     "\033[1m\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[1m\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define RESET "\033[0m"

#define clear() printf("\e[1;1H\e[2J") // Clear screen

// STRUCTURES ==========================================================
typedef struct config{
    int size;           //snake size
    int AI_version;
    int players[MAX_PLAYERS];   //players[i] = 1 if it is a hooman, 2 if schlanga, 0 if not player
} config;

// PROTOTYPES ==========================================================
config new_config();

// Game ================================================================
void play(config cfg);
int move(snake* s, direction d, field* map);
void pop_item(field* map);

// Input/Output ========================================================
int kbhit(void);
bool key_is_dir(char c);
direction key_to_dir(char c);

// Display =============================================================
void print_to_pos(coord pos, char c);
void print_to_pos_colored(coord pos, char c, char* color);
void mode_raw(int activate);
void print_msg(int msg);
//void display(field* map, snake* s1, snake* s2);

#endif
