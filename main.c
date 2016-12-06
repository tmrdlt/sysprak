
#define _POSIX_C_SOURCE 2
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "performConnection.h"
#include "connect_to_server.h"
#include "config.h"
#include <sys/wait.h>


// Bedienungshinweise
void printHelp() {
        printf("How to use:\n");
        printf("flags: -g <game_id> -p <player_number> -f <config_filename>\n");
        printf("-g: REQUIRED, game_id has to be 13 digits.\n");
        printf("-p: OPTIONAL, use this to specify the player number.\n");
        printf("-f: OPTIONAL, use this to specify a config file\n");
}

int player_number = -1;                       //hier kann die -p flag gespeichert werden
char *game_id;                                //hier kann die -g flag gespeichert werden
char *filename;                               //hier kann die -f flag gespeichert werden
char standard_filename[] = "client.conf";     //standard_filename


int main(int argc, char *argv[]) {


        
        pid_t pid = 0;                        //Prozess-ID des Kindprozesses
        int ret_code = 0;                     //Hilfsvariable für fork()
        filename = standard_filename;

        // flag Verwaltung über getopt
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


        // öffne Konfigurationsdatei und schreibe Werte in hostname, portnumber & gamekindname
        openconfig(filename);
        printf("Hostname: %s \n", _config.hostname);
        int fd = connect_to_server();

        if(fd == -1)
                return EXIT_FAILURE;

        initConnection(fd, game_id);


        //int shm_id;
        //char *shmdata;
        //shm_id = shm_id();

        // Aufspaltung in zwei Prozesse über fork()
        pid = ret_code = fork();
        if (ret_code < 0) {
                perror ("Fehler bei fork().");
                exit(EXIT_FAILURE);
        }

        /*
        *Elternprozess - Thinker
        */
        if (pid > 0) {
                printf("Hi hier ist der Thinker (Elternprozess)\n");
                //shmdata = address_shm (shm_id);



                ret_code = waitpid(pid, NULL, 0);
                if (ret_code < 0) {
                        perror ("Fehler beim Warten auf Kindprozess.");
                        exit(EXIT_FAILURE);
                }

        /*
        *Kindprozess - Connector
        */
        }else{
                printf("Hi hier ist der Connector (Kindprozess)\n");
                holdConnection(fd);
                //  shmdata = address_shm (shm_id);
        }


        return EXIT_SUCCESS;


}
