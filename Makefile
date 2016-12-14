CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c99
LDFLAGS = -lm -lpthread
LDLIBS = -lm

# Variablen:
GAMEID = 123gameid1234 	# -g flag (REQUIRED)
PLAYER = 1							# -p flag (OPTIONAL)
CONFIG = client.conf		# -f flag (OPTIONAL)

OBJ = config.o msg_creator.o string_helper.o performConnection.o connect_to_server.o shared_memory_segment.o thinker.o court_helper.o
SRC = $(OBJ:%.o=%.c)
HDR = $(OBJ:%.o=%.h) bashni_structs.h

client: $(OBJ) main.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) main.c $(LDLIBS)

play: client
		./$< -g $(GAMEID)



.PHONY: clean
clean:
		rm -f $(OBJ) play client
