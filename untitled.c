#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#define C_UP 122
#define C_DOWN 115
#define C_LEFT 113
#define C_RIGHT 100
#define C_QUIT 27
#define MIN_WINDOW_WIDTH 15
#define MIN_WINDOW_HEIGHT 15

#define clear() printf("\e[1;1H\e[2J") // Clear screen

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

/*Affichage*/
// Set cursor to [x,y] pos and print char param
void print_to_pos(coord pos, char c) {
	printf("\033[%d;%dH%c", pos.x, pos.y, c);
	fflush(stdout);
}

/* Constructeurs */
coord new_coord(int x, int y) {
	coord c;
	c.x = x;
	c.y = y;
	return c;
}

coord new_coord_empty() {
	coord c;
	c.x = 0;
	c.y = 0;
	return c;
}

snake* new_snake(int size, coord start_pos, field* map) {
	snake* s = malloc(sizeof(snake));
	int i;
	s->body = malloc(size*sizeof(coord));
	s->head = size-1;
	s->tail = 0;
	s->size = size;
	s->dir = UP;
	for (i = size-1; i>=0; i--) {
		s->body[i] = start_pos;
		print_to_pos(start_pos, 's');
		map->f[start_pos.x][start_pos.y] = SNAKE;
		start_pos.x++;
	}
	return s;
}

void free_snake(snake* s){
	int i;
	for(i=0;i<s->size;i++){
		free(s->body[i]);
	}
	free(s);
}

void mode_raw(int activer) 
{ 
    static struct termios cooked; 
    static int raw_actif = 0; 
  
    if (raw_actif == activer) 
        return; 
  
    if (activer) 
    { 
        struct termios raw; 
  
        tcgetattr(STDIN_FILENO, &cooked); 
  
        raw = cooked; 
        cfmakeraw(&raw); 
        tcsetattr(STDIN_FILENO, TCSANOW, &raw); 
    } 
    else 
        tcsetattr(STDIN_FILENO, TCSANOW, &cooked); 
  
    raw_actif = activer; 
}

field* new_field() {
	struct winsize sz; // Struct containing size of window
	
	ioctl(0, TIOCGWINSZ, &sz); // Calculate size
	field* map = malloc(sizeof(field));
	map->width = sz.ws_col; // Width of window
	map->height = sz.ws_row; // Height of window
	if (map->width < MIN_WINDOW_WIDTH || map->height < MIN_WINDOW_HEIGHT) {
		printf("Your window is too small\n");
		exit(1);
	}
	map->f = malloc(map->height*sizeof(square*));
	int a, b;
	coord c = new_coord_empty();
	for (a = 0; a<map->width; a++) {
		map->f[a] = malloc(map->width*sizeof(square));
	}
	for (a = 0; a<map->height; a++) {
		for (b=0; b<map->width; b++) {
			c.x = a;
			c.y = b;
			if (a == 1 || a == map->height-1 || b == 1 || b == map->width-1) {
				map->f[a][b] = WALL;
				print_to_pos(c, '#');
			} else {
				map->f[a][b] = EMPTY;
				print_to_pos(c, ' ');
			}
		}
	}
	return map;
}

void game_over() {
	mode_raw(0);
	clear();
	printf("Game over!\n");
	/* Free */
	exit(0);
}

/* Emulates kbhit() function on Windows which detects keyboard input */
int kbhit(void) 
{ 
    struct timeval tv = { 0, 0 }; 
    fd_set readfds; 
  
    FD_ZERO(&readfds); 
    FD_SET(STDIN_FILENO, &readfds); 
  
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1; 
}

/* Move */
int move(snake* s, field* map, char c) {
	coord c_head;
	coord c_tail;
	
	// Init: c_head: head's coord
	// c_tail: head's coord
	c_head = s->body[s->head];
	c_tail = s->body[s->tail];
	
	s->head = s->tail; // Pointer of head becomes pointer of tail
	s->tail = (s->head+1) % (s->size); // Pointer of head becomes next pointer

	// Updating snake
	if (c == C_UP) {
		s->dir = UP;
		s->body[s->head].x = c_head.x-1;
		s->body[s->head].y = c_head.y;
	} else if (c == C_LEFT) {
		s->dir = LEFT;
		s->body[s->head].x = c_head.x;
		s->body[s->head].y = c_head.y-1;
	} else if (c == C_RIGHT) {
		s->dir = RIGHT;
		s->body[s->head].x = c_head.x;
		s->body[s->head].y = c_head.y+1;
	} else if (c == C_DOWN) {
		s->dir = DOWN;
		s->body[s->head].x = c_head.x+1;
		s->body[s->head].y = c_head.y;
	}
	
	// Display
	print_to_pos(c_tail, ' ');
	print_to_pos(s->body[s->head], 's');
	
	if (map->f[s->body[s->head].x][s->body[s->head].y] != EMPTY) {
		game_over();
	} else {
		map->f[s->body[s->head].x][s->body[s->head].y] = SNAKE;
		map->f[c_tail.x][c_tail.y] = EMPTY;
	}
	return 0;
}

int main() {
	mode_raw(1); // Disable display of user input
	clear(); // Clear screen
	printf("\e[?25l"); // Hide cursor
	field* map = new_field(); // Field
	coord start_pos = new_coord(map->height/2, map->width/2); // Starting position of snake depending on size of the window
	snake* s = new_snake(10, start_pos, map); // Create snake with size 10 at start_pos on map
	char c; // Char for move
	
	// Loop while c is not escape
	do {
		usleep(150000); // Sleep
		// Check is user hits keyboard
		if (kbhit()) {
			c = getchar(); // Get input
		} else {
			// Automatic moves
			if (s->dir == UP) {
				c = C_UP;
			} else if (s->dir == LEFT) {
				c = C_LEFT;
			} else if (s->dir == RIGHT) {
				c = C_RIGHT;
			} else if (s->dir == DOWN) {
				c = C_DOWN;
			} else {
				exit(1);
			}
		}
		// Move
		if (c == C_UP || c == C_DOWN || c == C_LEFT || c == C_RIGHT) {
			if (move(s, map, c)) {
				exit(1);
			}
		}
	} while (c != C_QUIT);
	game_over();
	return 0;
}
