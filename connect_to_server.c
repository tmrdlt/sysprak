#include "connect_to_server.h"


int connect_to_server ()
{
	int create_socket;
	struct sockaddr_in address;
	struct hostent *host;
	struct in_addr **ip_host;

	//Anlegen von Socket
	if((create_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket anlegen fehlgeschlagen! \n");
        return -1;
	}


	//Vorbereiten von Socket
	host = gethostbyname (config.hostname);
	ip_host = (struct in_addr **) host -> h_addr_list;
	address.sin_family = AF_INET;
	address.sin_port = htons (config.portnumber);
	address.sin_addr = **ip_host;

	//Verbinden mit Server



	if((connect (create_socket, (struct sockaddr *)&address, sizeof(address))) != 0){
        perror("Verbindungsaufbau fehlgeschlagen! \n");
        return -1;
    }
    printf ("Verbindung mit dem Server hergestellt \n");


	 //EXIT

    return create_socket;

}
