//
//  shared_memory_segment.h
//  bashni
//
//  Created by Holzner on 28.11.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#ifndef shared_memory_segment_h
#define shared_memory_segment_h



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
}segment;

void shared_memory_segment ();

#endif /* shared_memory_segment_h */
