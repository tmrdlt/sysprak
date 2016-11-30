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
#include "config.h"

typedef struct{
    char *player_name;
    int number;
    int flag;
} player;


typedef struct {
    char *game_name;
    int *player_number;
    int player_count;
    
    pid_t process_id_thinker;
    pid_t process_id_connector;
}game;

//Enum for States of Client
typedef enum { PROLOG, COURSE, DRAFT }phase;

//Attributes for handle Methods
typedef struct {
    int fd;
    char *server_reply;
    char **splited_reply;
    int count_elements;
}phase_data;

struct {
    int version_check;
    int version_accepted;
    int playing;
    int game_name;
    int you;
    int total;
    int players;
}prolog_data;

//phase table function
typedef phase phase_func_t( phase_data *data );

/**
 * Handles messages of Gameserver in Prolog
 */
phase handle_prolog(phase_data *data );

/**
 * Handles messages of Gameserver in Spielverlauf
 */
phase handle_course(phase_data *data );

/**
 * Handles messages of Gameserver in Spielzu
 */
phase handle_draft(phase_data *data );

/**
 * Handles messages of Gameserver (determin Method for actual state)
 */
phase run_phase( phase cur_phase, phase_data *data );

void performConnection(int fd, char *game_id);

//void handle(char *server_reply, int fd);

int split(char *string_to_split , char delimiter , char ***dest);

void disconnect(int fd);

int send_to_gameserver(int fd, char *message);

#endif /* performConnection_h */
