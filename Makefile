CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c99
LDFLAGS = -lm -lpthread
LDLIBS = -lm

all: performConnection.o connect_to_server.o play

connect_to_server.o: connect_to_server.c connect_to_server.h config.h
		$(CC) $(CFLAGS) -c connect_to_server.c

performConnection.o: performConnection.c performConnection.h config.h
		$(CC) $(CFLAGS) -c performConnection.c


config.o: config.c config.h
		$(CC) $(CFLAGS) -c config.c

play: performConnection.o connect_to_server.o config.o main.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o connect_to_server.o config.o main.c $(LDLIBS)

clean:
	rm -f performConnection.o connect_to_server.o  config.o play
