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
#include "shared_memory_segment.h"
#include "bashni_structs.h"

char *think_nxt_move(char court[8][8] , int time_in_s, int max_size_court,char my_color);

void think(int id_seg_gameparams);

void test_thinker();

void print_court(char court[8][8], int size);

#endif /* thinker_h */
