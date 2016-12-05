CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c99
LDFLAGS = -lm -lpthread
LDLIBS = -lm

config.o: config.c config.h
		$(CC) $(CFLAGS) -c config.c

msg_creator.o: msg_creator.c msg_creator.h
		$(CC) $(CFLAGS) -c msg_creator.c

string_helper.o: string_helper.c string_helper.h
		$(CC) $(CFLAGS) -c string_helper.c

performConnection.o: performConnection.c performConnection.h config.h msg_creator.h string_helper.h
				$(CC) $(CFLAGS) -c performConnection.c

connect_to_server.o: connect_to_server.c connect_to_server.h config.h
		$(CC) $(CFLAGS) -c connect_to_server.c

play: performConnection.o connect_to_server.o config.o msg_creator.o string_helper.o main.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ performConnection.o connect_to_server.o config.o msg_creator.o string_helper.o main.c $(LDLIBS)

clean:
	rm -f config.o msg_creator.o string_helper.o performConnection.o connect_to_server.o play
