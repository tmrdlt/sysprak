#include "config.h"
#define ZEICHEN 50
#define ZEILEN 3

// zum erweitern um weitere parameter den Wert ZEILEN erhöhen und unten die entsprechenden Variablen einfügen

void openconfig(char *filename) {
        char *_filename = filename;
        FILE *file;
        char line [ZEILEN][ZEICHEN];
        file = fopen(_filename, "r");



        if(file == NULL) {
                printf("Konfigurationsdatei konnte nicht geöffnet werden.\n");
        }else {
          // File Zeilenweise einlesen
                int i;
                while (fgets(line[i], ZEICHEN, file)) {
          // /n wegschmeißen
                line[i][strlen(line[i]) - 1] = '\0';
                  i++;
                }

                fclose(file);
        }


for (int i = 0; i < 3; i++) {
   removeSpaces(line[i]);
}

// hier entsprechend um weitere parameter erweitern:

char *_hostname = line[0];
getmethevalue(&_hostname);
char *_portnumber = line[1];
getmethevalue(&_portnumber);
char *_gamekindname = line[2];
getmethevalue(&_gamekindname);

config.hostname = _hostname;
config.portnumber = atoi(_portnumber);
config.gamekindname = _gamekindname;

// TEST: printf("host: %s\nport: %d\nkind: %s\n", config.hostname, config.portnumber, config.gamekindname);



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
void getmethevalue(char ** string) {
  char *token;
  token = strtok (*string ,"=");
  char *lastToken;
  while (token != NULL)  {
    lastToken = token ;
    token = strtok (NULL, "=");
  }
  *string = lastToken;
}
