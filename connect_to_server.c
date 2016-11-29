
#include "connect_to_server.h"


int connect_to_server ()
{
	int create_socket, if_connected;
	struct sockaddr_in address;
	struct hostent *host;
	struct in_addr **ip_host;

	//Anlegen von Socket
<<<<<<< HEAD
	create_socket = socket (AF_INET, SOCK_STREAM, 0);
	if(create_socket < 0) {
		printf ("Socket wurde nicht angelegt \n");
	}
	 printf ("Socket wurde angelegt \n");
	 
=======
	if((create_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket anlegen fehlgeschlagen! \n");
        return -1;
	}

    printf ("Socket wurde angelegt \n");

>>>>>>> master
	//Vorbereiten von Socket
	host = gethostbyname (config.hostname);
	ip_host = (struct in_addr **) host -> h_addr_list;
	address.sin_family = AF_INET;
	address.sin_port = htons (config.portnumber);
	address.sin_addr = **ip_host;

	//Verbinden mit Server
<<<<<<< HEAD
	if_connected = connect (create_socket, (struct sockaddr *)&address, sizeof(address));
	if(if_connected != 0){
	 	printf ("Verbindung mit dem Server konnte nicht hergestellt werden \n");
	 }
	 printf ("Verbindung mit dem Server hergestellt \n");
		 
=======
	if((connect (create_socket, (struct sockaddr *)&address, sizeof(address))) != 0){
        perror("Verbindungsaufbau fehlgeschlagen! \n");
        return -1;
    }
    printf ("Verbindung mit dem Server hergestellt \n");


	 //EXIT
>>>>>>> master
    return create_socket;

}
