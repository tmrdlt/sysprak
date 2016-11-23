#define _POSIX_C_SOURCE 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "performConnection.h"
#include "connect_to_server.h"


void printHelp() {
        printf("How to use:\n");
        printf("Please add -g <gameid>\n");
}


int mainb(int argc, char *argv[]) {

        int gameid = 0;

        // read gameid with -g flag
        int ret;
        while ((ret = getopt(argc, argv, "g:")) != -1) {
            switch (ret) {
                case 'g':
                        gameid = atoi(optarg);
                        break;
                default:
                        printHelp();
                        return EXIT_FAILURE;
                        break;
                }
        }

        if (gameid == 0) {
                printHelp();
                return EXIT_FAILURE;
        }

        printf("GameID: %d\n", gameid);

        //performConnection();

        // insert code here...

        printf("...for testing purposes\n");
        //testing
    
        int fd = connect_to_server();
        performConnection(fd);
        return EXIT_SUCCESS;
}
