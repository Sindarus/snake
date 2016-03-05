/*
    \file AI.c
    \brief Functions related to AI.
    \details This file is separated in 2 parts :
             1 - functions that are used by an AI main function
             2 - AI main function : they have to return the choosen direction to go.
*/

#include <stdlib.h>     //for 'rand()'

#include "types.h"

#include "AI.h"


// Helpers =============================================================
/**
* \fn int detect(snake* s, direction c, field* map);
* \brief function that returns 1 if the 's' snake can go in the
*        'c' direction without dying.
*/
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

// AI main functions ===================================================
/**
* \fn dir rngesus(snake* s);
* \brief chooses a direction to move randomly. No wall avoiding.
* \details The function picks a random direction. If the direction is
*          the opposite of the direction the schlanga is moving, then it
*          repicks a direction, and so on... No wall avoiding.
*          You may have noticed that we affect an int to 'd' which is
*          a direction. That's possible because behind every enumeration
*          lies an int. 'UP' is in fact 0, 'DOWN' is in fact 1 ... etc 
* \returns direction choosen
*/
direction rngesus(snake* s){
    direction d;

    do{
        d = rand() % 4;
    }while(d == opposite(s->dir));

    return d;
}

/**
* \fn dir rngesus2(snake* s);
* \brief chooses a direction to move randomly. With wall avoiding mechanism.
*/
direction rngesus2(snake* s, field* map){
    direction dir;

    do{
        dir = rand() % 4;
    }while(dir == opposite(s->dir) || !detect(s, dir, map));

    return dir;
}

/**
* \fn dir IA_random_v3(snake* s);
* \brief chooses a direction to move randomly. With wall avoiding mechanism.
*        With self_avoiding mechanism. Protection against infinite picking.
*/
direction AI_random_v3(snake* s, field* map){
    direction dir;
    bool bad_move;
    int i;
    int pick_counter = 0;

    do{
        dir = rand() % 4;
        pick_counter++;

        //check if this move makes the snake go into his own body
        bad_move = 0;
        for(i=0; i<s->size; i++){
            if(i == s->head) continue;
            if(are_equal(
                    s->body[i],     
                    coord_after_dir(get_head_coord(s), dir) )){
                bad_move = 1;
            }
        }
    }while((dir == opposite(s->dir) || !detect(s, dir, map) || bad_move)
            && pick_counter < 10);

    return dir;
}