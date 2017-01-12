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

typedef enum {UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT} direction;

bool tower_is_dame(char tower);

field* next_field(direction dir , field court[COURT_SIZE][COURT_SIZE], int i , int j,int max_size, int *new_i, int *new_j);

void think_nxt_move(field court[COURT_SIZE][COURT_SIZE]  , int time_in_s, int max_size_court,char my_color, char opponent_color);

bool char_cmp_ignore_case(char char_1 , char char_2);

void think();

int check_field(field court[COURT_SIZE][COURT_SIZE] ,int max_size, int i_feld, int j_feld, char my_color, char opponent_color, move_value mv[4], int must_bash);

int check_bashing(field court[COURT_SIZE][COURT_SIZE],int max_size, direction dir, int i_feld, int j_feld, move_value mv[4], int index, char my_color, char opponent_color);

int check_dame(field court[COURT_SIZE][COURT_SIZE],int max_size, direction dir, int i_feld, int j_feld, move_value *mv, char my_color, char opponent_color, int must_bash);

int check_safe(field court[COURT_SIZE][COURT_SIZE],int max_size_court, int i_field, int j_field, char opponent_color);

int check_covered(field court[COURT_SIZE][COURT_SIZE] ,int max_size_court,int i_field, int j_field, char my_color ,  direction dir_move);

extern int fd;

extern int id_seg_gameparams;


//void print_court(field **court, int size);

#endif /* thinker_h */
