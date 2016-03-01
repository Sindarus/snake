#ifndef H_GAME
#define H_GAME

// CONSTANTS ==========================================
#define C_UP 122
#define C_DOWN 115
#define C_LEFT 113
#define C_RIGHT 100
#define C_QUIT 27
#define MIN_WINDOW_WIDTH 15
#define MIN_WINDOW_HEIGHT 15
#define M_WIN "You win!"
#define M_LOSE "Game over!"
#define M_QUIT "Goodbye"

#define clear() printf("\e[1;1H\e[2J") // Clear screen


// STRUCTURES =========================================
typedef struct {
	int x;
	int y;
} coord;

typedef enum {UP, DOWN, LEFT, RIGHT} direction;

typedef enum {EMPTY, SNAKE, WALL} square;

typedef struct {
	int size;
	int head;
	int tail;
	coord* body;
	direction dir;
} snake;

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

// Other
void game_over(field* map, snake* s, char* message);
int kbhit(void);

void play(int size);


#endif
