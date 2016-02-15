#ifndef H_JEU
#define H_JEU

//STRUCTURES =========================================
typedef struct {
    int x;
    int y;
} coord;

typedef enum {HAUT, BAS, GAUCHE, DROITE} direction;

typedef enum {VIDE, SNAKE, MUR} tuile;

typedef struct {
    int taille;
    int tete;
    coord* corps;
    direction dir;
} snake;

typedef tuile** terrain;

//PROTOTYPES =========================================
//constructeurs ======================================
coord cons_coord(int x, int y);
coord cons_coordi();
snake cons_snake(int n);
terrain cons_terrain(int k, int l);

//fonctions pour bouger ==============================
coord bouger(coord c, int dx, int dy);
void collision(terrain t, coord c);
void haut(terrain t, snake s);
void bas(terrain t, snake s);
void gauche(terrain t, snake s);
void droite(terrain t, snake s);
void auto_deplacement(terrain t, snake s);

//autres =============================================
void fin_de_partie();

//affichage ==========================================
void affiche_terrain(terrain t, int k, int l);

#endif
