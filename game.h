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

#define clear() printf("\e[1;1H\e[2J") // Clear screen

// PROTOTYPES ==========================================================
// Game ================================================================
void play(int size);
int move(snake* s, direction d, field* map);

// Input/Output ========================================================
int kbhit(void);
bool key_is_dir(char c);
direction key_to_dir(char c);

// Display =============================================================
void print_to_pos(coord pos, char c);
void mode_raw(int activate);
void normal_console();
void print_msg(int msg);
//void display(field* map, snake* s1, snake* s2);

#endif
