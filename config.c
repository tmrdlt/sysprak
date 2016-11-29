#include "config.h"



void openconfig(char *filename) {
        char *_filename = filename;
        FILE *file;

        file = fopen(_filename, "r");

        if(file == NULL) {
                printf("Konfigurationsdatei konnte nicht geöffnet werden.\n");
        }else {
                fscanf(file, "%s %d %s", config.hostname, &config.portnumber, config.gamekindname);

            //  test:  printf("host: %s\nport: %d\nkind: %s\n", config.hostname, config.portnumber, config.gamekindname);
                fclose(file);
        }

}
