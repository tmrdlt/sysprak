//
//  thinker.c
//  bashni
//
//  Created by Holzner on 07.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#include "thinker.h"
#include <time.h>
#include <sys/wait.h>

#define MOVE_ILLEGAL 0
#define MOVE_FOR 2
#define MOVE_BACK 1
#define MOVE_COVERED 2
#define MOVE_SAFE 3
#define MOVE_BASHING 20

int fd;

int id_seg_gameparams;

//int main(int argc, char *argv[]) {
//    test_thinker();
//}

void think(){
    
    printf("Signal received\n");
    game_state *_game_state = address_shm(id_seg_gameparams);
    
    if(_game_state->flag_thinking){
        
        
        char my_color = 'w' , oponent_color = 'b';
        
        if (_game_state->player_number != 0){
            my_color = 'b' , oponent_color = 'w';
        }
        think_nxt_move(_game_state->court, 3000, COURT_SIZE, my_color, oponent_color);
    }else{
        perror("Thinkanstoß aber Connector Flag nicht gesetzt! \n");
        //... do something
    }
    
    
}


void think_nxt_move(field court[COURT_SIZE][COURT_SIZE] , int allowed_time, int max_size,char my_color, char opponent_color){
    int passed_time = 0; // TODO Update
    
    move_value rated_moves [12 *4];
    
    int count_legal_moves = 0;
    
    //rate all possible drafts
    for(int i = 0 ; i < max_size && passed_time < allowed_time; i ++){
        for(int j = 0 ; j < max_size && passed_time < allowed_time; j ++){
            
            if(court[i][j].towers[strlen(court[i][j].towers)-1] == my_color){
                //init drafts
                move_value mv[4];
                for(int n = 0; n < 4; n++){
                    mv[n].value = MOVE_ILLEGAL;
                    strcpy(mv[n].move_id, court[i][j].field_id);
                    
                }
                
                
                int bashing = check_field(court, max_size, i, j, my_color, opponent_color, mv, 0);
                
                if (bashing == 1){
                    // Hier muss nach weiteren Teilzügen gesucht werden
                }
                
                for(int n = 0; n < 4; n++){
                    if(mv[n].value > MOVE_ILLEGAL){
                        rated_moves[count_legal_moves++] = mv[n];
                    }
                }
            }
            
            passed_time += 10; // TODO Update
        }
    }
    
    move_value best_move;
    best_move.value = MOVE_ILLEGAL;
    
    //find best rated move
    for(int i = 0 ; i < count_legal_moves && passed_time < allowed_time; i ++){
        if(rated_moves[i].value > best_move.value){
            best_move = rated_moves[i];

        }
    }
    
    // send best move to connector
    printf("Next Move Found: %s Rating: %d\n" , best_move.move_id, best_move.value);
    
    ssize_t bytes_wrote = write(fd, best_move.move_id, strlen(best_move.move_id));
    
    if(bytes_wrote < 0){
        perror ("Fehler beim schreiben in pipe.");
        exit(EXIT_FAILURE);
    }
    
//    int ret_code = waitpid(pid, NULL,0);
//    
//    if (ret_code < 0) {
//        perror ("Fehler beim Warten auf Connector.");
//        exit(EXIT_FAILURE);
//    }
    
    
}

//Bewerte alle Möglichkeiten eines Feldes - return 1 wenn schlagender möglich
int check_field(field court[COURT_SIZE][COURT_SIZE],int max_size, int i_feld, int j_feld, char my_color, char opponent_color, move_value mv[4], int must_bash){
    int bashing = 0;
    int index = 0;
    
    for(int n = i_feld -1 ; n < i_feld +2 ; n++){
        if(n == i_feld) continue; // die felder direkt neben davor oder dahinter müssen nicht geprüft werden
        for(int k = j_feld-1 ; k < j_feld+2 ; k++){
            if(k == j_feld) continue; // die felder direkt neben davor oder dahinter müssen nicht geprüft werden
            
            // prüfe ob index überläuft
            if (n < max_size && k < max_size && n >= 0 && k >= 0 ){
                
                // Wenn Feld leer (beziehbar)
                if(strstr(court[n][k].towers,"_") && (must_bash != 1)){
                    //Zug bauen
                    strcat(mv[index].move_id, ":");
                    strcat(mv[index].move_id ,court[n][k].field_id);
                    
                    mv[index].old_i = i_feld; mv[index].old_j = j_feld; mv[index].new_i = n; mv[index].new_j = k;
                    
                    //Zug bewerten
                    mv[index].value += check_safe(court, max_size, n, k, opponent_color) + check_covered(court, max_size, n, k, my_color, i_feld, j_feld);
                    if (n > i_feld) mv[index].value =MOVE_ILLEGAL;
                    else mv[index].value += MOVE_FOR;
                    
                    // Wenn Gegner im Feld
                }else if(char_cmp_ignore_case(court[n][k].towers[strlen(court[n][k].towers)-1] , opponent_color)){
                    
                    // Finde das Feld hinter dem gegner
                    int next_i , next_j;
                    if(n < i_feld) next_i = n -1 ; else next_i = n + 1;
                    if(k < j_feld) next_j = k -1 ; else next_j = k + 1;
                    
                    // Falls ein Feld hinter dem Genger existiert
                    if(next_i < max_size && next_j < max_size && next_i >= 0 && next_j >= 0){
                        
                        //Prüfe ob Feld hinter Gegner Leer
                        if(strstr(court[next_i][next_j].towers,"_")){
                            strcat(mv[index].move_id, ":");
                            strcat(mv[index].move_id ,court[next_i][next_j].field_id);
                            
                            mv[index].old_i = i_feld; mv[index].old_j = j_feld; mv[index].new_i = next_i; mv[index].new_j = next_j;
                            // Bewerte zug
                            mv[index].value +=  MOVE_BASHING + check_safe(court, max_size, next_i, next_j, opponent_color) + check_covered(court, max_size, next_i, next_j, my_color, n, k);
                            if (n > i_feld ) mv[index].value +=  MOVE_BACK;
                            else mv[index].value += MOVE_FOR;
                            bashing = 1;
                        }
                    }
                    
                    
                }else{
                    // kein gültiger zug setzte MOVE_ILLEGAL
                    mv[index].value = MOVE_ILLEGAL;
                }
                index++;
            }
        }
    }
    return bashing;
}


//Check if oponent player is in range to bash your tower
int check_safe(field court[COURT_SIZE][COURT_SIZE],int max_size_court, int i, int j, char opponent_color){
    
    for (int n = i-1 ; n < i+2 ; n++){
        if(n == i) continue;
        for(int k = j-1 ; k < j+2 ; k++){
            if(k == j) continue;
            // return 0 if one can bash ur move
            if(n < max_size_court && k < max_size_court && n >= 0 && k >= 0){
                if(court[n][k].towers[strlen(court[n][k].towers)] == opponent_color)
                    return 0;
            }
        }
    }
    // MOVE_SAFE
    return MOVE_SAFE;
}

//Check if new Position would be covered by other tower
int check_covered(field court[COURT_SIZE][COURT_SIZE] ,int max_size_court, int i, int j, char my_color, int old_i , int old_j){
    
    int covered = 0;
    
    for (int n = i-1 ; n < i+2 ; n++){
        if(n == i) continue;
        
        for(int k = j-1 ; k < j+2 ; k++){
            if(k == j || (n == old_i && k == old_j)) continue;
            // return 0 if one can bash ur move
            if(n < max_size_court && k < max_size_court && n >= 0 && k >= 0){
                if(court[n][k].towers[strlen(court[n][k].towers)-1] == my_color)
                    covered += MOVE_COVERED;
            }
        }
    }
    
    return covered;
    
}


bool char_cmp_ignore_case(char char_1 , char char_2){
    return (char_1 == char_2|| char_1  == char_2+32 || char_1 == char_2 -32);
}
