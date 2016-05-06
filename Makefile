CC = gcc
CFLAGS = -g -Wall -Wextra

all: create_obj snake snake_test


create_obj:
	@ if [ ! -d "obj" ]; then mkdir obj; echo "mkdir obj";fi


snake: obj/game.o obj/main.o obj/types.o obj/game.o obj/AI.o
	$(CC) $(CFLAGS) obj/types.o obj/game.o obj/AI.o obj/main.o -o snake -lm

obj/main.o: src/main.c src/game.h
	$(CC) $(CFLAGS) -c src/main.c -o $@

obj/AI.o: src/AI.c src/game.h src/types.h
	$(CC) $(CFLAGS) -c src/AI.c -o $@

obj/game.o: src/game.c src/types.h src/AI.h
	$(CC) $(CFLAGS) -c src/game.c -o $@

obj/game_with_no_display.o: src/game.c src/types.h src/AI.h
	$(CC) -c src/game.c -DDO_NOT_DISPLAY -o obj/game_with_no_display.o -o $@

obj/types.o: src/types.c
	$(CC) $(CFLAGS) -c src/types.c -o $@



snake_test: obj/main_test.o obj/test_types.o obj/types.o obj/game_with_no_display.o obj/AI.o obj/test_AI.o
	$(CC) $(CFLAGS) obj/main_test.o obj/test_types.o obj/test_AI.o obj/types.o obj/game_with_no_display.o obj/AI.o -o snake_test -lm

obj/main_test.o: test/main_test.c test/test_types.h
	$(CC) $(CFLAGS) -c test/main_test.c -o $@

obj/test_types.o: test/test_types.c test/test_types.h src/types.h
	$(CC) $(CFLAGS) -c test/test_types.c -o $@

obj/test_AI.o: test/test_AI.c test/test_AI.h src/types.h
	$(CC) $(CFLAGS) -c test/test_AI.c -o $@



clean :
	@rm -f *.o
