/**
* \file AI.c
* \brief Functions related to AI.
* \details This file is separated in 2 parts :
*          1 - functions that are used by an AI main function
*          2 - AI main function : they have to return the choosen direction to go.
*/

#include <stdlib.h>     //for 'rand()'
#include <stdbool.h>
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
direction rngesus(snake* s,field* map){
    int n=map->height;
    n++;
    direction d;

    do{
        d = rand() % 4;
    }while(d == opposite(s->dir));

    return d;
}

/**
* \fn direction rngesus2(snake* s, field* map);
* \brief chooses a direction to move randomly. With wall and snake
*        avoiding mechanism.
*/
direction rngesus2(snake* s, field* map){
    direction dir;
    int pick_counter = 0;

    do{
        dir = rand() % 4;
        pick_counter++;
    }while( (dir == opposite(s->dir) || !detect(s, dir, map))
                && pick_counter < IA_MAX_PICK);

    return dir;
}

bool not_in(coord c, coord* tableau, int taille){
    int i;
    for(i=0;i<taille;i++){
        if (are_equal(c,tableau[i])){
            return false;
        }
    }
    return true;
}

direction spread(snake* s,field* map){
    coord tableft[(map->width)*(map->height)];
    coord tabright[(map->width)*(map->height)];
    coord tabup[(map->width)*(map->height)];
    coord tabdown[(map->width)*(map->height)];

    coord start=(s->body[s->head]);
    coord u=coord_after_dir(start,UP);
    coord d=coord_after_dir(start,DOWN);
    coord l=coord_after_dir(start,LEFT);
    coord r=coord_after_dir(start,RIGHT);

    int a1,a2,a3,a4;

    int rec(field* map, coord c, coord* tableau, int i){
        coord up=coord_after_dir(c,UP);
        coord down=coord_after_dir(c,DOWN);
        coord left=coord_after_dir(c,LEFT);
        coord right=coord_after_dir(c,RIGHT);

        if ( get_square_at(map,up) == EMPTY && not_in(up,tableau,i)){
            tableau[i]=up;
            i++;
            rec(map,up,tableau,i);
        }
        else if ( get_square_at(map,right) == EMPTY && not_in(right,tableau,i)){
            tableau[i]=right;
            i++;
            rec(map,right,tableau,i);
        }
        else if ( get_square_at(map,down) == EMPTY && not_in(down,tableau,i)){
            tableau[i]=down;
            i++;
            rec(map,down,tableau,i);
        }
        else if ( get_square_at(map,left) == EMPTY && not_in(left,tableau,i)){
            tableau[i]=left;
            i++;
            rec(map,left,tableau,i);
        }
        else{
            return i;
        }
    }

    if (get_square_at(map,l) == EMPTY){a1=rec(map,l,tableft,0);}
    else{a1=0;}
    if (get_square_at(map,r) == EMPTY){a2=rec(map,r,tabright,0);}
    else{a2=0;}
    if (get_square_at(map,d) == EMPTY){a3=rec(map,d,tabdown,0);}
    else{a3=0;}
    if (get_square_at(map,u) == EMPTY){a4=rec(map,u,tabup,0);}
    else{a4=0;}

    if (a1>a2 && a1>a3 && a1>a4){
        return LEFT;
    }
    else if (a2>a1 && a2>a3 && a2>a4){
        return RIGHT;
    }
    else if (a3>a2 && a3>a1 && a3>a4){
        return DOWN;
    }
    else if (a4>a2 && a4>a1 && a4>a3){
        return UP;
    }
    else {
        return rngesus2(s,map);
    }

}

