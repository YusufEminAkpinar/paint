CC=gcc
LIBS=-lraylib -lm
OPTS=-Wall -pedantic -O3

all: main

main: paint.c
	gcc paint.c $(OPTS) $(LIBS) -o out/paint
