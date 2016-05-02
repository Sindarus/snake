CC = gcc
CFLAGS = -g -Wall -Wextra

all: create_obj snake snake_test


create_obj:
	@ if [ ! -d "obj" ]; then mkdir obj; echo "mkdir obj";fi


snake: obj/game.o obj/main.o obj/types.o obj/game.o obj/AI.o
	$(CC) $(CFLAGS) obj/types.o obj/game.o obj/AI.o obj/main.o -o snake

obj/main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c -o $@

obj/AI.o: AI.c game.h types.h
	$(CC) $(CFLAGS) -c AI.c -o $@

obj/game.o: game.c types.h AI.h
	$(CC) $(CFLAGS) -c game.c -o $@

obj/game_with_no_display.o: game.c types.h AI.h
	$(CC) -c game.c -DDO_NOT_DISPLAY -o game_with_no_display.o -o $@

obj/types.o: types.c
	$(CC) $(CFLAGS) -c types.c -o $@



snake_test: obj/main_test.o obj/test_types.o obj/types.o obj/game_with_no_display.o obj/AI.o
	$(CC) $(CFLAGS) obj/main_test.o obj/test_types.o obj/types.o obj/game_with_no_display.o obj/AI.o -o snake_test

obj/main_test.o: main_test.c test_types.h
	$(CC) $(CFLAGS) -c main_test.c -o $@

obj/test_types.o: test_types.c test_types.h types.h
	$(CC) $(CFLAGS) -c test_types.c -o $@


clean :
	@rm -f *.o
