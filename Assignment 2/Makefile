RM = rm r-f
CC = gcc
CFLAGS = -std=c99 -g -D_POSIX_C_SOURCE=200809L

default: all

all: bin client 

bin:
	mkdir -p bin/

client: rogue-one.c
	$(CC) $< $(CFLAGS) -o bin/rogue-one

clean:
	$(RM) bin/

.PHONY: client clean
