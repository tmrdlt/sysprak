#include<unistd.h>
#include<stdlib.h>

#define  HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define  PORTNUMBER 1357
#define  GAMEKINDNAME "Bashni"


char *GAME_ID;

typedef struct{
    char *player_name;
    int number;
    int flag;
} player;


typedef struct {
    char *game_name;
    int *player_number;
    int player_count;

    pid_t process_id_thinker;
    pid_t process_id_connector;
}game;
