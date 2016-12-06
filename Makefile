CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c99
LDFLAGS = -lm -lpthread
LDLIBS = -lm

# Variablen:
GAMEID = 123gameid1234 # -g flag (REQUIRED)
PLAYER = 1	# -p flag (OPTIONAL)
CONFIG = client.conf # -f flag (OPTIONAL)

OBJ = config.o msg_creator.o string_helper.o performConnection.o connect_to_server.o
SRC = $(OBJ:%.o=%.c)
HDR = $(OBJ:%.o=%.h)

play: client
		./$< -g $(GAMEID)

client: $(OBJ) main.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) main.c $(LDLIBS)

.PHONY: clean
clean:
		rm -f $(OBJ) play client
