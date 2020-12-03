RM = rm -rf
CC = gcc
CFLAGS = -std=c99 -g -pthread -lssl -lcrypto -lm 

default: all

all: bin timewar 

bin:
	mkdir -p bin/

timewar: timewar.c
	$(CC) $< $(CFLAGS) -o bin/timewar

clean:
	$(RM) bin/

.PHONY: timewar 
