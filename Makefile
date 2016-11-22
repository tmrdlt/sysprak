CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c99
LDFLAGS = -lm -lpthread

all: performConnection.o main

performConnection.o: performConnection.c performConnection.h
	$(CC) $(CFLAGS) -c performConnection.c

main: performConnection.o main.c bashni_structs.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o main.c

clean:
	rm -f performConnection.o main
