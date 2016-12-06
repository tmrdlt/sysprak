
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
        printf("flags: -g <game_id> -p <player_number> -f <config_filename>\n");
        printf("-g: REQUIRED, game_id has to be 13 digits.\n");
        printf("-p: REQUIRED, specifies the player number.\n");
        printf("-f: OPTIONAL, use this to specify a config file\n");
}
int player_number = -1;
char *game_id;
char *filename;
char standard_filename[] = "client.conf";

int main(int argc, char *argv[]) {

        pid_t pid;
        filename = standard_filename;
        
        // GameID mit -g Flag einlesen
        int ret;
        while ((ret = getopt(argc, argv, "g:p::f::")) != -1) {
                switch (ret) {
                case 'g':
                        game_id = optarg;
                        break;
                case 'p':
                        player_number = atoi(optarg);
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




        printf("GameID: %s\n", game_id);

        // öffne Konfigurationsdatei und schreibe Werte in hostname, portnumber & gamekindname
        openconfig(filename);
        printf("Hostname: %s \n" , _config.hostname);
        int fd = connect_to_server();

        if(fd == -1)
                return EXIT_FAILURE;

        initConnection(fd, game_id);


		//int shm_id;
		//char *shmdata;
		//shm_id = shm_id();
        switch (pid = fork()) {
        case -1:
                printf ("Fehler bei fork()\n");
                break;
        case 0:
                printf("Hi hier ist der Connector (Kindprozess)\n");
                holdConnection(fd);
                //shmdata = address_shm (shm_id);
                break;
        default:
                printf("Hi hier ist der Thinker (Elternprozess)\n");
              //  shmdata = address_shm (shm_id);
                break;
        }


        return EXIT_SUCCESS;


}
