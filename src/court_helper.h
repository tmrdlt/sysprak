#ifndef court_helper_h
#define court_helper_h

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "bashni_structs.h"

void set_court(field court[COURT_SIZE][COURT_SIZE], int size, int player_id);

void set_draft(field court[COURT_SIZE][COURT_SIZE], char* draft);

void print_court(field court[COURT_SIZE][COURT_SIZE], int size, int player_id);

void print_unicode(char _char);

#endif /* court_helper_h */
