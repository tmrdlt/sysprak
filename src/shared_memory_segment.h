#ifndef shared_memory_segment_h
#define shared_memory_segment_h

int shm_id (int size);

void *address_shm (int shm_id);

int dettach_shm (void *address);

//Loeschen von SHM
int delete_shm (int shm_id);

void write_shm (char *address_shm, int size, char *data);

char *read_shm (char *address_shm, int size);

#endif /* shared_memory_segment_h */
