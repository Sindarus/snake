/*
    \file main.c
    \brief Point d'entrée du programme
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>

#include "game.h"

/*
    \fn void a_propos();
    \brief Affiche des informations a propos du programme
    \details
*/
void a_propos(){
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

    //char input;
    int taille;
    int choix;
    bool ok = 0;

    while(ok != 1){
        printf("----------------------------\n");
        printf("|           MENU           |\n");
        printf("----------------------------\n");
        printf("0 - Quitter le jeu\n");
        printf("1 - Jouer au Snake\n");
        printf("2 - A propos de ce programme\n");
        scanf("%i", &choix);
        switch(choix){
            case 0:
                ok = 1; printf("Au revoir !\n");
                break;
            case 1:
                printf("Entrez la taille du serpent :\n");
                scanf("%i", &taille);
                play(taille);
                break;
            case 2:
                a_propos();
                break;
        }
        usleep(500000);
    }

    return 0;
}

