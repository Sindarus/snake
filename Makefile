CC = gcc
CFLAGS = -g -Wall -Wextra

all: snake snake_test

snake: game.o main.o types.o game.o AI.o
	$(CC) $(CFLAGS) types.o game.o AI.o main.o -o snake

main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c

AI.o: AI.c game.h types.h
	$(CC) $(CFLAGS) -c AI.c

game.o: game.c types.h AI.h
	$(CC) $(CFLAGS) -c game.c

game_with_no_display.o: game.c types.h AI.h
	$(CC) -c game.c -DDO_NOT_DISPLAY -o game_with_no_display.o

types.o: types.c
	$(CC) $(CFLAGS) -c types.c



snake_test: main_test.o test_types.o types.o game_with_no_display.o AI.o
	$(CC) $(CFLAGS) main_test.o test_types.o types.o game_with_no_display.o AI.o -o snake_test

main_test.o: main_test.c test_types.h
	$(CC) $(CFLAGS) -c main_test.c

test_types.o: test_types.c test_types.h types.h
	$(CC) $(CFLAGS) -c test_types.c
