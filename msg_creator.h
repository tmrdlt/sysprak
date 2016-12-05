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

char *version = "2.3";

char *create_msg_version();

char *create_msg_player(char *player_id);

char *create_msg_id(char *id);

char *create_msg_okwait();

char *create_msg_thinking();

char *create_msg_play(char *move);

#endif /* msg_creator_h */
