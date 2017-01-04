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

//Bewerte alle Möglichkeiten eines Feldes - return 1 wenn schlagender möglich
int check_field(field court[COURT_SIZE][COURT_SIZE],int max_size, int i_feld, int j_feld, char my_color, char opponent_color, move_value mv[4], int must_bash){
    int bashing = 0;
    int index = 0;

    for(int n = i_feld -1 ; n < i_feld +2 ; n++){
        if(n == i_feld) continue; // die felder direkt neben, davor oder dahinter müssen nicht geprüft werden
        for(int k = j_feld-1 ; k < j_feld+2 ; k++){
            if(k == j_feld) continue; // die felder direkt neben, davor oder dahinter müssen nicht geprüft werden

            // prüfe ob index überläuft
            if (n < max_size && k < max_size && n >= 0 && k >= 0 ){

                // Wenn Feld leer (beziehbar)
                if(strstr(court[n][k].towers,"_") && (must_bash != 1)){
                    //Zug bauen
                    strcat(mv[index].move_id, ":");
                    strcat(mv[index].move_id ,court[n][k].field_id);

                     //Zug bewerten
                    mv[index].value += check_safe(court, max_size, n, k, opponent_color) + check_covered(court, max_size, n, k, my_color, i_feld, j_feld);
                    if (n > i_feld) mv[index].value =MOVE_ILLEGAL;
                    else mv[index].value += MOVE_FOR;

                    // Wenn Gegner im Feld
                }else if(char_cmp_ignore_case(court[n][k].towers[strlen(court[n][k].towers)-1] , opponent_color)){

                    bashing = check_bashing(court, max_size, n, k, i_feld, j_feld, mv, index, my_color , opponent_color);
                    
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

/**
 * Ausgliederung für rekursiven Aufruf nach schlagendem Teilzug
 * TODO: Ausstehend zyklische Spielzüge sind aktuell nicht möglich da die Türme (der ziehende und die geschlagenen) noch im alten Feld stehen
 */
int check_bashing(field court[COURT_SIZE][COURT_SIZE],int max_size, int n , int k , int i_feld, int j_feld, move_value mv[4], int index, char my_color, char opponent_color){
    
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
            
            // Bewerte zug
            mv[index].value +=  MOVE_BASHING + check_safe(court, max_size, next_i, next_j, opponent_color) + check_covered(court, max_size, next_i, next_j, my_color, n, k);
            if (n > i_feld ) mv[index].value +=  MOVE_BACK;
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
            //Iteriere über die 4 möglichen Felder
            for(int x = next_i -1 ; x < next_i +2 ; x++){
                if(x == next_i) continue; // die felder direkt neben, davor oder dahinter müssen nicht geprüft werden
                for(int y = next_j-1 ; y < next_j+2 ; y++){
                    if(y == next_j) continue; // die felder direkt neben, davor oder dahinter müssen nicht geprüft werden
                    //prüfe auf index überlauf
                    if (x < max_size && y < max_size && x >= 0 && y >= 0 ){
                        //Prüfe ob ein gegnerischer Turm im feld steht
                        if(char_cmp_ignore_case(court[x][y].towers[strlen(court[x][y].towers)-1] , opponent_color)){
                            
                            //Rekursion zur Erstellung eines Mehrzügigen Spielzugs
                            //180 Grad Spielzug muss momentan nicht geprüft werden, da im alten Feld der Turm noch steht
                            check_bashing(court, max_size, x, y, next_i, next_j, mv_nxt, index_rec, my_color , opponent_color);
                            index_rec++;
                        }
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
            
            return 1;
        }
    }
    
    return 0;
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
