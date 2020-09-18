CFLAGS?=-std=c99 -D_GNU_SOURCE -Wall -O2
CC=gcc

default: all

all: bin theflash

bin:
	mkdir -p bin/
	
theflash: theflash.c
	$(CC) $(CFLAGS) -o bin/theflash $<

clean:
	rm -rf bin/
