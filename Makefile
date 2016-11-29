CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c99
LDFLAGS = -lm -lpthread
LDLIBS = -lm

all: performConnection.o connect_to_server.o shared_memory_segment.o play

connect_to_server.o: connect_to_server.c connect_to_server.h config.h
		$(CC) $(CFLAGS) -c connect_to_server.c

performConnection.o: performConnection.c performConnection.h config.h
		$(CC) $(CFLAGS) -c performConnection.c

shared_memory_segment.o: shared_memory_segment.c shared_memory_segment.h
		$(CC) $(CFLAGS) -c shared_memory_segment.c

config.o: config.c config.h
		$(CC) $(CFLAGS) -c config.c

play: performConnection.o connect_to_server.o shared_memory_segment.o config.o main.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o connect_to_server.o config.o main.c $(LDLIBS)

clean:
	rm -f performConnection.o connect_to_server.o shared_memory_segment.o config.o play
