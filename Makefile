all: snake snake_test

snake: game.o main.o types.o game.o AI.o
	gcc -g types.o game.o AI.o main.o -o snake

main.o: main.c game.h
	gcc -g -c main.c -Wall -Wextra

AI.o: AI.c game.h types.h
	gcc -g -c AI.c -Wall -Wextra

game.o: game.c types.h AI.h
	gcc -g -c game.c -Wall -Wextra

types.o: types.c 
	gcc -g -c types.c -Wall -Wextra



snake_test: main_test.o test_types.o types.o game.o AI.o
	gcc -g main_test.o test_types.o types.o game.o AI.o -o snake_test

main_test.o: main_test.c test_types.h
	gcc -g -c main_test.c -Wall -Wextra

test_types.o: test_types.c test_types.h types.h
	gcc -g -c test_types.c -Wall -Wextra
