/*
    \file main.c
    \brief Point d'entrée du programme
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

/*
    \fn void apropos();
    \brief Affiche des informations a propos du programme
    \details
*/
void apropos(){
	printf("Ce jeu a été développé par : \n");
	printf("Marius Hinge\n");
	printf("Camille Feller\n");
	printf("Aurélien Bourgeois\n");
	printf("Clément Saintier\n");
}

int main(){
    printf("==================================\n");
    printf("||          MAC² SNAKE          ||\n");
    printf("==================================\n");

    char input;
    int choix;
    bool ok = 0;

    int fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
    if(fd == -1) perror ("open");

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

