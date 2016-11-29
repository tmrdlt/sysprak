
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
}

int main(int argc, char *argv[]) {

    char *filename;

    char *game_id;
        // GameID mit -g Flag einlesen
        int ret;
        while ((ret = getopt(argc, argv, "g:")) != -1) {
                switch (ret) {
                case 'g':
                        game_id = (optarg);
                        
                        
                        // hat die GameId wirklich 13 Stellen?

                        if (strlen(game_id) != 13) {
                            printHelp();
                            return EXIT_FAILURE;
                        }

                        printf("GameID: %s\n", game_id);

                        int fd = connect_to_server();
                        if(fd == -1)
                            return EXIT_FAILURE;
                        performConnection(fd, game_id);
                        
                        pid_t pid;
                        switch (pid = fork()) {
                            case -1:
                                printf ("Fehler bei fork()\n");
                                break;
                            case 0:
                                printf("Hi hier ist der Connector (Kindprozess)\n");
                                break;
                            default:
                                printf("Hi hier ist der Thinker (Elternprozess)\n");
                                break;
                        }
                        

                        return EXIT_SUCCESS;
                default:
                        printHelp();
                        return EXIT_FAILURE;
                       
                }
        }

}
