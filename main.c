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

    int choice;
    bool ok = false;
    config cfg = new_config();
    cfg.players[0] = 1; //player 0 is a human
    cfg.players[1] = 2; //player 1 is an AI

    while(!ok){
        printf("----------------------------\n");
        printf("|           MENU           |\n");
        printf("----------------------------\n");
        printf("0 - Quit game\n");
        printf("1 - Play\n");
        printf("2 - How to play ?\n");
        printf("3 - About this program\n");
        printf("Please enter your choice.\n");
        if(scanf("%i", &choice) == 0){
            printf("Menu error\n");
            exit(1);
        }
        switch(choice){
            case 0:
                ok = true;
                printf("Good bye !\n");
                break;
            case 1:
                printf("Enter snake size :\n");
                if(scanf("%i", &(cfg.size)) == 0){
                    printf("Menu error\n");
                    exit(1);
                }
                if(cfg.size < 1){
                    cfg.size = 1;
                }

                printf("Select the version of the AI : 1, 2, 3 or 4 ?\n");
                if(scanf("%i", &(cfg.AI_version)) == 0){
                    printf("Menu error\n");
                    exit(1);
                }
                if(cfg.AI_version >= 1 && cfg.AI_version <= 4){
                    play(cfg);
                }
                else{
                    printf("version of AI not implemented. Selecting 4 instead. Press enter to continue.");
                    cfg.AI_version = 4;
                    scanf("%i", &choice); //"press any key to continue"
                    play(cfg);
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

