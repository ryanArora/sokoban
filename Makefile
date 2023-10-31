CC = gcc
CFLAGS = -Wall -I src/include

all: game

game: src/game.c src/random.c
	$(CC) $(CFLAGS) -o game src/game.c src/random.c

clean:
	rm -f game

.PHONY: all clean
