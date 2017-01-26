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
    printf("C: Clientversionsnummer: %s", version);
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
        printf("C: Gewünschte Spielernummer: %d\n", player_id);
    } else {
        printf("C: Keine präferierte Spielernummer\n");
    }

    strcat(msg, delimiter);
}

void create_msg_id(char *id_game, char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "ID ";
    printf("C: Möchte mit GameID %s verbinden.\n", id);
    strcpy(msg, id);
    strcat(msg, id_game);
    strcat(msg, delimiter);
}

void create_msg_okwait(char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "OKWAIT";
    printf("C: Ok, ich warte...\n");
    strcpy(msg, id);
    strcat(msg, delimiter);
}

void create_msg_thinking(char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "THINKING";
    printf("C: Berechne Spielzug...\n");
    strcpy(msg, id);
    strcat(msg, delimiter);
}

void create_msg_play(char *move, char msg[MAX_MESSAGE_LENGTH]){
    char id[] = "PLAY ";
    printf("C: Sende Zug: %s\n", move);
    strcpy(msg, id);
    strcat(msg, move);
    strcat(msg, delimiter);
}
