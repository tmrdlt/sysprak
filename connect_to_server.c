#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<unistd.h>
#include "connect_to_server.h"

#define  HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define  PORTNUMBER 1357


int connect_to_server ()
{
	int create_socket, if_connected;
	struct sockaddr_in address;
	struct hostent *host; 
	struct in_addr **ip_host;
	
	//Anlegen von Socket
	create_socket = socket (AF_INET, SOCK_STREAM, 0);
	if(create_socket < 0) {
		printf ("Socket wurde nicht angelegt \n");
	}
	 printf ("Socket wurde angelegt \n");
	 
	//Vorbereiten von Socket
	host = gethostbyname ( HOSTNAME );
	ip_host = (struct in_addr **) host -> h_addr_list;
	address.sin_family = AF_INET;
	address.sin_port = htons ( PORTNUMBER );
	address.sin_addr = **ip_host;
	
	//Verbinden mit Server
	if_connected = connect (create_socket, (struct sockaddr *)&address, sizeof(address));
	if(if_connected != 0){
	 	printf ("Verbindung mit dem Server konnte nicht hergestellt werden \n");
	 }
	 printf ("Verbindung mit dem Server hergestellt \n");
		 
    return create_socket;
	
}
