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


char *create_msg_version(){
    char id[] = "VERSION ";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id) + strlen(version) + strlen(delimiter)));
    
    if(msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    
    strcpy(msg, id);
    strcat(msg, version);
    strcat(msg, delimiter);
    
    return msg;
}

char *create_msg_player(int player_id){
    char id[] = "PLAYER ";
    char *msg;
    char *id_string;
    
    int_to_string(player_id, id_string);
    
    if(player_id >= 0){
        
        msg = (char*) malloc(sizeof(char)*(strlen(id) + strlen(id_string) + strlen(delimiter)));

    }else
        msg = (char*) malloc(sizeof(char)*(strlen(id)  + strlen(delimiter)));
    
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    strcpy(msg, id);
    if(player_id)
        strcat(msg, id_string);
    strcat(msg, delimiter);
    return msg;
}

char *create_msg_id(char *id_game){
    char id[] = "ID ";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id) + strlen(id_game) + strlen(delimiter)));
    
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    strcpy(msg, id);
    strcat(msg, id_game);
    strcat(msg, delimiter);
    return msg;
}

char *create_msg_okwait(){
    char id[] = "OKWAIT";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id)  + strlen(delimiter)));
    
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    strcpy(msg, id);
    strcat(msg, delimiter);
    return msg;
}

char *create_msg_thinking(){
    char id[] = "THINKING";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id)  + strlen(delimiter)));
    
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
    strcpy(msg, id);
    strcat(msg, delimiter);
    return msg;
}

char *create_msg_play(char *move){
    char id[] = "PLAY ";
    char *msg = (char*) malloc(sizeof(char)*(strlen(id) + strlen(move) + strlen(delimiter)));
   
    if (msg == NULL){
        perror("Es konnte kein Speiecher alloziert werden");
        exit(EXIT_FAILURE);
    }
     strcpy(msg, id);
    strcat(msg, move);
    strcat(msg, delimiter);
    return msg;
}



