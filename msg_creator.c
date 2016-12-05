//
//  msg_creator.c
//  bashni
//
//  Created by Holzner on 05.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#include "msg_creator.h"


char *create_msg_version(){
    char *msg = (char*)malloc(sizeof(char)*14);
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    msg = "VERSION ";
    strcat(msg, version);
    strcat(msg, "\n");	
    return msg;
}

char *create_msg_player(char *player_id){
    char *msg = (char*)malloc(sizeof(char)*14);
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    msg = "PLAYER ";
    if(player_id)
        strcat(msg, player_id);
    strcat(msg, "\n");
    return msg;
}

char *create_msg_id(char *id){
    char *msg = (char*)malloc(sizeof(char)*20);
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    msg= "ID ";
    char id_game[16];
    sprintf(id_game, "%s", id);
    
    //sprintf(id_game, "%s", "#3h2xfhfym0oh2");
    strcat(id_game, "\n");
    strcat(msg, id_game);
    return msg;
}

char *create_msg_okwait(){
    char *msg = (char*)malloc(sizeof(char)*10);
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    msg ="OKWAIT\n";
    return msg;
}

char *create_msg_thinking(){
    char *msg = (char*)malloc(sizeof(char)*10);
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    msg ="THINKING\n";
    return msg;
}

char *create_msg_play(char *move){
    char *msg = (char*)malloc(sizeof(char)*14);
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    msg = "PLAY ";
    strcat(msg, move);
    strcat(msg, "\n");
    return msg;
}