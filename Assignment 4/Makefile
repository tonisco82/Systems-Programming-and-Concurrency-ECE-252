RM = rm -rf
CC = gcc
CFLAGS = -std=c99 -g -pthread -lssl -lcrypto -lm 

default: all

all: bin hogwarts

bin:
	mkdir -p bin/

hogwarts: hogwarts.c
	$(CC) $< $(CFLAGS) -o bin/hogwarts

clean:
	$(RM) bin/ 

.PHONY: hogwarts 
