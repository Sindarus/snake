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
    printf("Moving snakes :\n\
        Player 1 : use ZQSD. (Z for up, S for down, Q for left and D for right.)\n\
        Player 2 : use 5123. (5 for up, 2 for down, 1 for left and 3 for right.)\n");
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
    printf("==============================================\n");
    printf("||             RNGESUS PRESENTS             ||\n");
    printf("||    (¯·._.·[SNAKE VS. SCHLANGA]·._.·¯)    ||\n");
    printf("==============================================\n");

    int choice;
    bool ok = false;
    config cfg;

    while(!ok){
        printf("----------------------------\n");
        printf("|           MENU           |\n");
        printf("----------------------------\n");
        printf("0 - Quit game\n");
        printf("1 - Play against an AI\n");
        printf("2 - Play with another human\n");
        printf("3 - How to play ?\n");
        printf("4 - About this program\n");
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
                cfg.mode = 1;

                printf("Enter snake size :\n");
                if(scanf("%i", &(cfg.size)) == 0){
                    printf("Menu error\n");
                    exit(1);
                }
                if(cfg.size < 1){
                    cfg.size = 1;
                }

                printf("Select the version of the AI : between 1 and 6\n");
                if(scanf("%i", &(cfg.AI_version)) == 0){
                    printf("Menu error\n");
                    exit(1);
                }
                if(cfg.AI_version >= 1 && cfg.AI_version <= 6){
                    play(cfg);
                }
                else{
                    printf("Version of AI not implemented. Returning to main menu.\n");
                    sleep(1);
                }
                break;
            case 2:
                cfg.mode = 2;

                printf("Enter snake size :\n");
                if(scanf("%i", &(cfg.size)) == 0){
                    printf("Menu error\n");
                    exit(1);
                }
                if(cfg.size < 1){
                    cfg.size = 1;
                }

                play(cfg);
                break;
            case 3:
                instructions();
                break;
            case 4:
                about();
                break;
            default :
                printf("This choice does not exist. Returning to main menu.\n");
                sleep(1);
                break;
        }
    }

    return 0;
}

