#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE 2

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>

#include "shared_memory_segment.h"
#include "performConnection.h"
#include "connect_to_server.h"
#include "thinker.h"
#include "config.h"
#include "bashni_structs.h"

// Bedienungshinweise für manuelle Ausführung
void printHelp() {

    printf("\nBenutzungshinweise:\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("Konsolenaufruf mit Flags: ./bin/client -g <game_id> -p<player_number> -f<config_filename>\n");
    printf("-g: REQUIRED, game_id hat normalerweise 13 Stellen.\n");
    printf("-p: OPTIONAL, kann genutzt werden um die gewünschte Spielernummer anzugeben (0 oder 1).\n");
    printf("-f: OPTIONAL, kann genutzt werden um eine andere Konfigurationsdatei anzugeben.\n");
    printf("Achtung: Bei den optionalen Argumenten kein Leerzeichen nach dem jeweiligen Flag angeben!\n");
    printf("---------------------------------------------------------------------------------\n");
}
// Signal Handler um SIGUSR1 und ctr + c abzufangen
void handle_signal(int sig) {
    switch (sig) {
        case SIGUSR1:
            think();
            break;
        case SIGINT:
            printf("\n...Der Client wurde durch ein Nutzersignal beendet.\n");
            exit(EXIT_SUCCESS);
            break;
    }
}

int player_number = -1;             				  //hier kann die -p flag gespeichert werden
char *game_id;                        				//hier kann die -g flag gespeichert werden
char *filename;                        				//hier kann die -f flag gespeichert werden
char standard_filename[] = "conf/client.conf";     //standard_filename

int main(int argc, char *argv[]) {

    if (argc <= 1){
        printHelp();
        return EXIT_FAILURE;
    }
    //Seed Initialisierung für den Thinker
    srand((unsigned)time(NULL));

    //Prozess-ID des Kindprozesses und des Elternprozesses
    pid_t cpid, w;
    pid_t  ppid;
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

    printf("\n...Willkommen beim Clienten der Gruppe 16 im System-Praktikum WS 16-17!\n");
    // öffne Konfigurationsdatei und schreibe Werte in hostname, portnumber & gamekindname
    openconfig(filename);
    int _shm_id;
    game_state *shmdata;
    _shm_id = shm_id(sizeof(game_state) + sizeof(char) * COURT_SIZE * COURT_SIZE);
    shmdata = (game_state*) address_shm(_shm_id);
    shmdata->game_name = game_id;
    shmdata->player_number = player_number;
    //Anlegen von namenlosen Pipe
    int feld[2];
    if (pipe(feld) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    //Elternpid bestimmen
    ppid = getpid();

    // Aufspaltung in zwei Prozesse über fork()
    cpid = fork();
    if (cpid == -1) {
        perror("Fehler bei fork().");
        exit(EXIT_FAILURE);
    }
    //Kindprozess
    if (cpid == 0) {
        printf("...Connectorprozess erstellt...PID: %ld\n\n", (long) getpid());
        int _fd = connect_to_server();
        if (_fd == -1)
            return EXIT_FAILURE;
        //Schreibeseite schliessen
        close(feld[1]);
        fd_pipe_thinker = feld[0];
        performConnection(_fd, _shm_id);
        printf("...Connectorprozess beendet.\n");
        exit(EXIT_SUCCESS);

        //Elternprozess
    } else {
        printf("\n...Thinkerprozess erstellt.....PID: %ld\n", (long) ppid);
        //Leseseite schliessen
        close(feld[0]);
        fd = feld[1];
        id_seg_gameparams = _shm_id;
        //Zum Abfangen des Status vom Kindprozess in waitpid
        int status;

    again:
        signal(SIGUSR1, handle_signal);
        signal(SIGINT, handle_signal);

        do {
            w = waitpid(cpid, &status, 0);
            if (w == -1) {
                // scheinbar stellt SIGUSR1 waitpid auf -1, was wir nicht wollen, daher wird dieser fall abgefangen:
                if (errno == EINTR) {
                    goto again;
                }
                exit(EXIT_FAILURE);
            }
            if (WIFEXITED(status)) {
                //printf("Child terminated normally with status: %d\n", WEXITSTATUS(status));
            }
            // so lange warten bis child normal terminiert, also WIFEXITED == TRUE
        } while (!WIFEXITED(status));

        printf("...Thinkerprozess beendet.\n\n");
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}
