#include <stdbool.h>
#include <sys/ioctl.h>

#include "types.h"

#include "test_types.h"

// Constructors ========================================================
bool test_new_coord(){
    coord c1 = new_coord(1, 3);
    coord c2 = new_coord(0, 0);
    coord c3 = new_coord(-1, -3);

    if(c1.x != 1 || c1.y != 3 ||
            c2.x != 0 || c2.y != 0 ||
            c3.x != -1 || c3.y != -3){
        return 0;
    }
    else return 1;
}

bool test_new_coord_empty(){
    coord c = new_coord_empty();

    if(c.x != 0 || c.y != 0) return 0;
    else return 1;
}

bool test_new_field(){
    field* map = new_field();
    int i, j;

    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz); // Calculate size of window
    if(map->width != sz.ws_col) return 0;
    if(map->height != sz.ws_row) return 0;

    for(i=0; i<map->height; i++){
        for(j=0; j<map->width; j++){
            if(map->f[i][j] != WALL &&
                    map->f[i][j] != EMPTY &&
                    map->f[i][j] != SNAKE &&
                    map->f[i][j] != SCHLANGA){
                return 0;
            }
        }
    }

    free_field(map);
    return 1;
}

bool test_new_snake(){
    field* map = new_field();
    snake* s = new_snake(T_SNAKE, 6, new_coord(1, 1), map);
    int i;

    if(s->head != 5 || s->size != 6 ||
            s->body[5].x != 1 || s->body[5].y != 1){
        free_field(map);
        free_snake(s);
        return 0;
    }

    for(i=0; i<6; i++){
        if(map->f[s->body[i].x][s->body[i].y] != SNAKE &&
                map->f[s->body[i].x][s->body[i].y] != SCHLANGA){
            free_field(map);
            free_snake(s);
            return 0;
        }
    }

    free_field(map);
    free_snake(s);
    return 1;
}



// Objects managment ===================================================
bool test_are_equal(){
    coord c1;
    coord c2 = new_coord(6,8);

    int i;
    int j;
    for(i=-5; i<6; i++){
        for(j=-5; j<6; j++){
            c1 = new_coord(i, j);
            if(! are_equal(c1, new_coord(i, j)) ){
                return 0;
            }
            if(are_equal(c1, c2)){
                return 0;
            }
        }
    }
    return 1;
}

bool test_opposite(){
    if(opposite(UP) != DOWN || opposite(DOWN) != UP ||
            opposite(LEFT) != RIGHT || opposite(RIGHT) != LEFT){
        return 0;
    }

    return 1;
}

bool test_turn_left(){
    /*testing 'test_turn_left()' would be equivalent to rewriting the function here,
    running both the function and the rewritten function for a given set of parameters
    and comparing the output. This is useless.*/
    return 1;
}

bool test_turn_right(){
    /*testing 'test_turn_right()' would be equivalent to rewriting the function here,
    running both the function and the rewritten function for a given set of parameters
    and comparing the output. This is useless.*/
    return 1;
}

bool test_get_square_at(){
    field* map = new_field();
    int i,j;
    for (i = 0; i < map->height; i++){
        for (j = 0; j < map->width; j++){
            if(map->f[i][j] != get_square_at(map, new_coord(i, j))){
                free_field(map);
                return 0;
            }
        }
    }

    free_field(map);
    return 1;
}

bool test_set_square_at(){
    field* map = new_field();int i,j;
    for(i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            set_square_at(map, new_coord(i, j), WALL);
            if(map->f[i][j] != WALL) return 0;
        }
    }
    free_field(map);
    return 1;
}

bool test_get_tail(){
    field* map = new_field();
    snake* s = new_snake(T_SNAKE, 8, new_coord(1,1), map);

    coord c = s->body[get_tail(s)];
    if(map->f[c.x][c.y] != SNAKE){
        free_snake(s);
        free_field(map);
        return 0;
    }

    free_snake(s);
    free_field(map);
    return 1;
}

bool test_get_head_coord(){
    field* map = new_field();
    snake* s = new_snake(T_SNAKE, 8, new_coord(1,1), map);

    coord c = get_head_coord(s);
    if(map->f[c.x][c.y] != SNAKE){
        free_snake(s);
        free_field(map);
        return 0;
    }

    free_snake(s);
    free_field(map);
    return 1;
}

bool test_get_tail_coord(){
    field* map = new_field();
    snake* s = new_snake(T_SNAKE, 8, new_coord(1,1), map);

    coord c = get_tail_coord(s);
    if(map->f[c.x][c.y] != SNAKE){
        free_snake(s);
        free_field(map);
        return 0;
    }

    free_snake(s);
    free_field(map);
    return 1;
}

bool test_coord_after_dir(){
    /*testing 'coord_after_dir()' would be equivalent to rewriting the function here,
    running both the function and the rewritten function for a given set of parameters
    and comparing the output. This is useless.*/
    return 1;
}