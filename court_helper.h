#ifndef court_helper_h
#define court_helper_h

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bashni_structs.h"


void set_court(field court[COURT_SIZE][COURT_SIZE], int size);

void set_draft(field court[COURT_SIZE][COURT_SIZE], int size, char* draft);

void print_court(field court[COURT_SIZE][COURT_SIZE], int size);
