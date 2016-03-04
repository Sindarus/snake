/*
    \file main.c
    \brief Entry point of the program
*/

#include <stdio.h>          //for 'printf()'
#include <stdlib.h>         //for 'srand()'
#include <unistd.h>         //for 'usleep()'
#include <time.h>           //for 'time()'

#include "game.h"

/*
    \fn void about();
    \brief Displays info about the program
*/
void about(){
    printf("This game has been developped by : \n");
    printf("Marius Hinge\n");
    printf("Camille Feller\n");
    printf("Aurélien Bourgeois\n");
    printf("Clément Saintier\n");
}

/**
* \fn int main();
* \brief Ain'tcha ever seen a main function ?
*/
int main(){
    // FILE* fp;
    // fp = fopen("output.log", "w");
    // close(fp)

    srand(time(NULL));
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
        printf("0 - Quit game\n");
        printf("1 - Play\n");
        printf("2 - About this program\n");
        scanf("%i", &choix);
        switch(choix){
            case 0:
                ok = 1; printf("Good bye !\n");
                break;
            case 1:
                printf("Enter snake size :\n");
                scanf("%i", &taille);
                play(taille);
                break;
            case 2:
                about();
                break;
        }
        usleep(500000);
    }

    return 0;
}

