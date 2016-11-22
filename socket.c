#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<unistd.h>
#define sysprak.priv.lab.nm.ifi.lmu.de HOSTNAME
#define 1357 PORTNUMBER
int main (int argc, char **argv)
{
	int create_socket;
	struct sockaddr_in address;
	struct hostent *host; 
	struct in_addr **ip_host;
	
	//Anlegen von Socket
	if((create_socket = socket (AF_INET, SOCK_STREAM, 0)) > 0) {
		printf ("Socket wurde angelegt \n");
	}
	
	//Vorbereiten von Socket
	host = gethostbyname ( HOSTNAME );
	ip_host = (struct in_addr **) host -> h_addr_list;
	address.sin_family = AF_NET;
	address.sin_port = htons ( PORTNUMBER );
	address.sin_addr = ip_host;
	
	//Verbinden mit Server
	if((connect (create_socket, (struct sockaddr *)&address, sizeof(address))) == 0){
	 	printf ("Verbindung mit dem Server hergestellt \n");
	 }
	 
		 
	 //EXIT
	 close (create_socket);
		
	
}
