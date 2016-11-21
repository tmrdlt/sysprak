//
//  performConnection.h
//  bashni
//
//  Created by Holzner on 16.11.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#ifndef performConnection_h
#define performConnection_h


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "bashni_structs.h"

typedef enum {
    PROLOG,
    COURSE,
    DRAFT
}phase;

typedef enum {
    VERSION,
    ID,
    PLAYER,
    OKWAIT,
    THINKING,
    PLAY
} client_msg;


static const char *CLIENT_MSG[] = {
    "VERSION", "ID", "PLAYER", "OKWAIT","THINKING","PLAY"
};

void performConnection(int fd);

void handle(char *server_reply);

int split(char *string_to_split , char delimiter , char ***dest);

#endif /* performConnection_h */
