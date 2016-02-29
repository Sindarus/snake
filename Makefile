snake: game.o display.o main.o
	gcc display.o game.o main.o -o snake

display.o: display.c
	gcc -c display.c -Wall -Wextra

game.o: game.c
	gcc -c game.c -Wall -Wextra

main.o: main.c game.h
	gcc -c main.c -Wall -Wextra


