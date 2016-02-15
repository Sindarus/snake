#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

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

	/*Constructeurs*/

coord cons_coord(int x, int y) {
	coord c;
	c.x=x; c.y=y;
	return c;
}

coord cons_coordi() {
	coord c;
	c.x=0;c.y=0;
	return c;
}

snake cons_snake(int n) {
	snake s;
	s.corps=malloc(n*sizeof(coord));
	s.tete=0;
	s.taille=n;
	s.dir=DROITE;
	return s;
}

terrain cons_terrain(int k, int l) {
	int a; int b;
	terrain plat=malloc(k*sizeof(tuile*));
	for (a=0;a<k;a++) {plat[a]=malloc(l*sizeof(tuile));}
	for (a=0;a<k;a++) {
		for (b=0;b<l;b++) {
			plat[a][b]=VIDE;
		}
	}
	return plat;
}

	/*Bouger*/
coord bouger(coord c, int dx, int dy){
	coord d=cons_coord(c.x+dx, c.y+dy);
	return d;
}

void collision(terrain t, coord c){
	if (t[c.x][c.y] == VIDE){
		t[c.x][c.y]=SNAKE;
	}
	else{
		fin_de_partie();
	}
}

void haut(terrain t,snake s){
	coord c=s.corps[s.tete];
	coord d;
	s.dir=HAUT;
	s.tete=(s.tete-1)%(s.taille);
	d=bouger(c,1,0);
	s.corps[s.tete]=d;
	t[c.x][c.y]=VIDE;
	collision(t,d);
}

void bas(terrain t,snake s){
	coord c=s.corps[s.tete];
	coord d;
	s.dir=BAS;
	s.tete=(s.tete-1)%(s.taille);
	d=bouger(c,-1,0);
	s.corps[s.tete]=d;
	t[c.x][c.y]=VIDE;
	collision(t,d);
}

void gauche(terrain t,snake s){
	coord c=s.corps[s.tete];
	coord d;
	s.dir=GAUCHE;
	s.tete=(s.tete-1)%(s.taille);
	d=bouger(c,0,-1);
	s.corps[s.tete]=d;
	t[c.x][c.y]=VIDE;
	collision(t,d);
}

void droite(terrain t,snake s){
	coord c=s.corps[s.tete];
	coord d;
	s.dir=DROITE;
	s.tete=(s.tete-1)%(s.taille);
	d=bouger(c,0,1);
	s.corps[s.tete]=d;
	t[c.x][c.y]=VIDE;
	collision(t,d);
}

void auto_deplacement(terrain t, snake s) {
	coord c=s.corps[s.tete];
	s.tete=(s.tete-1)%(s.taille);
	switch (s.dir) {
		case HAUT:{
			haut(t,s);
			break;}
		case BAS:{
			bas(t,s);
			break;}
		case GAUCHE:{
			gauche(t,s);
			break;}
		case DROITE:{
			droite(t,s);
			break;}
	}
}

	/*Affichage*/

void affiche_terrain(terrain t, int k, int l) {
	int i; int j;
	for(i=0;i<k;i++){
		for(j=0;j<l;j++){
			switch (t[i][j]){
				case VIDE:
					printf(" ");
					break;
				case SNAKE:
					printf("s");
					break;
				case MUR:
					printf("#");
					break;
			}
			printf("\n");
		}
	}
}

int main(){return 0;}
	/*Gestion commande*/