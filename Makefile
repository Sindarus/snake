snake: game.o main.o types.o game.o AI.o
	gcc -g types.o game.o AI.o main.o -o snake

main.o: main.c game.h game.h
	gcc -g -c main.c -Wall -Wextra

AI.o: AI.c game.h types.h
	gcc -g -c AI.c -Wall -Wextra

game.o: game.c types.h AI.h
	gcc -g -c game.c -Wall -Wextra

types.o: types.c 
	gcc -g -c types.c -Wall -Wextra

