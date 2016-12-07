//
//  bashni_structs.h
//  bashni
//
//  Created by Holzner on 07.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#ifndef bashni_structs_h
#define bashni_structs_h

typedef struct {
    int fd;
    char *server_reply;
    char **splited_reply;
    int count_elements;
}phase_data;

typedef struct {
    int version_check;
    int version_accepted;
    int playing;
    int game_name;
    int you;
    int total;
    int players;
}prolog_data;

typedef struct{
    char *player_name;
    int number;
    int flag;
} player;


typedef struct {
    char *game_name;
    int player_number;
    int player_count;
    
    int process_id_thinker;
    int process_id_connector;
    
    int flag_thinking;
    
    char **court;
    
    int players_shm_ids;
    
}game_state;

#endif /* bashni_structs_h */
