#define _POSIX_C_SOURCE 2
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "performConnection.h"
#include "connect_to_server.h"

// Stellen einer Long Zahl bestimmen
int countdigits(long num){
        return (int) log10(num) + 1;

}
// Bedienungshinweise
void printHelp() {
        printf("How to use:\n");
        printf("Please add -g <gameid>\n");
        printf("the GameID has to be 13 digits\n");
}

int main(int argc, char *argv[]) {

        unsigned long gameid = 0;

        // GameID mit -g Flag einlesen
        int ret;
        while ((ret = getopt(argc, argv, "g:")) != -1) {
              switch (ret) {
                case 'g':
                        gameid = atol(optarg);
                        break;
                default:
                        printHelp();
                        return EXIT_FAILURE;
                        break;
                }
        }

        // hat die GameId wirklich 13 Stellen?
        if (countdigits(gameid) != 13) {
                printHelp();
                return EXIT_FAILURE;
        }

        printf("GameID: %ld\n", gameid);

        // insert code here...

        printf("...for testing purposes\n");

        //testing

        int fd = connect_to_server();
        if(fd == -1)
            return EXIT_FAILURE;
        performConnection(fd);
        return EXIT_SUCCESS;
}
