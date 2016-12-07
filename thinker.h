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

char *think_nxt_move(char **court , int time_in_s, int max_size_court);

void think(int id_seg_gameparams);

#endif /* thinker_h */
