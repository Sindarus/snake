/**
* \file AI.c
* \brief Functions related to AI.
* \details This file is separated in 2 parts :
*          1 - functions that are used by an AI main function
*          2 - AI main function : they have to return the choosen direction to go.
*/

#include <stdlib.h>     //for 'rand()'
#include <stdbool.h>
#include <math.h>
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

/*
* \fn bool not_in(coord c, coord* tableau, int taille);
* \brief Return true if the 'c' coord is in the array 'tableau' (of length 'taille'). Else return false.
*/
bool not_in(coord c, coord* tableau, int taille){
    int i;
    for(i=0;i<taille;i++){
        if (are_equal(c,tableau[i])){
            return false;
        }
    }
    return true;
}

int rec(field* map, coord c, coord* tableau, int* i){
    coord up=coord_after_dir(c,UP);
    coord down=coord_after_dir(c,DOWN);
    coord left=coord_after_dir(c,LEFT);
    coord right=coord_after_dir(c,RIGHT);

    if (get_square_at(map,c) != EMPTY || !not_in(c,tableau,i[0]) || i[0]>20){
        return 0;
    }
    else{
        tableau[i[0]]=c;
        i[0]++;

        int zz=rec(map,up,tableau,i);
        int ze=rec(map,left,tableau,i);
        int zr=rec(map,right,tableau,i);
        int zt=rec(map,down,tableau,i);
        return (1+zz+ze+zr+zt);
    }
    return 0;
}


/*
* \fn direction spread(snake* s,field* map);
* \brief Chooses a direction considering how much space is left to move in.
*        When the choice doesn't matter, rngesus2 will be used.
*/
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
    int i=0;

    a1=rec(map,l,tableft,&i);i=0;
    a2=rec(map,r,tabright,&i);i=0;
    a3=rec(map,d,tabdown,&i);i=0;
    a4=rec(map,u,tabup,&i);

    if ( (a1==a2 && a1==a3) || (a2==a3 && a2==a4) || (a3==a4 && a3==a1) || (a4==a2 && a4==a1)){
        return rngesus2(s,map);
    }
    else if (a1>=a2 && a1>=a3 && a1>=a4){
        return LEFT;
    }
    else if (a2>=a3 && a2>=a4 && a2>=a1){
        return RIGHT;
    }
    else if (a3>=a1 && a3>=a2 && a3>=a4){
        return DOWN;
    }
    else if (a4>=a1 && a4>=a2 && a4>=a3){
        return UP;
    }
    else {
        return rngesus2(s,map);
    }
}

float dist(coord depart, coord arrivee){
    return (sqrtf((depart.x-arrivee.x)*(depart.x-arrivee.x) + (depart.y-arrivee.y)*(depart.y-arrivee.y)));
}

bool compare(float a, float b){
    if (a==0){
        return false;
    }
    else if (b==0){
        return true;
    }
    else {
        return (a<b);
    }
}

direction best(float a, float b, float c, float d, snake* s, field* map){
    if ((compare(c,a)) && (compare(c,b)) && (compare(c,d))){
        return LEFT;
    }
    else if ((compare(b,a)) && (compare(b,c)) && (compare(b,d))){
        return DOWN;
    }
    else if ((compare(a,b)) && (compare(a,c)) && (compare(a,d))){
        return UP;
    }
    else if ((compare(d,a)) && (compare(d,b)) && (compare(d,c))){
        return RIGHT;
    }
    else{
        return rngesus2(s,map);
    }
}

direction aggro_dist(snake* s, field* map, snake* enemy){
    coord end=(enemy->body[enemy->head]);
    float a=0,b=0,c=0,d=0;
    coord start=(s->body[s->head]);

    if (detect(s,UP,map)){
        a=dist(coord_after_dir(start,UP),end);
    }
    else if (detect(s,DOWN,map)){
        b=dist(coord_after_dir(start,DOWN),end);
    }
    else if (detect(s,LEFT,map)){
        c=dist(coord_after_dir(start,LEFT),end);
    }
    else if (detect(s,RIGHT,map)){
        d=dist(coord_after_dir(start,RIGHT),end);
    }

    return best(a,b,c,d,s,map);
}
