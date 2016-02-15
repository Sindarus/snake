#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

void apropos(){
	printf("Ce jeu a été développé par : \n");
	printf("Marius Hinge\n");
	printf("Camille Feller\n");
	printf("Aurélien Bourgeois\n");
	printf("Clément Saintier\n");
}

int main(){
    printf("==================================\n");
    printf("||          MCAC SNAKE          ||\n");
    printf("==================================\n");

    int choix;
    bool ok = 0;

    while(ok != 1){
    	printf("----------------------------\n");
	    printf("|           MENU           |\n");
	    printf("----------------------------\n");
    	printf("1 - Jouer au Snake\n");
    	printf("2 - A propos de ce programme\n");
    	printf("3 - Quitter le jeu\n");
    	scanf("%i", &choix);
    	switch(choix){
    		case 1:
    			printf("Pas encore implémenté\n"); 	break;
    		case 2:
    			apropos(); 							break;
    		case 3:
    			ok = 1; printf("Au revoir !\n"); 	break;
    	}
    	sleep(1);
    }

    return 0;
}

