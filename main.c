#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void printHelp() {
        printf("How to use:\n");
        printf("Please add -g <gameid>\n");
}
int main(int argc, char *argv[]) {

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

        // insert code here...
        printf("...for testing purposes\n");
        //testing

        return EXIT_SUCCESS;
}
