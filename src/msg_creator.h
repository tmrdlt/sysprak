//
//  msg_creator.h
//  bashni
//
//  Created by Holzner on 05.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#ifndef msg_creator_h
#define msg_creator_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_MESSAGE_LENGTH 100

extern char *version;

void create_msg_version(char msg [MAX_MESSAGE_LENGTH]);

void create_msg_player(int player_id, char msg [MAX_MESSAGE_LENGTH]);

void create_msg_id(char *id, char msg [MAX_MESSAGE_LENGTH]);

void create_msg_okwait(char msg [MAX_MESSAGE_LENGTH]);

void create_msg_thinking(char msg [MAX_MESSAGE_LENGTH]);

void create_msg_play(char *move, char msg [MAX_MESSAGE_LENGTH]);

#endif /* msg_creator_h */
