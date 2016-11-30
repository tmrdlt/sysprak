#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include "shared_memory_segment.h"
#include "performConnection.h"

//Anlegen von SHM
int shm_id () {
	
	int shm_id;
	shm_id = shmget(IPC_PRIVATE, sizeof(player)*game.player_count, IPC_CREAT);
	if(shm_id < 0) {
        perror("SHM anlegen fehlgeschlagen! \n");
        return -1;
	}
	return shm_id; 
	}
	
//Anbinden von SHM
char *address_shm (int shm_id) {

	char *address_shm;
	address_shm = shmat (shm_id, NULL, 0);
	if(address_shm < 0) {
        perror("SHM anbinden fehlgeschlagen! \n");
        return -1;
	}
	return address_shm;
}
	
	/*
	if(shmctl (shm_id, IPC_RMID, ?) < 0) {
        perror("SHM loeschen fehlgeschlagen! \n");
        return -1;
	}
	
	
	//Ablegen von Daten in SHM
	char *data;
	data = address_shm;
	for (game.player_count=!0 ; game.player_count--)
	{
		*data++ = player.player_name;
		*data++ = player.number;
		*data++ = player.flag;
	}*/
}
