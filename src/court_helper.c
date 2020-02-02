#include "court_helper.h"

#define UNICODE false //Variable, die steuert ob Spielfeld in Unicode zeichen gedruckt werden soll oder nicht (geht z.B. unter Mac nicht)

// struct field besetzen: mit den field_ids (A8, B8, usw.) und mit . _ . _ usw.
void set_court(field court[COURT_SIZE][COURT_SIZE], int size, int player_id) {

    // wenn der client weiß ist (player_id = 0):
    if(player_id == 0) {
        // field_ids setzen
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                char k = j+65; // 0 zu 'A', 1 zu 'B' usw.
                char l = 56-i; // 0 zu '8', 1 zu '7' usw.
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
        // wenn der client schwarz ist (player_id = 1):
    } else {
        // field_ids setzen
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                char k = 72-j; // 0 zu 'H',
                char l = 49+i; // 0 zu '1',
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
}


void set_draft(field court[COURT_SIZE][COURT_SIZE], char* draft) {

    char field_id_draft[3] = {draft[2], draft[3], '\0'};
    char zug[2] = {draft[0], '\0'};

    for (int i = 0; i < COURT_SIZE; i++) {
        for (int j = 0; j < COURT_SIZE; j++) {
            if (strcmp(court[i][j].field_id, field_id_draft) == 0) {
                if (court[i][j].towers[0] == '.'|| court[i][j].towers[0] == '_') {
                    //reinkopieren
                    strcpy(court[i][j].towers, zug);
                } else {
                    //anhängen
                    strcat(court[i][j].towers, zug);
                }
            }
        }
    }
}


void print_court(field court[COURT_SIZE][COURT_SIZE], int size, int player_id) {

    printf("   ╭─┬─┬─┬─┬─┬─┬─┬─╮\n");
    printf("   │A│B│C│D│E│F│G│H│\n");
    printf("╭─┬┴─┴─┴─┴─┴─┴─┴─┴─┴┬─╮\n");

    // wenn der client weiß ist (player_id = 0):
    if(player_id == 0) {
        for(int i=0; i<size; i++) {
            printf("│%d│ ", 8-i);
            for(int j=0; j<size; j++) {
                int len = (int)strlen(court[i][j].towers);
                if (UNICODE) {
                    print_unicode(court[i][j].towers[len-1]);
                } else {
                    printf("%c ", court[i][j].towers[len-1]);
                }
            }
            printf("│%d│\n", 8-i);
        }
        // wenn der client schwarz ist (player_id = 1):
    } else {
        for(int i=(size-1); i>=0; i--) {
            printf("│%d│ ", i);
            for(int j=(size-1); j>=0; j--) {
                int len = (int)strlen(court[i][j].towers);
                if (UNICODE) {
                    print_unicode(court[i][j].towers[len-1]);
                } else {
                    printf("%c ", court[i][j].towers[len-1]);
                }
            }
            printf("│%d│\n", i);
        }
    }
    printf("╰─┴┬─┬─┬─┬─┬─┬─┬─┬─┬┴─╯\n");
    printf("   │A│B│C│D│E│F│G│H│\n");
    printf("   ╰─┴─┴─┴─┴─┴─┴─┴─╯\n");

    printf("\nWhite Towers\n");
    printf("============\n");
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            int len = (int)strlen(court[i][j].towers);
            if (court[i][j].towers[len-1] == 'W' || court[i][j].towers[len-1] == 'w') {
                printf("%s: ", court[i][j].field_id);
                if (UNICODE == 1) {
                    for (int k = 0; k < len; k++) {
                        print_unicode(court[i][j].towers[k]);
                    }
                }else{
                    printf("%s",court[i][j].towers);
                }
                printf("\n");
            }
        }
    }

    printf("\nBlack Towers\n");
    printf("============\n");
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            int len = (int) strlen(court[i][j].towers);
            if (court[i][j].towers[len-1] == 'B' || court[i][j].towers[len-1] == 'b') {
                printf("%s: ", court[i][j].field_id);
                if (UNICODE == 1) {
                    for (int k = 0; k < len; k++) {
                        print_unicode(court[i][j].towers[k]);
                    }
                }else{
                    printf("%s",court[i][j].towers);
                }
                printf("\n");
            }
        }
    }
    printf("\n");
}


//Methode um chars in diesen Unicode zeichen auszugeben
void print_unicode(char _char) {
    switch (_char) {
        case 'w': printf("⛀ ");
            break;
        case 'W': printf("⛁ ");
            break;
        case 'b': printf("⛂ ");
            break;
        case 'B': printf("⛃ ");
            break;
        case '.': printf(". ");
            break;
        case '_': printf("_ ");
            break;
    }
}
