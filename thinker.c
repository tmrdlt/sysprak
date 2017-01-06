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

void think(){

  

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

    move_value rated_moves [12 *12];

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


                check_field(court, max_size, i, j, my_color, opponent_color, mv, 0);


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

}


/*
 * Damen Zug rekusive Iteration diagonal in eine Richtung über das Spielfeld
 */
int check_dame(field court[COURT_SIZE][COURT_SIZE],int max_size, direction dir, int i_feld, int j_feld, move_value mv, char my_color, char opponent_color){
    
    int next_i, next_j;
    field next = next_field(dir, court, i_feld, j_feld, max_size, &next_i, &next_j);
    
    if (next.field_id[0] !='x'){
        
        
        if(strstr(next.towers,"_") || next.f_would_be_empty == 1){
           
            //bewerte möglichen Zug
            mv.value += check_safe(court, max_size, next_i, next_j, opponent_color) + check_covered(court, max_size, next_i, next_j, my_color, dir);
            
            if (dir >= LOWER_LEFT) mv.value +=  MOVE_BACK;
            else mv.value += MOVE_FOR;
            
            move_value rate_nxt_move;
            
            strcpy(rate_nxt_move.move_id, mv.move_id);
            
            strcat(mv.move_id, ":");
            strcat(mv.move_id, next.field_id);
            
            //prüfe weiter in selber Richtung
            check_dame(court, max_size, dir, next_i, next_j, rate_nxt_move,  my_color, opponent_color);
            
            //teste ob Zug ins nächste Feld besser bewertet wäre
            if(rate_nxt_move.value > mv.value){
                mv = rate_nxt_move;
            }
            
        }else if(char_cmp_ignore_case(next.towers[strlen(next.towers)-1] , opponent_color)){
            //find field behind
            int i_behind, j_behind;
            field field_behind;
            
            
            while ((field_behind = next_field(dir, court, next_i, next_j, max_size, &i_behind, &j_behind)).field_id[0] != 'x') {
                if(!strstr(field_behind.towers,"_") && field_behind.f_would_be_empty != 1){
                    break;
                
                }else{
                    
                }
            }
            
            return 1;
        }
        
        return 0;
       
    } else{
        mv.value = MOVE_ILLEGAL;
        return -1;
    }
    
}

//Bewerte alle Möglichkeiten eines Feldes - return 1 wenn schlagender möglich
int check_field(field court[COURT_SIZE][COURT_SIZE],int max_size, int i_feld, int j_feld, char my_color, char opponent_color, move_value mv[4], int must_bash){
    int bashing = 0;
    int index = 0;

    for(direction dir = UPPER_LEFT ; dir <= LOWER_RIGHT ; dir++){
        
        
        int next_i, next_j;
        field next = next_field(dir, court, i_feld, j_feld, max_size, &next_i, &next_j);
    
            // prüfe ob index überläuft
            if (next.field_id[0] !='x'){

                // Wenn Feld leer (beziehbar)
                if(strstr(next.towers,"_") && (must_bash != 1)){
                    //Zug bauen
                    strcat(mv[index].move_id, ":");
                    strcat(mv[index].move_id, next.field_id);

                     //Zug bewerten
                    mv[index].value += check_safe(court, max_size, next_i, next_j, opponent_color) + check_covered(court, max_size, next_i, next_j , my_color, dir);
                    
                    if (dir >= LOWER_LEFT) mv[index].value =MOVE_ILLEGAL;
                    else mv[index].value += MOVE_FOR;

                    // Wenn Gegner im Feld
                }else if(char_cmp_ignore_case(next.towers[strlen(next.towers)-1] , opponent_color)){

                    bashing = check_bashing(court, max_size, dir, i_feld, j_feld, mv, index, my_color , opponent_color);
                    
                }else{
                    // kein gültiger zug setzte MOVE_ILLEGAL
                    mv[index].value = MOVE_ILLEGAL;
                }
                index++;
            
        }
    }
    return bashing;
}

/**
 * Ausgliederung für rekursiven Aufruf nach schlagendem Teilzug
 * TODO: Test ob zyklische Züge mit der einfachen Flag f_would_be_empty korrekt berechnet werden
 */
int check_bashing(field court[COURT_SIZE][COURT_SIZE],int max_size, direction dir, int i_field, int j_field, move_value mv[4], int index, char my_color, char opponent_color){
    
    int next_i, next_j;
    // Finde das Feld hinter dem gegner
    field next = next_field(dir, court, i_field, j_field, max_size, &next_i, &next_j);
    
    // Falls ein Feld hinter dem Genger existiert
    if (next.field_id[0] !='x' ){
        
        //Prüfe ob Feld hinter Gegner Leer
        if(strstr(next.towers,"_") || next.f_would_be_empty == 1){
            strcat(mv[index].move_id, ":");
            strcat(mv[index].move_id ,next.field_id);
            
            //setze die beiden Felder (das Alte und geschlagene) auf leer
            next.f_would_be_empty = 1;
            court[i_field][j_field].f_would_be_empty = 1;
            
            // Bewerte zug
            mv[index].value +=  MOVE_BASHING + check_safe(court, max_size, next_i, next_j, opponent_color) + check_covered(court, max_size, next_i, next_j, my_color, dir);
            
            if (dir >= LOWER_LEFT) mv[index].value +=  MOVE_BACK;
            else mv[index].value += MOVE_FOR;
            
            //Suche und bewerte alle weiteren möglichen Teilzüge:
            //erstellt zunächst kopien des berechneten Zuges um später vergleichen zu können
            move_value mv_nxt[4];
            for(int i = 0 ; i < 4; i++){
                strcpy(mv_nxt[i].move_id ,mv[index].move_id);
                mv_nxt[i].value = mv[index].value;
            }
            // index für die neuen Züge
            int index_rec = 0;
          
            for(direction dir = UPPER_LEFT ; dir <= LOWER_RIGHT ; dir++){
                int next_i, next_j;
                
                field next_rec = next_field(dir, court, i_field, j_field, max_size, &next_i, &next_j);

                // return 0 if one can bash ur move
                if (next_rec.field_id[0] !='x'){
                        //Prüfe ob ein gegnerischer Turm im feld steht und noch nicht in einem vorherigem Zug geschlagen worden wäre
                        if(char_cmp_ignore_case(next_rec.towers[strlen(next_rec.towers)-1] , opponent_color)
                           && next_rec.f_would_be_empty != 1){
                            
                            //Rekursion zur Erstellung eines Mehrzügigen Spielzugs
                            check_bashing(court, max_size,dir, next_i, next_j, mv_nxt, index_rec, my_color , opponent_color);
                            index_rec++;
                        }
                    }

                
            }
            //prüfe den besten weiteren schlagenden Teilzug
            for(int i = 0; i < 4; i ++ ){
                if(mv_nxt[i].value > mv[index].value){
                    //setze den neuen Zug
                    mv[index] = mv_nxt[i];
                }
            }
            
            //setze die beiden Felder (das Alte und geschlagene) wieder auf nicht leer
            next.f_would_be_empty = 1;
            court[i_field][j_field].f_would_be_empty = 1;
            return 1;
        }
    }
    
    return 0;
}


//Check if oponent player is in range to bash your tower
int check_safe(field court[COURT_SIZE][COURT_SIZE],int max_size_court, int i_field, int j_field, char opponent_color){

    for(direction dir = UPPER_LEFT ; dir <= LOWER_RIGHT ; dir++){
        int next_i, next_j;
        field next = next_field(dir, court, i_field, j_field, max_size_court, &next_i, &next_j);
        
            // return 0 if one can bash ur move
            if (next.field_id[0] !='x'){
                if(next.towers[strlen(next.towers)] == opponent_color)
                    return 0;
            }
        
    }
    // MOVE_SAFE
    return MOVE_SAFE;
}

//Check if new Position would be covered by other tower
int check_covered(field court[COURT_SIZE][COURT_SIZE] ,int max_size_court,  int i_field, int j_field, char my_color, direction dir_move){

    int covered = 0;

    for(direction dir = UPPER_LEFT ; dir <= LOWER_RIGHT ; dir++){
        if(dir == dir_move) continue;
        
        int next_i, next_j;
        field next = next_field(dir, court, i_field, j_field, max_size_court, &next_i, &next_j);
        
            // return 0 if one can bash ur move
        if(next.field_id[0] !='x'){
            if(next.towers[strlen(next.towers)-1] == my_color)
                covered += MOVE_COVERED;
            
        }
    }

    return covered;

}

/*
 * Findet das Nächste beziehbare Feld in Richtung dir, in new_i, new_j werden die neuen indizes gespeichert
 */
field next_field(direction dir , field court[COURT_SIZE][COURT_SIZE], int i, int j, int max_size, int *new_i, int *new_j){
    if (i < max_size && j < max_size && i >= 0 && j >= 0 ){
        switch (dir) {
            case UPPER_LEFT:
                *new_i = i-1; *new_j = j-1;
            case UPPER_RIGHT:
                *new_i = i-1; *new_j = j+1;
            case LOWER_LEFT:
                *new_i = i+1; *new_j = j-1;
            default:
                *new_i = i+1; *new_j = j+1;
            }
        
        return court[*new_i][*new_j];
    }else{
        field empty_field;
        empty_field.field_id[0] = 'x';
        return empty_field;
    }
}


bool char_cmp_ignore_case(char char_1 , char char_2){
    return (char_1 == char_2|| char_1  == char_2+32 || char_1 == char_2 -32);
}
