//
//  thinker.h
//  bashni
//
//  Created by Holzner on 07.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#ifndef thinker_h
#define thinker_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "shared_memory_segment.h"
#include "bashni_structs.h"
#include "string_helper.h"
#include "court_helper.h"

void think_nxt_move(field **court , int time_in_s, int max_size_court,char my_color);

void think_nxt_move_white(field **court , int time_in_s, int max_size_court,char my_color,char *move);

void think_nxt_move_black(field **court , int time_in_s, int max_size_court,char my_color,char *move);

bool char_cmp_ignore_case(char char_1 , char char_2);

void build_move(char *old_pos, char *new_pos, char *dest);

void think(int id_seg_gameparams);

void test_thinker();

//void print_court(field **court, int size);

#endif /* thinker_h */
