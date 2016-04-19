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
#include "AI.h"

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
        printf("1 - Play (IA level 0)\n");
        printf("2 - Play (IA level 1)\n");
        printf("3 - Play (IA level 2)\n");
        printf("4 - About this program\n");
        printf("To move your snake, please use Z for up, S for down, Q for left and D for right.\n");
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
                play(taille,rngesus);
                break;
            case 2:
                printf("Enter snake size :\n");
                scanf("%i", &taille);
                play(taille,rngesus2);
                break;
            case 3:
                printf("Enter snake size :\n");
                scanf("%i", &taille);
                play(taille,spread);
                break;
            case 4:
                about();
                break;
            default :
                ok = 0; printf("Choose again please.\n");
                break;
        }
    }

    return 0;
}

