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
* \fn void instructions();
* \brief Displays info about how to play the game
*/
void instructions(){
    printf("To move your snake, use ZQSD. (Z for up, S for down, Q for left and D for right.)\n");
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
    bool ok = false;

    while(!ok){
        printf("----------------------------\n");
        printf("|           MENU           |\n");
        printf("----------------------------\n");
        printf("0 - Quit game\n");
        printf("1 - Play\n");
        printf("2 - How to play ?\n");
        printf("3 - About this program\n");
        printf("Please enter your choice.\n");
        if(scanf("%i", &choix) == 0){
            printf("Menu error\n");
            exit(1);
        }
        switch(choix){
            case 0:
                ok = true;
                printf("Good bye !\n");
                break;
            case 1:
                printf("Enter snake size :\n");
                if(scanf("%i", &taille) == 0){
                    printf("Menu error\n");
                    exit(1);
                }
                if(taille < 1){
                    taille = 1;
                }

                printf("Select the level of the AI : 1, 2, 3 or 4 ?\n");
                if(scanf("%i", &choix) == 0){
                    printf("Menu error\n");
                    exit(1);
                }
                if(choix >= 1 && choix <= 4){
                    play(taille, choix);
                }
                else{
                    printf("Level of AI not implemented. Selecting 4 instead. Press enter to continue.");
                    scanf("%i", &choix); //"press any key to continue"
                    play(taille, 4);
                }
                break;
            case 2:
                instructions();
                break;
            case 3:
                about();
                break;
            default :
                printf("Choose again please.\n");
                break;
        }
    }

    return 0;
}

