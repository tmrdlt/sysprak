
#include "court_helper.h"



// struct field besetzen: mit den field_ids (A8, B8, usw.) und mit . _ . _ usw.
void set_court(field court[COURT_SIZE][COURT_SIZE], int size) {

        // field_ids setzen
        for (int i=0; i<size; i++) {
                for (int j=0; j<size; j++) {
                        char k = j+65;  // 0 zu 'A', 1 zu 'B' usw.
                        char l = 56-i;  // 0 zu '8', 1 zu '7' usw.
                        char feld[3] = {k, l, '\0'};
                        strcpy(court[i][j].field_id, feld);
                }
        }
        // ._._ setzten
        for(int i=0; i<(size/2); i++) {
                for(int j=0; j<(size/2); j++) {
                        court[2*i][2*j].towers[0] = '.';
                        court[2*i][2*j].towers[1] = '\0';
                        court[2*i][2*j+1].towers[0] = '_';
                        court[2*i][2*j+1].towers[1] = '\0';
                        court[2*i+1][(2*j)+1].towers[0] = '.';
                        court[2*i+1][(2*j)+1].towers[1] = '\0';
                        court[2*i+1][2*j].towers[0] = '_';
                        court[2*i+1][2*j].towers[1] = '\0';
                }
        }

}



void set_draft(field court[COURT_SIZE][COURT_SIZE], char* draft) {
        int i = 56-draft[3];  // '8' zu 0, '7' zu 1, usw.
        int j = draft[2]-65;  // 'A' zu 0, 'B' zu 1, usw.
        char zug[2] = {draft[0], '\0'};

        // if noch kein stein gesetzt
        if (court[i][j].towers[0] == '.'|| court[i][j].towers[0] == '_') {
                //reinkopieren
                strcpy(court[i][j].towers, zug);
        } else {
                //anhängen
                strcat(court[i][j].towers, zug);
        }
}


void print_court(field court[COURT_SIZE][COURT_SIZE], int size) {

        printf("   A B C D E F G H\n");
        printf("  +---------------+\n");
        for(int i=0; i<size; i++) {
                printf("%d| ", 8-i);
                for(int j=0; j<size; j++) {
                        int len = (int)strlen(court[i][j].towers);
                        printf("%c ", court[i][j].towers[len-1]);

                }
                printf("|%d\n", 8-i);
        }
        printf("  +---------------+\n");
        printf("   A B C D E F G H\n");

        printf("\nWhite Towers\n");
        printf("============\n");
        for(int i=0; i<size; i++) {
                for(int j=0; j<size; j++) {
                        int len = (int)strlen(court[i][j].towers);
                        if (court[i][j].towers[len-1] == 'W' || court[i][j].towers[len-1] == 'w') {
                                printf("%s: %s\n", court[i][j].field_id, court[i][j].towers);
                        }
                }
        }

        printf("\nBlack Towers\n");
        printf("============\n");
        for(int i=0; i<size; i++) {
                for(int j=0; j<size; j++) {
                        int len = (int) strlen(court[i][j].towers);
                        if (court[i][j].towers[len-1] == 'B' || court[i][j].towers[len-1] == 'b') {
                                printf("%s: %s\n", court[i][j].field_id, court[i][j].towers);
                        }
                }
        }
}





int main2() {


        field court[COURT_SIZE][COURT_SIZE];

        set_court(court, COURT_SIZE);
        char draft[] = "W@B6";
        char draft1[] = "w@B6";
        char draft2[] = "B@D6";
        char draft3[] = "b@D6";
        set_draft(court, draft);
        set_draft(court, draft1);
        set_draft(court, draft2);
        set_draft(court, draft3);
        print_court(court, COURT_SIZE);






        //      printf("%s\n", court[i][j].field_id);

    return 1;

}
