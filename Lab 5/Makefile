# Makefile, ECE252
# Yiqing Huang <yqhuang@uwaterloo.ca>

CC = gcc
CFLAGS_XML2 = $(shell xml2-config --cflags)
CFLAGS_CURL = $(shell curl-config --cflags)
CFLAGS = -Wall $(CFLAGS_XML2) $(CFLAGS_CURL) -std=gnu99 -g -DDEBUG1_
LD = gcc
LDFLAGS = -std=gnu99 -g
LDLIBS_XML2 = $(shell xml2-config --libs)
LDLIBS_CURL = $(shell curl-config --libs)
LDLIBS = $(LDLIBS_XML2) $(LDLIBS_CURL)

SRCS   = curl_xml.c findpng3.c
HEADERS = curl_xml.h
OBJS3  = findpng3.o
TARGETS= findpng3

all: ${TARGETS}

findpng3: $(HEADERS) $(SRCS)
	$(LD) -o findpng3 $(SRCS) $(CFLAGS) $(LDLIBS) $(LDFLAGS)

test1: findpng3
	valgrind --leak-check=full --show-leak-kinds=all ./findpng3 -m 10 -v logfile.txt http://ece252-1.uwaterloo.ca/lab4

test2: findpng3
	valgrind --leak-check=full --show-leak-kinds=all ./findpng3 -t 5 -m 30 -v logfile.txt http://ece252-1.uwaterloo.ca/lab4

test3: findpng3
	valgrind --leak-check=full --show-leak-kinds=all ./findpng3 -t 10 -m 50 -v logfile.txt http://ece252-1.uwaterloo.ca/lab4

test4: findpng3
	valgrind --leak-check=full --show-leak-kinds=all ./findpng3 -t 10 -m 60 -v logfile.txt http://ece252-1.uwaterloo.ca/lab4

clean:
	rm -f *~ *.d *.o *.gch $(TARGETS) *.png *.html *.txt output*
