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


void create_msg_version(char **dest){
    char id[] = "VERSION ";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id) + strlen(version) + strlen(delimiter)));
    
    strcpy(msg, id);
    strcat(msg, version);
    strcat(msg, delimiter);
    
    strcpy(*dest, msg);
    free(msg);
}

void create_msg_player(char **dest, int player_id){
    char id[] = "PLAYER ";
    char id_string[15];
    char *msg = (char*) malloc(sizeof(char)*(strlen(id) + 2 + strlen(delimiter)));

    strcpy(msg, id);
  
    if(player_id >= 0){
        int_to_string(player_id, id_string);
        strcat(msg, id_string);
    }
    
    strcat(msg, delimiter);
    strcpy(*dest, msg);
    free(msg);
}

void create_msg_id(char **dest, char *id_game){
    char id[] = "ID ";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id) + strlen(id_game) + strlen(delimiter)));
    
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    strcpy(msg, id);
    strcat(msg, id_game);
    strcat(msg, delimiter);
    strcpy(*dest, msg);
    free(msg);
}

void create_msg_okwait(char **dest){
    char id[] = "OKWAIT";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id)  + strlen(delimiter)));
    
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    strcpy(msg, id);
    strcat(msg, delimiter);
    strcpy(*dest, msg);
    free(msg);
}

void create_msg_thinking(char **dest){
    char id[] = "THINKING";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id)  + strlen(delimiter)));
    
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    strcpy(msg, id);
    strcat(msg, delimiter);
     strcpy(*dest, msg);
     free(msg);
}

void create_msg_play(char **dest, char *move){
    char id[] = "PLAY ";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id) + strlen(move) + strlen(delimiter)));
   
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
     strcpy(msg, id);
    strcat(msg, move);
    strcat(msg, delimiter);
    strcpy(*dest, msg);
    free(msg);
}



