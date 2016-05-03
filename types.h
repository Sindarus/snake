/**
* \file types.h
*/

#ifndef H_TYPES
#define H_TYPES

#include <stdbool.h>    //for 'bool' type


// STRUCTURES ==========================================================
/**
* \typedef coord
* \brief struct Represents a couple of coordinates.
*/
typedef struct coord {
    int x;  /**< x coordinate */
    int y;  /**< y coordinate */
} coord;

/**
* \typedef direction
* \brief Allows to use the four main directions.
*/
typedef enum {UP, DOWN, LEFT, RIGHT} direction;

/**
* \typedef square
* \brief Gathers the possible content of a square of the field.
*/
typedef enum {EMPTY, WALL, SNAKE, SCHLANGA, FOOD, POPWALL, HIGHSPEED, LOWSPEED, FREEZE} square;

/**
* \typedef t_type
* \brief type of a snake, can be either a snake or a schlanga.
*/
typedef enum {T_SNAKE=2, T_SCHLANGA=3} t_type;

/**
* \typedef snake
* \brief Represents a snake
* \details 'body' is an array of 'coord'.
*          'head' holds the index of the coordinates of the head in 'body'
*          'tail' holds the index of the coordinates of the tail in 'body'
*          'dir' is the direction the snake is currently moving.
*/
typedef struct snake {
    t_type type;    /**< type of snake, can be 'SCHLANGA' or 'SNAKE' */
    int size;       /**< size of snake */
    coord* body;    /**< array containing the coords of every part of the snake*/
    int head;       /**< current index of the head in the body array */
    direction dir;  /**< current direction the snake is faceing */
} snake;

/**
* \typedef field
* \brief Represents the arena on which the game is played
*/
typedef struct field {
    square** f;     		/**< 2D array representing the field*/
    int width;     			/**< width of the field */
    int height;     		/**< height of the field */
    int speed;				/**< basic speed of game */
    int freeze_snake;		/**< freezing-time left for snake */
    int freeze_schlanga;	/**< freezing-time left for schlanga */
} field;

// PROTOTYPES ==========================================================
// Constructors ========================================================
coord new_coord(int x, int y);
coord new_coord_empty();
field* new_field();
snake* new_snake(t_type type, int size, coord start_pos, field* map);

// Destructors =========================================================
void free_snake(snake* s);
void free_field(field* map);
void free_all(field* map, snake* s1, snake* s2);

// Objects managment ===================================================
bool are_equal(coord c1, coord c2);
direction opposite(direction d);
direction turn_left(direction d);
direction turn_right(direction d);
square get_square_at(field* map, coord c);
void set_square_at(field* map, coord c, square stuff);
int get_tail(snake* s);
coord get_head_coord(snake* s);
coord get_tail_coord(snake* s);
coord coord_after_dir(coord c, direction dir);

#endif
