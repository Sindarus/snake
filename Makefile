snake: game.o main.o
	gcc game.o main.o -o snake

game.o: game.c
	gcc -c game.c -Wall -Wextra

main.o: main.c game.h
	gcc -c main.c -Wall -Wextra


