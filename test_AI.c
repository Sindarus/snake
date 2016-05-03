#include "types.h"
#include "AI.h"

#include "test_AI.h"

bool test_dist(){
    coord c1 = new_coord(0, 0);
    coord c2 = new_coord(0, 1);
    if(dist(c1, c2) != 1) return 0;
    else return 1;
}