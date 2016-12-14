
#define _POSIX_C_SOURCE 2
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<limits.h>
#include "shared_memory_segment.h"
#include "performConnection.h"
#include "connect_to_server.h"
#include "thinker.h"
#include "config.h"
#include <sys/wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>

#define SIZE_COURT 8

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
        //test_msg_pattern();
        return EXIT_FAILURE;
    }


    // öffne Konfigurationsdatei und schreibe Werte in hostname, portnumber & gamekindname
    openconfig(filename);
    printf("Hostname: %s \n", _config.hostname);
    int fd = connect_to_server();

    if(fd == -1)
        return EXIT_FAILURE;



    int _shm_id;
    game_state *shmdata;
    _shm_id =shm_id(sizeof(game_state) + sizeof(char)*SIZE_COURT*SIZE_COURT);

    shmdata = (game_state*)address_shm(_shm_id);

    shmdata->player_number = player_number;
    shmdata->game_name = game_id;

    performConnection(fd, _shm_id);
    
    //Anlegen von namenlosen Pipe
    int feld[2] , n;
    char puffer[PIPE_BUF];
    
    if (pipe (feld) < 0) {
      perror ("pipe");
      exit (EXIT_FAILURE);
   }

    // Aufspaltung in zwei Prozesse über fork()
    pid = ret_code = fork();

    if (ret_code < 0) {
        perror ("Fehler bei fork().");
        exit(EXIT_FAILURE);
    }


    /*
     *Elternprozess - Thinker
     */
    else if (pid > 0) {
        printf("Hi hier ist der Thinker (Elternprozess)\n");
        //shmdata->process_id_thinker = pid;
        
        signal(SIGUSR1, think);
        
        //Leseseite schliessen
        close (feld[0]);
        
        // In die Schreibseite der Pipe schreiben 
        if (write (feld[1], puffer, PIPE_BUF) < 0) {
        	perror ("Fehler beim Schreiben in die Pipe.");
		} else {
			n = write (feld[1], puffer, PIPE_BUF);
		}

         ret_code = wait(NULL);

        if (ret_code < 0) {
            perror ("Fehler beim Warten auf Kindprozess.");
            exit(EXIT_FAILURE);
        }
        printf("beende Thinker\n");
        //printf("Id thinker %d \n" , shmdata->process_id_thinker);
        /*
         *Kindprozess - Connector
         */
    }else{
        printf("Hi hier ist der Connector (Kindprozess)\n");
       // shmdata->process_id_connector = pid;

       // sleep(10);
       game_state *_game_state = address_shm(_shm_id);
    
    	if(_game_state->flag_thinking==1){
			if (kill(getppid(), SIGUSR1) < 0) {
       		perror ("Fehler bei Senden vom Signal).");
        	exit(EXIT_FAILURE);}
		}
       		
	         
       
        //Schreibeseite schliessen
        close (feld[1]);
        // Leseseite der Pipe auslesen
        if (read (feld[0], puffer, PIPE_BUF) != n) {
        	perror ("Fehler beim Lesen von der Pipe.");
		}

        printf("Id connector %d \n" , shmdata->process_id_connector);
        printf("beende Connector\n");
        exit(42);
        //  shmdata = address_shm (shm_id);
    }

    return EXIT_SUCCESS;

}
