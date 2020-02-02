#include "config.h"

config _config;

void openconfig(char *filename) {
    char *_filename = filename;
    FILE *file;
    char line [CONFIG_ZEILEN][CONFIG_ZEICHEN];
    file = fopen(_filename, "r");
    
    if(file == NULL) {
        perror("\n...Fehler in Konfigurationsdatei");
        strcpy(_config.hostname, STANDARD_HOSTNAME);
        _config.portnumber = STANDARD_PORTNUMBER;
        strcpy(_config.gamekindname, STANDARD_GAMEKINDNAME);
    }else {
        // File Zeilenweise einlesen
        int i= 0;
        while (fgets(line[i], CONFIG_ZEICHEN, file)) {
            // /n wegschmeißen
            line[i][strlen(line[i]) - 1] = '\0';
            i++;
        }
        fclose(file);
        for (int i = 0; i < 3; i++) {
            removeSpaces(line[i]);
        }
        
        // hier entsprechend um weitere parameter erweitern:
        strcpy(_config.hostname, line[0]);
        getmethevalue(_config.hostname);
        
        strcpy(_config.gamekindname, line[2]);
        getmethevalue(_config.gamekindname);
        
        char _portnumber[CONFIG_ZEICHEN];
        strcpy(_portnumber, line[1]);
        getmethevalue(_portnumber);
        _config.portnumber = atoi(_portnumber);
    }
}

// entfernt die leerzeichen aus den Strings
void removeSpaces(char* string) {
    char* i = string;
    char* j = string;
    while(*j != 0)
    {
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = 0;
}

// manipuliert einen string so, dass nur der teil nach dem "=" übrig bleibt.
void getmethevalue(char string[CONFIG_ZEICHEN]) {
    char *toget;
    char _toget[50];
    char *delimiter = "=";
    toget = strtok(string, delimiter);
    toget = strtok(NULL, delimiter);
    strcpy(_toget, toget);
    strcpy(string, _toget);
}
