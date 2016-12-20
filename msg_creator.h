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

extern char *version;

void create_msg_version(char **dest);

void create_msg_player(char **dest,int player_id);

void create_msg_id(char **dest, char *id);

void create_msg_okwait(char **dest);

void create_msg_thinking(char **dest);

void create_msg_play(char **dest, char *move);


#endif /* msg_creator_h */
