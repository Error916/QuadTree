CFLAGS= -ggdb -Wall -Wextra -std=c11 -pedantic -O3
LIBS=
SRC= src/main.c src/quadtree.c
CC = gcc

quad: $(SRC)
	$(CC) $(CFLAGS) -o quad $(SRC) $(LIBS)
