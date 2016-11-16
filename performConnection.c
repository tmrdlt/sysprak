
#include "performConnection.h"

/*
 * performConnection holds client connection to Gameserver.
 */
void performConnection(int *fd){
    //Placeholder
    int socket;
    //Buffer for reading/writing
    char message[1000] , server_reply[2000];

    if( send(socket, CLIENT_MSG[PLAYER], strlen(CLIENT_MSG[PLAYER]),0) < 0){
        perror("Player init failed");
        
        return;
    }
    
    while(1){
        if( recv(socket , server_reply , 2000 , 0) < 0){
            perror("Receiving Message failed");
            break;
        }else if (server_reply[0] == '-'){
            perror("Server rejected last message terminating client");
            break;
        }

        if(strstr(server_reply,"YOU")){
            char delimiter[] = " \n";
            char *splited_reply;
            splited_reply = strtok(server_reply, delimiter);
            player *p = (player*)malloc(sizeof(player*));
            p->id = splited_reply[2];
            p->name = &splited_reply[3];
        }
    }
    
    // TODO clean Memory here
    
}

