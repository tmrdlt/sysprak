#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>

struct segment{
	char *game_name;
	int *player_number;
	int player_count;
	pid_t process_id_thinker;
	pid_t process_id_connector;
};

struct player{
	char *player_name;
	int number;
	int flag;
} players;

void shared_memory_segment () {
	
}
