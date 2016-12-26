#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE 2


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "shared_memory_segment.h"
#include "performConnection.h"
#include "connect_to_server.h"
#include "thinker.h"
#include "config.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>

#define SIZE_COURT 8

// Bedienungshinweise
void printHelp() {

	printf("How to use:\n");
	printf("flags: -g <game_id> -p <player_number> -f <config_filename>\n");
	printf("-g: REQUIRED, game_id has to be 13 digits.\n");
	printf("-p: OPTIONAL, use this to specify the player number.\n");
	printf("-f: OPTIONAL, use this to specify a config file\n");
}

// Signal Handler um SIGUSR1 und ctr + c abzufangen
void handle_signal(int sig) {
		switch (sig) {
			case SIGUSR1:
			printf("Signal Number %d (SIGUSR1) recieved\n", sig);
			think();
			break;
			case SIGINT:
			printf("\nSignal Number %d (SIGINT) recieved\n", sig);
			printf("Program terminated by User\n");
			break;
		}
}

int player_number = -1;               //hier kann die -p flag gespeichert werden
char *game_id;                        //hier kann die -g flag gespeichert werden
char *filename;                       //hier kann die -f flag gespeichert werden
char standard_filename[] = "client.conf";     //standard_filename

int main(int argc, char *argv[]) {

	pid_t cpid, ppid, w;                    //Prozess-ID des Kindprozesses
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
		//test_msg_pattern();
		return EXIT_FAILURE;
	}

	// öffne Konfigurationsdatei und schreibe Werte in hostname, portnumber & gamekindname
	openconfig(filename);
	printf("Hostname: %s \n", _config.hostname);

	int _shm_id;

	game_state *shmdata;
	_shm_id = shm_id(
			sizeof(game_state) + sizeof(char) * SIZE_COURT * SIZE_COURT);

	shmdata = (game_state*) address_shm(_shm_id);

	shmdata->player_number = player_number;
	shmdata->game_name = game_id;

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

		printf("Connector (Kindprozess), PID: %ld\n", (long) getpid());
		// shmdata->process_id_connector = pid;

		int _fd = connect_to_server();

		if (_fd == -1)
			return EXIT_FAILURE;

		//Schreibeseite schliessen
		close(feld[1]);

		fd_pipe_thinker = feld[0];

		performConnection(_fd, _shm_id);

		printf("Id connector %d \n", shmdata->process_id_connector);


		printf("beende Connector\n");

		exit(EXIT_SUCCESS);
		//  shmdata = address_shm (shm_id);
		//printf("Id thinker %d \n" , shmdata->process_id_thinker);

	//Elternprozess
	} else {
		printf("Thinker (Elternprozess), PID: %ld\n", (long) ppid);
		//shmdata->process_id_thinker = pid;

		//Leseseite schliessen
		close(feld[0]);

		// In die Schreibseite der Pipe schreiben
		//write (feld[1], puffer, PIPE_BUF);

		fd = feld[1];

		id_seg_gameparams = _shm_id;

		int status; //Zum Abfangen des Status vom Kindprozess in waitpid

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
				printf("Child terminated normally with status: %d\n", WEXITSTATUS(status));
				}
		// so lange warten bis child normal terminiert, also WIFEXITED == TRUE
		} while (!WIFEXITED(status));

		printf("beende Thinker\n");
		exit(EXIT_SUCCESS);

	}

	return EXIT_SUCCESS;

}
