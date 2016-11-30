
#define _POSIX_C_SOURCE 2
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "performConnection.h"
#include "connect_to_server.h"
#include "config.h"



// Bedienungshinweise
void printHelp() {
        printf("How to use:\n");
        printf("Please add -g <gameid>\n");
        printf("the GameID has to be 13 digits\n");
        printf("Optional: add -f <filename> to specify the config file\n");
}
char *game_id;
char *filename;
char standard_filename[] = "client.conf";

int main(int argc, char *argv[]) {

        pid_t pid;


        // GameID mit -g Flag einlesen
        int ret;
        while ((ret = getopt(argc, argv, "g:f::")) != -1) {
                switch (ret) {
                case 'g':
                        game_id = optarg;
                        break;

                case 'f':
                        filename = optarg;
                        break;

                default:
                        printHelp();
                        return EXIT_FAILURE;
                }

        }

        // hat die GameId wirklich 13 Stellen?
        if (game_id == NULL || strlen(game_id) != 13) {
          printHelp();
          return EXIT_FAILURE;
        }

        // wenn kein -f Flag, dann nehme standard_filename
        if (argc < 4) {
                filename = standard_filename;
        }


        printf("GameID: %s\n", game_id);

        // öffne Konfigurationsdatei und schreibe Werte in hostname, portnumber & gamekindname
        openconfig(filename);

        int fd = connect_to_server();
        if(fd == -1)
                return EXIT_FAILURE;
        performConnection(fd, game_id);

		
		int shm_id; 
		char *shmdata;
		shm_id = shm_id();
        switch (pid = fork()) {
        case -1:
                printf ("Fehler bei fork()\n");
                break;
        case 0:
                printf("Hi hier ist der Connector (Kindprozess)\n");
                shmdata = address_shm (shm_id);
                break;
        default:
                printf("Hi hier ist der Thinker (Elternprozess)\n");
                shmdata = address_shm (shm_id);
                break;
        }


        return EXIT_SUCCESS;


}
