#ifndef H_GAME
#define H_GAME

// CONSTANTS ==========================================
#define C_UP 122		/** \def C_UP id number of the key used to move up. 122 = z 	*/
#define C_DOWN 115		/** \def C_UP id number of the key used to move down. 115 = s 	*/
#define C_LEFT 113		/** \def C_UP id number of the key used to move left. 113 = q 	*/
#define C_RIGHT 100		/** \def C_UP id number of the key used to move right. 100 = d 	*/
#define C_QUIT 27		/** \def C_UP id number of the key used to quit. 27 = <esc>		*/

#define MIN_WINDOW_WIDTH 15
#define MIN_WINDOW_HEIGHT 15

#define M_WIN "You win!"
#define M_LOSE "Game over!"
#define M_QUIT "Goodbye"

#define clear() printf("\e[1;1H\e[2J") // Clear screen


// STRUCTURES =========================================
/**
* \struct coord
* \brief struct Represents a couple of coordinates.
*/
typedef struct {
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
typedef enum {EMPTY, SNAKE, WALL} square;

/**
* \sctruct snake
* \brief Represents a snake
* \details 'body' is an array of 'coord'.
* 		   'head' holds the index of the coordinates of the head in 'body'
*		   'tail' holds the index of the coordinates of the tail in 'body'
*		   'dir' is the direction the snake is currently moving.
*/
typedef struct {
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
typedef struct {
	square** f;
	int width;
	int height;
} field;

// PROTOTYPES =========================================
// Constructors ======================================
coord new_coord(int x, int y);
coord new_coord_empty();
snake* new_snake(int size, coord start_pos, field* map);
field* new_field();

// Destructors
void free_snake(snake* s);
void free_field(field* map);

// Moving ==============================
int move(snake* s, field* map, char c);

// Display
void print_to_pos(coord pos, char c);
void mode_raw(int activate);

// Input/Output
int kbhit(void);

// Other
void game_over(field* map, snake* s, char* message);
int detect(snake* s, direction c, field* map);

void play(int size);


#endif
