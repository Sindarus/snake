MAC2_snake: jeu.o main.o
	gcc -o MAC2_snake jeu.o main.o

jeu.o: jeu.c
	gcc -o jeu.o -c jeu.c -Wall -Wextra

main.o: main.c jeu.h
	gcc -o main.o -c main.c -Wall -Wextra
