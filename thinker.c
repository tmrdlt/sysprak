//
//  thinker.c
//  bashni
//
//  Created by Holzner on 07.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#include "thinker.h"
#include <stdbool.h>
//
//int main(int argc, char *argv[]) {
//    test_thinker();
//}

void think(int id_seg_gameparams){
    game_state *_game_state = address_shm(id_seg_gameparams);
    
    if(_game_state->flag_thinking){
        //think_nxt_move(_game_state->court, 3000, 8);
    }else{
        perror("Thinkanstoß aber Connector Flag nicht gesetzt! \n");
        //... do something
    }
}

char *think_nxt_move(char court[8][8], int time_in_s, int max_size, char my_color){
    
    print_court(court, 8);
    
    int _timer;
    
    bool best_move_found = false;
    
    int i_tmp = -1;
    int i_old =-1, j_old= -1;
    int j_tmp = -1;
    
    while(_timer < time_in_s && !best_move_found){
        
        if(my_color == 'w'){
         
            for(int i = max_size-1 ; i >= 0 ; i --){
                
                for(int j = 0 ; j < max_size ; j ++){
                    
                    
                    if(court[i][j] == my_color){
                        
                        if (i < max_size-1 && j > 0){
                            if(court[i+1][j-1] == '.'){
                                if (i_tmp == -1){
                                    printf("tmp match \n");
                                    i_tmp = i+1;
                                    j_tmp = j-1;
                                    i_old = i;
                                    j_old = j;
                                }else if(court[i+1][j-1] == 'b' && i+2 >= 0 && j-2 < max_size-1){
                                    if(court[i+2][j-2] == '.'){
                                        printf("schlage b! \n");
                                        i_tmp = i+2;
                                        j_tmp = j-2;
                                        i_old = i;
                                        j_old = j;
                                        best_move_found = true;
                                        break;
                                    }
                                }
                            }
                        }else if(i < max_size-1 && j < max_size-1){
                            if(court[i+1][j+1] == '.'){
                                printf("tmp match \n");
                                if (i_tmp == -1){
                                    i_tmp = i+1;
                                    j_tmp = j+1;
                                    i_old = i;
                                    j_old = j;
                                }
                            }else if(court[i+1][j+1] == 'b' && i+2 < max_size-1 && j+2 < max_size-1){
                                if(court[i+2][j+2] == '.'){
                                    printf("schlage b! \n");
                                    i_tmp = i+2;
                                    j_tmp = j+2;
                                    i_old = i;
                                    j_old = j;
                                    best_move_found = true;
                                    break;
                                }
                            }
                        }
                        
                    }
                }
                if (best_move_found)break;
            }
            
        } else if(my_color == 'b'){
            
            for(int i = 0 ; i < max_size ; i ++){
                
                for(int j = 0 ; j < max_size ; j ++){
                    
                    
                    if(court[i][j] == my_color){
                        
                        if (i > 0 && j > 0){
                            if(court[i-1][j-1] == '.'){
                                if (i_tmp == -1){
                                    printf("tmp match \n");
                                    i_tmp = i-1;
                                    j_tmp = j-1;
                                    i_old = i;
                                    j_old = j;
                                }else if(court[i-1][j-1] == 'b' && i-2 >= 0 && j-2 < max_size-1){
                                    if(court[i+2][j-2] == '.'){
                                        printf("schlage b! \n");
                                        i_tmp = i-2;
                                        j_tmp = j-2;
                                        i_old = i;
                                        j_old = j;
                                        best_move_found = true;
                                        break;
                                    }
                                }
                            }
                        }else if(i > 0 && j < max_size-1){
                            if(court[i-1][j+1] == '.'){
                                printf("tmp match \n");
                                if (i_tmp == -1){
                                    i_tmp = i-1;
                                    j_tmp = j+1;
                                    i_old = i;
                                    j_old = j;
                                }
                            }else if(court[i-1][j+1] == 'w' && i-2 >= 0 && j+2 < max_size-1){
                                if(court[i-2][j+2] == '.'){
                                    printf("schlage b! \n");
                                    i_tmp = i-2;
                                    j_tmp = j+2;
                                    i_old = i;
                                    j_old = j;
                                    best_move_found = true;
                                    break;
                                }
                            }
                        }
                        
                        
                        
                    }
                }
                if (best_move_found)break;
            }
        }else{
            printf("Error My Color must be w or b ... not %c \n" , my_color );
        }
        best_move_found = true;
    }
    
    printf("MOVE: (%i,%i) -> (%i,%i)\n", i_old,j_old,i_tmp,j_tmp);
    
    return "asfa";
}


void test_thinker(){
    
    char court[8][8];
    
    int start = 0;
    
    for(int i = 0 ; i < 8 ; i++){
        int next = start;
        for (int j = 0 ; j < 8 ; j ++){
            
            if(i < 3 && j == next){
                court[i][j] = 'w';
                next += 2;
                
            } else if(i > 4 && j == next){
                court[i][j] = 'b';
                next += 2;
            } else if(j == next){
                court[i][j] = '.';
                next += 2;
            } else{
                court[i][j] = '-';
            }
            
        }
        if (start == 0){
            start = 1;
        }else{
            start = 0;
        }
        
    }
    think_nxt_move(court, 2000, 8 ,'b');
}

void print_court(char court[8][8], int size){
    for(int i = -1 ; i < 8 ; i++){
        if(i != -1)
            printf("%d| ",i);
        else
            printf("   ");
        for (int j = 0 ; j < 8 ; j ++){
            if(i == -1){
                printf("%d ",j);
                
            }else{
                printf("%c", court[i][j]);
                if(j < size-1){
                    printf(" ");
                }
            }
        }
        printf(" |\n");
    }
}


