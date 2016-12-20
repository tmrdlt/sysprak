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
int fd;



//int main(int argc, char *argv[]) {
//    test_thinker();
//}

void think(){

    printf("Signal received\n");
    write(fd, "A3:B4", strlen("A3:B4"));

    printf("Move send\n");

    int ret_code = wait(NULL);
    if (ret_code < 0) {
        perror ("Fehler beim Warten in think().");
        exit(EXIT_FAILURE);
      }

//    game_state *_game_state = address_shm(id_seg_gameparams);
//
//    if(_game_state->flag_thinking){
//
//        //think_nxt_move(_game_state->court, 3000, 8);
//    }else{
//        perror("Thinkanstoß aber Connector Flag nicht gesetzt! \n");
//        //... do something
//    }
}

void think_nxt_move(field **court, int time_in_s, int max_size, char my_color){

  //  print_court(court, max_size);

    char move[5];



    if(my_color == 'w'){
        think_nxt_move_white(court, time_in_s, max_size, my_color, move);
    }else{
        think_nxt_move_black(court, time_in_s, max_size, my_color, move);
        //printf("Error My Color must be w or b ... not %c \n" , my_color );
    }

    printf("MOVE: %s\n",move);
}



void think_nxt_move_white(field **court , int time_in_s, int max_size,char my_color, char *move){

    bool best_move_found = false;

    char *old_field ="00"; char *new_field="00";

    for(int i = max_size-1 ; i >= 0 ; i --){

        for(int j = 0 ; j < max_size ; j ++){



            int tmp=(int) strlen(court[i][j].towers)-1;
            if(char_cmp_ignore_case(court[i][j].towers[tmp] , my_color)){
               // printf("%c is mycolor %c\n" ,court[i][j].towers[tmp],my_color);
                // check "links" nach "vorne"

                new_field = check_lower_left(court, max_size, i, j);

                if(new_field){
                    old_field = court[i][j].field_id;
                }

                // check "rechts" nach "vorne"
                else if(i < max_size-1 && j < max_size-1){
                    if(strstr(court[i+1][j+1].towers,"_")){
                        if (!strcmp(old_field , "00")){
                            printf("tmp match \n");

                            old_field = court[i][j].field_id;
                            new_field= court[i+1][j+1].field_id;
                        }
                    }else if(char_cmp_ignore_case(court[i+1][j+1].towers[strlen(court[i+1][j+1].towers)-1] , 'b')
                             && i+2 < max_size && j+2 < max_size){
                        if(strstr(court[i+2][j+2].towers,"_")){
                            printf("schlage b! \n");

                            old_field = court[i][j].field_id;
                            new_field= court[i+2][j+2].field_id;

                            best_move_found = true;
                            break;
                        }
                    }

                }
                // check "links" nach "hinten"
                else if (i > 0 && j > 0){
                    if(strstr(court[i-1][j-1].towers,"_")){
                        if (!strcmp(old_field , "00")){
                            printf("tmp match \n");

                            old_field= court[i][j].field_id;
                            new_field= court[i-1][j-1].field_id;
                        }
                    }else if(char_cmp_ignore_case(court[i-1][j-1].towers[strlen(court[i-1][j-1].towers)-1] , 'b')
                             && i+2 >= 0 &&j-2 >= 0 ){
                        if(strstr(court[i-2][j-2].towers,"_")){
                            printf("schlage b! \n");
                            old_field = court[i][j].field_id;
                            new_field= court[i-2][j-2].field_id;

                            best_move_found = true;
                            break;
                        }
                    }


                }
                // check "rechts" nach "hinten"
                else if(i>0 && j < max_size-1){
                    if(strstr(court[i-1][j+1].towers,"_")){
                        if (!strcmp(old_field , "00")){
                            printf("tmp match \n");
                            old_field= court[i][j].field_id;
                            new_field= court[i-1][j+1].field_id;
                        }

                    }else if(char_cmp_ignore_case(court[i-1][j+1].towers[strlen(court[i-1][j+1].towers)-1] , 'b')
                             && i+2 >= 0 && j+2 < max_size){
                        if(strstr(court[i-2][j+2].towers,"_")){
                            printf("schlage b! \n");
                            old_field = court[i][j].field_id;
                            new_field= court[i-2][j+2].field_id;

                            best_move_found = true;
                            break;
                        }
                    }
                }


            }
        }
        if (best_move_found)break;
    }

    //  if (best_move_found)break;
    build_move(old_field, new_field, move);
}

char *check_lower_right(field **court,int max_size, int i, int j){
    char *result = NULL;
    if (i < max_size-1 && j > 0){
        if(strstr(court[i+1][j-1].towers,"_")){
                printf("tmp match \n");
                result= court[i+1][j-1].field_id;

            }
        }else if(char_cmp_ignore_case(court[i+1][j-1].towers[strlen(court[i+1][j-1].towers)-1] , 'b')
                 && i+2 < max_size && j-2 >= 0){
            if(strstr(court[i+2][j-2].towers,"_")){
                printf("schlage b! \n");
                result= court[i+2][j-2].field_id;

            }
        }
    return result;
}

char *check_lower_left(field **court,int max_size_court, int i, int j){
    return"";
}

char *check_upper_right(field **court,int max_size_court, int i, int j){
    return"";
}

char *check_upper_left(field **court,int max_size_court, int i, int j){
    return"";
}


void think_nxt_move_black(field **court , int time_in_s, int max_size_court,char my_color, char *move){

}




void build_move(char *old_pos, char *new_pos, char *result){
    char tmp [strlen(old_pos) + strlen(new_pos) +1];
    strcpy(tmp, old_pos);
    strcat(tmp, ":");
    strcat(tmp, new_pos);
    strcpy(result,tmp);
}

bool char_cmp_ignore_case(char char_1 , char char_2){
    return (char_1 == char_2 || char_1  == char_2+32 || char_1 == char_2 -32);
}
