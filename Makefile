CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c99
LDFLAGS = -lm -lpthread
LDLIBS = -lm

all: performConnection.o connect_to_server.o main

connect_to_server.o: connect_to_server.c connect_to_server.h
		$(CC) $(CFLAGS) -c connect_to_server.c

performConnection.o: performConnection.c performConnection.h
		$(CC) $(CFLAGS) -c performConnection.c

main: performConnection.o connect_to_server.o main.c bashni_structs.h
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o connect_to_server.o main.c $(LDLIBS)

clean:
	rm -f performConnection.o connect_to_server.o main
