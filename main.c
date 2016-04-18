/**
* \file main.c
* \brief Entry point of the program
*/

#include <stdio.h>          //for 'printf()'
#include <stdlib.h>         //for 'srand()'
#include <unistd.h>         //for 'usleep()'
#include <time.h>           //for 'time()'
#include <signal.h>         //for 'SIGINT' and 'signal()'

#include "game.h"

/**
* \fn void about();
* \brief Displays info about the program
*/
void about(){
    printf("This game has been developped by : \n");
    printf("Marius Hinge\n");
    printf("Camille Feller\n");
    printf("Aurélien Bourgeois\n");
    printf("Clément Saintier\n");
}

/**
* \fn void quit();
* \brief safely quits
*/
void quit(){
    clear();
    mode_raw(0);
    exit(1);
}

/**
* \fn int main();
* \brief Entry point of the program.
*/
int main(){
    signal(SIGINT, quit);

    clear();
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
        printf("Please enter your choice.\n");
        if(scanf("%i", &choix) == 0){
            printf("Menu error\n");
            exit(1);
        }
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
    }

    return 0;
}

