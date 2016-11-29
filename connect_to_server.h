#ifndef connect_to_server_h
#define connect_to_server_h

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<unistd.h>
#include "config.h"

int connect_to_server ();

#endif /* connect_to_server_h */
