
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define COURT_SIZE 8

typedef struct {
        char *field_id;
        char *towers;
} field;


void int_to_string(int _int,  char*dest){
        char str[15];
        sprintf(str, "%d", _int);
        strcpy(dest, str);

}

void set_court(field spielfeld[COURT_SIZE][COURT_SIZE], int size) {
        for (int i = 0; i < size; i++) {

                for (int j = 0; j < size; j++) {
                        char feld[3];
                        feld[0] = j+65;
                        feld[1] = 56-i;
                        feld[2] = '\0';

                        spielfeld[i][j].field_id = feld;
                        printf("%s\n", spielfeld[i][j].field_id);
                        //  int int_field_id = 8-i;
                        //  char _int_field_id[1];
                        //  int_to_string(int_field_id, _int_field_id);

                        //  char char_field_id = j+65;
                        //  char * _field_id = &char_field_id;
                        //  strcat(_field_id, _int_field_id);
                        //  printf("%s\n", _field_id);
                }
        }

        for(int i=0; i<(size/2); i++) {
                for(int j=0; j<(size/2); j++) {
                        spielfeld[2*i][2*j].towers = ".";
                        spielfeld[2*i][2*j+1].towers = "_";
                        spielfeld[2*i+1][(2*j)+1].towers = ".";
                        spielfeld[2*i+1][2*j].towers = "_";
                }
        }

}



void set_draft(field spielfeld[COURT_SIZE][COURT_SIZE], int size, char* draft) {
        int i = 56 - draft[3];
        int j = draft[2] - 65;
        char tmp[2] = {draft[0], '\0'};
        printf("%d, %d\n", i,j );



       spielfeld[i][j].towers = tmp;
       printf("%s\n", spielfeld[i][j].towers );




}


void print_court(field spielfeld[COURT_SIZE][COURT_SIZE], int size) {
        printf("   A B C D E F G H\n");
        printf("  +---------------+\n");
        for(int i=0; i<size; i++) {
                printf("%d| ", 8-i);
                for(int j=0; j<size; j++) {
                        printf("%s ", spielfeld[i][j].towers);

                }
                printf("|%d\n", 8-i);


        }
        printf("  +---------------+\n");
        printf("   A B C D E F G H\n");

}


int main() {


        field spielfeld[COURT_SIZE][COURT_SIZE];

        set_court(spielfeld, COURT_SIZE);
        char draft[] = "W@B6";
        char draft1[] = "b@F8";
        char draft2[] = "B@D8";
        char draft3[] = "w@B8";
        set_draft(spielfeld, COURT_SIZE, draft);
        print_court(spielfeld, COURT_SIZE);



                        //      printf("%s\n", spielfeld[i][j].field_id);




}
