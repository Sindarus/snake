#ifndef H_TYPES
#define H_TYPES

#include <stdbool.h>	//for 'bool' type


// STRUCTURES ==========================================================
/**
* \struct coord
* \brief struct Represents a couple of coordinates.
*/
typedef struct coord {
	int x;
	int y;
} coord;

/**
* \enum direction
* \brief Allows to use the four main directions.
*/
typedef enum {UP, DOWN, LEFT, RIGHT} direction;

/**
* \enum square
* \brief Gathers the possible content of a square of the field.
*/
typedef enum {EMPTY, WALL, SNAKE, SCHLANGA} square;

/**
* \sctruct snake
* \brief Represents a snake
* \details 'body' is an array of 'coord'.
* 		   'head' holds the index of the coordinates of the head in 'body'
*		   'tail' holds the index of the coordinates of the tail in 'body'
*		   'dir' is the direction the snake is currently moving.
*/
typedef struct snake {
	int size;
	int head;
	int tail;
	coord* body;
	direction dir;
} snake;

/**
* \sctruct field
* \brief Represents the arena on which the game is played
*/
typedef struct field {
	square** f;
	int width;
	int height;
} field;

// PROTOTYPES ==========================================================
// Constructors ========================================================
coord new_coord(int x, int y);
coord new_coord_empty();
snake* new_snake(int size, coord start_pos);
field* new_field();

// Destructors =========================================================
void free_snake(snake* s);
void free_field(field* map);
void free_all(field* map, snake* s1, snake* s2);

// Objects managment ===================================================
bool are_equal(coord c1, coord c2);
direction opposite(direction d);
square get_square_at(field* map, coord c);
square set_square_at(field* map, coord c, square stuff);
coord get_head_coord(snake* s);
coord coord_after_dir(coord c, direction dir);

#endif
