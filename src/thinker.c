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
    for(int i = 0 ; i < max_size; i ++){
        for(int j = 0 ; j < max_size; j ++){

            if(char_cmp_ignore_case(court[i][j].towers[strlen(court[i][j].towers)-1], my_color)){

                if(tower_is_dame(court[i][j].towers[strlen(court[i][j].towers)-1])){

                     //printf("Is dame: %s!\n", court[i][j].field_id );

                    for(direction dir = UPPER_LEFT ; dir <= LOWER_RIGHT ; dir++){
                        move_value mv;
                        mv.value = MOVE_ILLEGAL;
                        strcpy(mv.move_id, court[i][j].field_id);
                        
                        field tmp_court[COURT_SIZE][COURT_SIZE];
                        copy_court(tmp_court, court);
                      
                        check_dame(tmp_court, max_size, dir, i, j, &mv,  my_color, opponent_color, 0);
                        
                        if(mv.value > MOVE_ILLEGAL){
                            //printf("Move added in stash: %s , (%d)\n" , mv.move_id , mv.value);
                            rated_moves[count_legal_moves++] = mv;
                        }
                    }


                }else{

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
            }

            passed_time += 10; // TODO Update
        }
    }

    move_value best_move;
    best_move.value = MOVE_ILLEGAL;

    //find best rated move
    for(int i = 0 ; i < count_legal_moves && passed_time < allowed_time; i ++){
        if(rated_moves[i].value > best_move.value){
           // printf("%d, better than old(%d)\n" ,rated_moves[i].value,  best_move.value);
            best_move = rated_moves[i];

        } else if(rated_moves[i].value == best_move.value){
            if(randomize_even_drafts()){
                best_move = rated_moves[i];
            }   
        }
    }

    // send best move to connector
    //printf("Next Move Found: %s Rating: %d\n" , best_move.move_id, best_move.value);
    strcat(best_move.move_id, "\0");
    ssize_t bytes_wrote = write(fd, best_move.move_id, strlen(best_move.move_id));

    if(bytes_wrote < 0){
        perror ("Fehler beim schreiben in pipe.");
        exit(EXIT_FAILURE);
    }

}


/*
 * Damen Zug rekusive Iteration diagonal in eine Richtung über das Spielfeld
 */
int check_dame(field court[COURT_SIZE][COURT_SIZE],int max_size, direction dir, int i_feld, int j_feld, move_value *mv, char my_color, char opponent_color, int must_bash){

    int next_i, next_j;
    field *next = next_field(dir, court, i_feld, j_feld, max_size, &next_i, &next_j);


    if (next != NULL){


        if(strstr(next->towers,"_")){
            
          // printf("Field is empty: %s\n", next->field_id);

            move_value rate_nxt_move;

            strcpy(rate_nxt_move.move_id, mv->move_id);
            rate_nxt_move.value = mv->value;
            
            if(must_bash != 1){
                
                
                //printf("Draft must not bash\n");

                //bewerte möglichen Zug
                mv->value += check_safe(court, max_size, next_i, next_j, opponent_color) + check_covered(court, max_size, next_i, next_j, my_color, dir);

                //Damen zug nach hinten ist genauso wertvoll wie nach vorne
                if (dir >= LOWER_LEFT) mv->value +=  MOVE_FOR;
                else mv->value += MOVE_FOR;
                strcat(mv->move_id, ":");
                strcat(mv->move_id, next->field_id);
                strcat(mv->move_id,"\0");
            }
            
            field tmp_court[COURT_SIZE][COURT_SIZE];
            copy_court(tmp_court, court);

            //prüfe weiter in selber Richtung
            check_dame(tmp_court, max_size, dir, next_i, next_j, &rate_nxt_move,  my_color, opponent_color, must_bash);

            //teste ob Zug ins nächste Feld besser bewertet wäre
            if(rate_nxt_move.value > mv->value){
                //printf("New better Draft found: %s (%d) Old: %s (%d) \n" , rate_nxt_move.move_id , rate_nxt_move.value , mv->move_id , mv->value);
                mv->value = rate_nxt_move.value;
                strcpy(mv->move_id, rate_nxt_move.move_id);
            } else if(rate_nxt_move.value == mv->value){
                if(randomize_even_drafts()){
                    mv->value = rate_nxt_move.value;
                }
            }

        }else if(char_cmp_ignore_case(next->towers[strlen(next->towers)-1] , opponent_color) ){
            
            
          // printf("Enemy in field: %s\n", next->field_id);

            //find field behind
            int i_behind = next_i, j_behind = next_j;
            field *field_behind;
            
            move_value tmp_mv_result;
            tmp_mv_result.value = mv->value;
            strcpy(tmp_mv_result.move_id, mv->move_id);

            while ((field_behind = next_field(dir, court, i_behind, j_behind, max_size, &i_behind, &j_behind)) != NULL) {
                
                if(!strstr(field_behind->towers,"_")){
                    
                    // printf("Field behind enemy is not empty: %s\n", field_behind->field_id);
                    
                    break;

                }else{
                    
                    move_value tmp_mv;
                    tmp_mv.value = mv->value;
                    strcpy(tmp_mv.move_id, mv->move_id);
                    
                   // printf("Field behind enemy is empty: %s\n", field_behind->field_id);
                
                    //build move id
                    strcat(tmp_mv.move_id, ":");
                    strcat(tmp_mv.move_id ,field_behind->field_id);
                    strcat(tmp_mv.move_id, "\0");

                    //tmp clean Feld des Gegners und altes Feld
                    if(strlen(court[next_i][next_j].towers) > 1){
                        //befreien eines eigenen Steines
                        court[next_i][next_j].towers[strlen(court[next_i][next_j].towers)-1] = '\0';
                    }else{
                        court[next_i][next_j].towers[0] = '_';
                        court[next_i][next_j].towers[1] = '\0';
                    }
                    
                    // altes feld räumen
                    court[i_feld][j_feld].towers[0] = '_';
                    court[i_feld][j_feld].towers[1] = '\0';
                    //Rate
                    //schlagender Teilzug
                    tmp_mv.value += MOVE_BASHING;

                    //Damen zug nach hinten ist genauso wertvoll wie nach vorne
                    if (dir >= LOWER_LEFT) tmp_mv.value +=  MOVE_FOR;
                    else tmp_mv.value += MOVE_FOR;

                    //rate
                    tmp_mv.value += check_safe(court, max_size, i_behind, j_behind, opponent_color) + check_covered(court, max_size, i_behind, j_behind , my_color, dir);
                    
                    //rekursiver move
                    move_value tmp_mv_rec;
                    tmp_mv_rec.value = tmp_mv.value;
                    strcpy(tmp_mv_rec.move_id, tmp_mv.move_id);

                    

                    //rec check 4 directions
                    for(direction dir_rec = UPPER_LEFT ; dir_rec <= LOWER_RIGHT ; dir_rec++){
                        
                        if(dir_rec == revers_dir(dir)){
                            continue;
                        }

                        //printf("Check Direction: %d for further drafts\n", dir_rec);
                        
                        field tmp_court[COURT_SIZE][COURT_SIZE];
                        copy_court(tmp_court, court);
                        
                        check_dame(tmp_court, max_size, dir_rec, i_behind, j_behind, &tmp_mv_rec,  my_color, opponent_color, 1);

                        if(tmp_mv_rec.value > tmp_mv_result.value){
                            
                           // printf("New draft: %s (%d),  Old Draft: %s (%d) \n", tmp_mv_rec.move_id , tmp_mv_rec.value, tmp_mv.move_id, tmp_mv.value);
                            tmp_mv_result.value = tmp_mv_rec.value;
                            strcpy(tmp_mv_result.move_id, tmp_mv_rec.move_id);
                        } else if(tmp_mv_rec.value == tmp_mv_result.value){
                            if(randomize_even_drafts()){
                                tmp_mv_result.value = tmp_mv_rec.value;
                                strcpy(tmp_mv_result.move_id, tmp_mv_rec.move_id);
                            }
                        }
                        
                        tmp_mv_rec.value = tmp_mv.value;
                        strcpy(tmp_mv_rec.move_id, tmp_mv.move_id);
                    }

                    
                    
                }
            }
            

            if(tmp_mv_result.value > mv->value){
                mv->value = tmp_mv_result.value;
                strcpy(mv->move_id, tmp_mv_result.move_id);
             //   printf("Best Dame draft = %s (%d)\n ", mv->move_id , mv->value);
            }  else if(tmp_mv_result.value == mv->value){
                if(randomize_even_drafts()){
                    mv->value = tmp_mv_result.value;
                    strcpy(mv->move_id, tmp_mv_result.move_id);
                }
            }

            return 1;
        }

        return 0;

    } else{
        mv->value = MOVE_ILLEGAL;
        return -1;
    }

}

//Bewerte alle Möglichkeiten eines Feldes - return 1 wenn schlagender möglich
int check_field(field court[COURT_SIZE][COURT_SIZE],int max_size, int i_feld, int j_feld, char my_color, char opponent_color, move_value mv[4], int must_bash){
    int bashing = 0;
    int index = 0;


    for(direction dir = UPPER_LEFT ; dir <= LOWER_RIGHT ; dir++){


        int next_i, next_j;
        field *next = next_field(dir, court, i_feld, j_feld, max_size, &next_i, &next_j);

            // prüfe ob index überläuft
            if (next != NULL){

                // Wenn Feld leer (beziehbar)
                if(strstr(next->towers,"_") && (must_bash != 1)){
                    //Zug bauen
                    strcat(mv[index].move_id, ":");
                    strcat(mv[index].move_id, next->field_id);
                   // strcat(mv[index].move_id, "\0");


                     //Zug bewerten
                    mv[index].value += check_safe(court, max_size, next_i, next_j, opponent_color) + check_covered(court, max_size, next_i, next_j , my_color, dir);

                    if (dir >= LOWER_LEFT) mv[index].value =MOVE_ILLEGAL;
                    else mv[index].value += MOVE_FOR;

                    // Wenn Gegner im Feld
                }else if(char_cmp_ignore_case(next->towers[strlen(next->towers)-1] , opponent_color)){
                    field tmp_court[COURT_SIZE][COURT_SIZE];
                    copy_court(tmp_court, court);

                    bashing = check_bashing(tmp_court, max_size, dir, next_i, next_j, mv, index, my_color , opponent_color);

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
 */
int check_bashing(field court[COURT_SIZE][COURT_SIZE],int max_size, direction dir, int i_field, int j_field, move_value mv[4], int index, char my_color, char opponent_color){
    
    int next_i, next_j;

    // Finde das Feld hinter dem gegner
    field *next = next_field(dir, court, i_field, j_field, max_size, &next_i, &next_j);

    // Falls ein Feld hinter dem Genger existiert
    if (next != NULL ){

        //Prüfe ob Feld hinter Gegner Leer
        if(strstr(next->towers,"_") ){

            strcat(mv[index].move_id, ":");
            strcat(mv[index].move_id ,next->field_id);
            strcat(mv[index].move_id, "\0");

//            printf("Move clauclated: %s\n" , mv[index].move_id);

  //          printf("Would bash: %s \n" , mv[index].move_id);
           
            //tmp clean Feld des Gegners und altes Feld
            if(strlen(court[next_i][next_j].towers) > 1){
                //befreien eines eigenen Steines
                court[next_i][next_j].towers[strlen(court[next_i][next_j].towers)-1] = '\0';
            }else{
                court[next_i][next_j].towers[0] = '_';
                court[next_i][next_j].towers[1] = '\0';
            }
            
            // altes feld räumen
            court[i_field][j_field].towers[0] = '_';
            court[i_field][j_field].towers[1] = '\0';

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

            for(direction dir_rec = UPPER_LEFT ; dir_rec <= LOWER_RIGHT ; dir_rec++){
                
                if(dir_rec == revers_dir(dir)){
                    continue;
                }
                
                int next_i_rec, next_j_rec;

                field *next_rec = next_field(dir_rec, court, next_i, next_j, max_size, &next_i_rec, &next_j_rec);


                // return 0 if one can bash ur move
                if (next_rec != NULL){
                        //Prüfe ob ein gegnerischer Turm im feld steht und noch nicht in einem vorherigem Zug geschlagen worden wäre
                        if(char_cmp_ignore_case(next_rec->towers[strlen(next_rec->towers)-1] , opponent_color)){

                            field tmp_court[COURT_SIZE][COURT_SIZE];
                            copy_court(tmp_court, court);
                            //Rekursion zur Erstellung eines Mehrzügigen Spielzugs
                            check_bashing(tmp_court, max_size, dir_rec, next_i_rec, next_j_rec, mv_nxt, index_rec, my_color , opponent_color);
                            index_rec++;
                        }
                    }


            }
            //prüfe den besten weiteren schlagenden Teilzug
            for(int i = 0; i < 4; i ++ ){
                if(mv_nxt[i].value > mv[index].value){
                    //setze den neuen Zug
                    mv[index] = mv_nxt[i];
                } else if(mv_nxt[i].value == mv[index].value){
                    if(randomize_even_drafts()){
                        mv[index] = mv_nxt[i];
                    }
                }
            }

            return 1;
        }
    }

    return 0;
}


//Check if oponent player is in range to bash your tower
int check_safe(field court[COURT_SIZE][COURT_SIZE],int max_size_court, int i_field, int j_field, char opponent_color){

    for(direction dir = UPPER_LEFT ; dir <= LOWER_RIGHT ; dir++){
        int next_i, next_j;
        field *next = next_field(dir, court, i_field, j_field, max_size_court, &next_i, &next_j);

            // return 0 if one can bash ur move
            if (next != NULL){
                if(next->towers[strlen(next->towers)-1] == opponent_color)
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
        field *next = next_field(dir, court, i_field, j_field, max_size_court, &next_i, &next_j);

            // return 0 if one can bash ur move
        if(next != NULL){
            if(next->towers[strlen(next->towers)-1] == my_color)
                covered += MOVE_COVERED;

        }
    }

    return covered;

}

/*
 * Findet das Nächste beziehbare Feld in Richtung dir, in new_i, new_j werden die neuen indizes gespeichert
 */
field* next_field(direction dir , field court[COURT_SIZE][COURT_SIZE], int i, int j, int max_size, int *new_i, int *new_j){
    field *result;



        switch (dir) {
            case UPPER_LEFT:
                if(i > 0 && j > 0) {
                    *new_i = i-1; *new_j = j-1;
                    result = &court[*new_i][*new_j];
                }else{
                    return NULL;
                }
                break;
            case UPPER_RIGHT:
                if(i > 0 &&  j < max_size-1 ) {
                    *new_i = i-1; *new_j = j+1;
                    result = &court[*new_i][*new_j];
                }else{
                    return NULL;
                }
                break;
            case LOWER_LEFT:
                if(i < max_size-1&& j > 0) {
                    *new_i = i+1; *new_j = j-1;
                    result = &court[*new_i][*new_j];
                }else{
                   return NULL;
                }
                break;
            default:
                if(i < max_size-1 && j < max_size-1) {
                    *new_i = i+1; *new_j = j+1;
                    result = &court[*new_i][*new_j];
                }else{
                   return NULL;
                }
                break;
            }

        return result;
}

bool tower_is_dame(char tower){
    return (tower == 'B' || tower == 'W');
}


bool char_cmp_ignore_case(char char_1 , char char_2){
    return (char_1 == char_2|| char_1  == char_2+32 || char_1 == char_2 -32);
}

void copy_court(field dest[COURT_SIZE][COURT_SIZE] , field src[COURT_SIZE][COURT_SIZE]){
    for(int i = 0 ; i < COURT_SIZE ; i++ ){
        for (int j = 0; j< COURT_SIZE; j++) {
            strcpy(dest[i][j].field_id , src[i][j].field_id);
            strcpy(dest[i][j].towers , src[i][j].towers);
        }
    }
}


direction revers_dir(direction dir){
    switch (dir) {
        case UPPER_LEFT:
            return LOWER_RIGHT;
        case UPPER_RIGHT:
            return LOWER_LEFT;
        case LOWER_LEFT:
            return UPPER_RIGHT;
        default:
            return UPPER_LEFT;
    }
}


bool randomize_even_drafts(){
    srand((unsigned)time(NULL));
    int r = rand() % 2;
    printf("Rand : %d\n" , r);
    return r == 0;
}






