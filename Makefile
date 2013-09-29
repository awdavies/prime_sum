CC = gcc
CFLAGS = -g -Wall -std=c99 -I.
LDFLAGS = -lpthread 
all: main

main: main.c
	${CC} ${CFLAGS} main.c -o main ${LDFLAGS}
