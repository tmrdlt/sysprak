#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include "shared_memory_segment.h"
#include "performConnection.h"

//Anlegen von SHM
int shm_id (int size) {
    
    int shm_id;
    shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT| 0777);
    if(shm_id < 0) {
        perror("SHM anlegen fehlgeschlagen! \n");
        return -1;
    }
    return shm_id;
}

//Anbinden von SHM
void *address_shm (int shm_id) {
    
    void *address_shm;
    address_shm = shmat (shm_id, (void*)0, 0);
    if(address_shm < (void*)0) {
        perror("SHM anbinden fehlgeschlagen! \n");
        return "-1";
    }
    return address_shm;
}

//Loslösen von SHM
int dettach_shm (void *address){
    if(shmdt (address) < 0) {
        perror("SHM loslösen fehlgeschlagen! \n");
        return -1;}
    return 0;
}
	
	/*int delete_shm (it shm_id) {
	struct shmid_ds *buf;
	if(shmctl (shm_id, IPC_RMID, buf) < 0) {
        perror("SHM loeschen fehlgeschlagen! \n");
        return -1;
	}
	return 0;
}
	
	//Ablegen von Daten in SHM
	void write_in_shm(char *address_shm){
		for (game.player_count=!0 ; game.player_count--)
			{
		*address_shm++ = player.player_name;
		*address_shm++ = player.number;
		*address_shm++ = player.flag;
			}
	}*/


//Loeschen von SHM
int delete_shm (int shm_id){
    if(shmctl (shm_id, IPC_RMID, 0) < 0) {
        perror("SHM loeschen fehlgeschlagen! \n");
        return -1;}
    return 0;

}

//Ablegen von Daten in SHM
//void write_shm (char *address_shm, int size, char *data)
//{
//    for (int i = 0; i < size; i++)
//    {
//        address_shm[i] = data[i];}
//}

//Lesen von Daten
//char *read_shm (char *address_shm, int size)
//{
//    char *data = malloc(sizeof(char)*size);
//    for (int i = 0; i < size; i++)
//    {
//        data[i] = address_shm[i];
//    }
//    return data;
//}
