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
    shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT);
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

//Loeschen von SHM
int delete_shm (int shm_id){
    if(shmctl (shm_id, IPC_RMID, 0) < 0) {
        perror("SHM loeschen fehlgeschlagen! \n");
        return -1;}
    return 0;
}

//Ablegen von Daten in SHM
void write_shm (char *address_shm, int size, char *data)
{
    for (int i = 0; i < size; i++)
    {
        address_shm[i] = data[i];}
}

//Lesen von Daten
char *read_shm (char *address_shm, int size)
{
    char *data;
    for (int i = 0; i < size; i++)
    {
        data[i] = address_shm[i];
    }
    return data;
}