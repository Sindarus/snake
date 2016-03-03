#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "game.h"

// Constructors ======================================
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
	for (a = 0; a<map->height; a++) {
		map->f[a] = malloc(map->width*sizeof(square));
	}
	for (a = 0; a<map->height; a++) {
		for (b = 0; b<map->width; b++) {
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



// Destructors
void free_snake(snake* s){
	free(s->body);
	free(s);
}

void free_field(field* map){
	int i;
	for(i = 0; i<map->height; i++){
		free(map->f[i]);
	}
	free(map->f);
	free(map);
}

// Moving ==============================
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
		return -1;
		//~ game_over(map, s);
	} else {
		map->f[s->body[s->head].x][s->body[s->head].y] = SNAKE;
		map->f[c_tail.x][c_tail.y] = EMPTY;
	}
	return 0;
}

// Display ==================================
// Set cursor to [x,y] pos and print char param
void print_to_pos(coord pos, char c) {
	printf("\033[%d;%dH%c", pos.x, pos.y, c);
}

// Enable not printing characters sent with keyboard
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

// Input/Output
/* Emulates kbhit() function on Windows which detects keyboard input */
int kbhit(void) { 
    struct timeval tv = { 0, 0 }; 
    fd_set readfds; 
  
    FD_ZERO(&readfds); 
    FD_SET(STDIN_FILENO, &readfds); 
  
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1; 
}

// Other
void game_over(field* map, snake* s, char* message) {
	mode_raw(0);
	clear();
	printf("%s\n", message);
	//~ printf("You'll be redirected in 2 seconds\n");
	//~ sleep(2);
	free_field(map);
	free_snake(s);
}

int detect(snake* s, direction c, field* map){
	int a=s->body[s->head].x;
	int b=s->body[s->head].y;
	if (c == UP) {
		return (map->f[a-1][b] == EMPTY);
	} else if (c == LEFT) {
		return (map->f[a][b-1] == EMPTY);
	} else if (c == RIGHT) {
		return (map->f[a][b+1] == EMPTY);
	} else if (c == DOWN) {
		return (map->f[a+1][b] == EMPTY);
	}
	else{return 1;}
}

void play(int size) {
	clear(); // Clear screen
	printf("\e[?25l"); // Hide cursor
	field* map = new_field(); // Field
	if (size > map->height/2) {
		free_field(map);
		clear();
		printf("Please increase your window size or decrease your snake size\n");
		exit(1);
	}
	coord start_pos = new_coord(map->height/2, map->width/5); // Starting position of snake depending on size of the window
	coord start_pos2 = new_coord(map->height/2, 4*map->width/5); // Starting position of snake depending on size of the window
	snake* s = new_snake(size, start_pos, map); // Create snake with size 10 at start_pos on map
	snake* shlanga = new_snake(size, start_pos2, map); // Create snake with size 10 at start_pos on map
	char c; // Char for move
	int r; // Random
	srand(time(NULL));
	
	mode_raw(1); // Disable display of user input

	// Loop while c is not escape
	do {
		usleep(150000); // Sleep
		// Check if user hits keyboard
		if (kbhit()) {
			c = getchar(); // Get input
		} 
		
		// Move snake
		if (c == C_UP || c == C_DOWN || c == C_LEFT || c == C_RIGHT) {
			if (move(s, map, c) < 0) {
				game_over(map, s, M_LOSE);
				free_snake(shlanga);
				return;
			}
		} else {
			// Automatic moves
			if (s->dir == UP) {
				if (move(s, map, C_UP)) {
					game_over(map, s, M_LOSE);
					free_snake(shlanga);
					return;
				}
			} else if (s->dir == LEFT) {
				if (move(s, map, C_LEFT)) {
					game_over(map, s, M_LOSE);
					free_snake(shlanga);
					return;
				}
			} else if (s->dir == RIGHT) {
				if (move(s, map, C_RIGHT)) {
					game_over(map, s, M_LOSE);
					free_snake(shlanga);
					return;
				}
			} else if (s->dir == DOWN) {
				if (move(s, map, C_DOWN)) {
					game_over(map, s, M_LOSE);
					free_snake(shlanga);
					return;
				}
			} else {
				exit(1);
			}
		}
		
		// Move shlanga
		do {
			r = 1 + rand() % 4;
		} while (!(r == 1 && shlanga->dir != DOWN) &&
			!(r == 2 && shlanga->dir != RIGHT) &&
			!(r == 3 && shlanga->dir != LEFT) &&
			!(r == 4 && shlanga->dir != UP));

		if (r == 1 && shlanga->dir != DOWN && detect(shlanga,UP,map)) {
			if (move(shlanga, map, C_UP) < 0) {
				game_over(map, shlanga, M_WIN);
				free_snake(s);
				return;
			}
		} else if (r == 2 && shlanga->dir != RIGHT && detect(shlanga,LEFT,map)) {
			if (move(shlanga, map, C_LEFT) < 0) {
				game_over(map, shlanga, M_WIN);
				free_snake(s);
				return;
			}
		} else if (r == 3 && shlanga->dir != LEFT && detect(shlanga,RIGHT,map)) {
			if (move(shlanga, map, C_RIGHT) < 0) {
				game_over(map, shlanga, M_WIN);
				free_snake(s);
				return;
			}
		} else if (r == 4 && shlanga->dir != UP && detect(shlanga,DOWN,map)) {
			if (move(shlanga, map, C_DOWN) < 0) {
				game_over(map, shlanga, M_WIN);
				free_snake(s);
				return;
			}
		}
		
		fflush(stdout);
	} while (c != C_QUIT);
	
	game_over(map, s, M_QUIT);
	free_snake(shlanga);
	return;
}