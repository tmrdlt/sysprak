//
//  msg_creator.c
//  bashni
//
//  Created by Holzner on 05.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#include "msg_creator.h"
#include "string_helper.h"

char *version = "2.3";
char *delimiter ="\n";


void create_msg_version(char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "VERSION ";

    strcpy(msg, id);
    strcat(msg, version);
    strcat(msg, delimiter);
}

void create_msg_player(int player_id, char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "PLAYER ";
    char id_string[4];

    strcpy(msg, id);

    if(player_id >= 0){
        int_to_string(player_id, id_string);
        strcat(msg, id_string);
    }

    strcat(msg, delimiter);
}

void create_msg_id(char *id_game, char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "ID ";
    strcpy(msg, id);
    strcat(msg, id_game);
    strcat(msg, delimiter);
}

void create_msg_okwait(char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "OKWAIT";
    strcpy(msg, id);
    strcat(msg, delimiter);
}

void create_msg_thinking(char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "THINKING";
    strcpy(msg, id);
    strcat(msg, delimiter);
}

void create_msg_play(char *move, char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "PLAY ";
    strcpy(msg, id);
    strcat(msg, move);
    strcat(msg, delimiter);
}
