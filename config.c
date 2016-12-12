#include "config.h"
#include <errno.h>
#define ZEICHEN 50
#define ZEILEN 3


// zum erweitern um weitere parameter den Wert ZEILEN erhöhen und unten die entsprechenden Variablen einfügen

config _config;

void openconfig(char *filename) {
        char *_filename = filename;
        FILE *file;

        char line [ZEILEN][ZEICHEN];
        file = fopen(_filename, "r");



        if(file == NULL) {
            printf("Konfigurationsdatei konnte nicht geöffnet werden.\n");
            _config.hostname = "sysprak.priv.lab.nm.ifi.lmu.de";
            _config.portnumber = 1357;
            _config.gamekindname = "Bashni";
            
        }else {
          // File Zeilenweise einlesen
                int i= 0;
                while (fgets(line[i], ZEICHEN, file)) {
          // /n wegschmeißen
                    line[i][strlen(line[i]) - 1] = '\0';
                  i++;
                }

                fclose(file);
            for (int i = 0; i < 3; i++) {
                removeSpaces(line[i]);
            }
            
            // hier entsprechend um weitere parameter erweitern:
            
            char *_hostname = malloc(sizeof(char)*strlen(line[0]));
            strcpy(_hostname, line[0]);
            getmethevalue(&_hostname);
           
            char *_portnumber = malloc(sizeof(char)*strlen(line[1]));
            strcpy(_portnumber, line[1]);
            getmethevalue(&_portnumber);
           
            char *_gamekindname =malloc(sizeof(char)*strlen(line[2]));
            strcpy(_gamekindname , line[2]);
            
            getmethevalue(&_gamekindname);
        
           
            _config.portnumber = atoi(_portnumber);
            _config.gamekindname = _gamekindname;
            _config.hostname= _hostname;
           // free(_portnumber);
           // free(_gamekindname);
            printf("hostname: %s \n" ,_config.hostname);
        }




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
  char *lastToken = NULL;
  while (token != NULL)  {
    lastToken = token ;
    token = strtok (NULL, "=");
  }
  *string = lastToken;
}
