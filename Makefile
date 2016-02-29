snake: jeu.o main.o
	gcc -o snake jeu.o main.o

jeu.o: jeu.c
	gcc -c jeu.c -Wall -Wextra

main.o: main.c jeu.h
	gcc -c main.c -Wall -Wextra


