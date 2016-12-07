//
//  thinker.c
//  bashni
//
//  Created by Holzner on 07.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#include "thinker.h"

void think(int id_seg_gameparams){
    game_state *_game_state = address_shm(id_seg_gameparams);
    
    if(_game_state->flag_thinking){
        think_nxt_move(_game_state->court, 3000, 8);
        
    }else{
        perror("Thinkanstoß aber Connector Flag nicht gesetzt! \n");
        //... do something
    }
}

char *think_nxt_move(char **court, int time_in_s, int max_size){

  return "asfa";
}