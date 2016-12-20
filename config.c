#include "config.h"
#include <errno.h>

#define STANDARD_HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define STANDARD_GAMEKINDNAME "Bashni"
#define STANDARD_PORTNUMBER 1357

// zum erweitern um weitere parameter den Wert ZEILEN erhöhen und unten die entsprechenden Variablen einfügen

config _config;

void openconfig(char *filename) {
        char *_filename = filename;
        FILE *file;

        char line [CONFIG_ZEILEN][CONFIG_ZEICHEN];
        file = fopen(_filename, "r");



        if(file == NULL) {
            printf("Konfigurationsdatei konnte nicht geöffnet werden.\n");
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
                  printf("%s\n", line[i]);
                }

                fclose(file);
            for (int i = 0; i < 3; i++) {
                removeSpaces(line[i]);
            }

            // hier entsprechend um weitere parameter erweitern:


            strcpy(_config.hostname, line[0]);
            getmethevalue(_config.hostname);

            strcpy(_config.gamekindname , line[2]);
            getmethevalue(_config.gamekindname);

            char _portnumber[CONFIG_ZEICHEN];
            strcpy(_portnumber, line[1]);
            getmethevalue(_portnumber);
            _config.portnumber = atoi(_portnumber);
/*
            strcpy(_config.hostname, STANDARD_HOSTNAME);
            _config.portnumber = STANDARD_PORTNUMBER;
            strcpy(_config.gamekindname, STANDARD_GAMEKINDNAME);*/


        }
strcpy(_config.hostname, STANDARD_HOSTNAME);







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
  char *delimiter = "=";
  toget = strtok(string, delimiter);
  toget = strtok(NULL, delimiter);
  printf("toget2:%s\n",toget );
  strcpy(string, toget);
  printf("%s\n", string);
}
