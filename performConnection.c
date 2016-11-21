
#include "performConnection.h"

/*
 * performConnection holds client connection to Gameserver.
 */
void performConnection(int *fd){
    //Placeholder
    int socket;
    //Buffer for reading/writing
    char  server_reply[2000];

    if( send(socket, CLIENT_MSG[PLAYER], strlen(CLIENT_MSG[PLAYER]),0) < 0){
        perror("Player init failed");
        
        return;
    }
    
    while(1){
        if(recv(socket , server_reply , 2000 , 0) < 0){
            perror("Receiving Message failed");
            break;
        }else if (server_reply[0] == '-'){
            perror("Server rejected last message terminating client");
            break;
        }
        
    }
    
    // TODO clean Memory here
    
}


void handle(char *server_reply){
   // char message[1000];
    
    char **splited_reply; //= malloc(sizeof(server_reply)*2);
   
    int count_elements = split(server_reply, ' ', &splited_reply);
    
    if(strstr(splited_reply[1], "YOU")) {
        char *end;
        
        long l = strtol(splited_reply[2], &end, 13);
       
        player *p = (player*) malloc(sizeof(player*));
        p->id = (int)l;
        p->name = splited_reply[3];
        printf("Your Player id is: %i and your name: %s" , p->id, p->name);
        
        free(p);
    }else if(strstr(splited_reply[1], "TOTAL")) {
        
        int players_in_game = (int)splited_reply[2];
        
        if(players_in_game != 1){
            printf("In dem von dir gewählten Spiel befinden sich bereits %i Spieler" , players_in_game);
        }else{
            printf("In dem von dir gewählten Spiel befindet sich bereits %i Spieler" , players_in_game);
        }
        
    }else if(strstr(splited_reply[1], "PLAYING")) {
    
        printf("Protokollverletzung durch Gameserver, Spieler bereits in Spiel!");
    
    }else if(strstr(splited_reply[1], "MNM")) {
        
        printf("Protokollverletzung durch Gameserver, Versionsnummer bereits erhalten!");
    
    }else if(strstr(splited_reply[1], "ENDPLAYERS")) {
        
        printf("Alle Gegenspieler eingelesen");
        
    }else if(strstr(splited_reply[1], "WAIT")) {
        
        printf("Warte auf Gameserver");
        
    }else if(strstr(splited_reply[1], "MOVE")) {
         printf("Du bist am Zug und hast %s sekunden" ,splited_reply[2]);
        
    }else if(strstr(splited_reply[1], "PIECESLIST")) {
         printf("Steine setzen");
    
    }else if(strstr(splited_reply[1], "@")) {
        printf("Stein auf %s setzen", splited_reply[1]);
    
    }else if(strstr(splited_reply[1], "OKTHINKING")) {
        printf("Berechne deinen Zug");
        
    }else if(strstr(splited_reply[1], "GAMEOVER")) {
        printf("das Spiel ist vorbei");
    }else if(strstr(splited_reply[1], "ENDPIECESLIST")) {
        printf("Alle Steine gelesen und gesetzt");
    }else if(strstr(splited_reply[1], "PLAYER0WON")) {
        if (strcmp(splited_reply[2], "Yes")){
            printf("Player 0 gewinnt");
        } else{
            printf("Player 0 verliert");
        }
        
    }else if(strstr(splited_reply[1], "PLAYER1WON")) {
        if (strcmp(splited_reply[2], "Yes")){
            printf("Player 1 gewinnt");
        } else{
            printf("Player 1 verliert");
        }
    }else if(strstr(splited_reply[1], "QUIT")) {
        printf("Beende deinen Clienten ");
    }else{
        //new player
        if(count_elements == 4){
            printf("Spieler mit Nr: %s und Name: %s ist bereit: %s" , splited_reply[1], splited_reply[2], splited_reply[3]);
        }else if (count_elements == 2){
            printf("Protokollverletzung durch Gameserver, Spieler bereits in Spiel!");
        } else if (strstr(server_reply, "Client version accepted")){
            printf("Protokollverletzung durch Gameserver, Game Id bereits gesendet und verifiziert!");
        }else{
            printf("Nachricht konnte nicht erkannt werden:\n %s", server_reply);
        }
    }
    printf("\n");
   free(splited_reply);
}

/**
 * Helper Method for Message parsing: splits a String and saves all its splits into dest array
 */
int split (char *string_to_spilt, char delimiter, char ***dest)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *copy_of_string;
    char *tmp_dest;
    
    
    // count words and allocate array of strings
    copy_of_string = string_to_spilt;
    while (*copy_of_string != '\0')
    {
        if (*copy_of_string == delimiter)
            count++;
        copy_of_string++;
    }

    *dest = (char**) malloc(sizeof(char*) * count);
    if (*dest == NULL){
        perror("Memory allocation failed");
        exit(1);
    }
    
    // count tokens of each word and allocate each string
    copy_of_string = string_to_spilt;
    while (*copy_of_string != '\0')
    {
        if (*copy_of_string == delimiter)
        {
            (*dest)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*dest)[i] == NULL){
                perror("Memory allocation failed");
                exit(1);
            }
            
            token_len = 0;
            i++;
        }
        copy_of_string++;
        token_len++;
    }
    (*dest)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*dest)[i] == NULL){
        perror("Memory allocation failed");
        exit(1);
    }
    
    // fill array with splited strings
    i = 0;
    copy_of_string = string_to_spilt;
    tmp_dest = ((*dest)[i]);
    while (*copy_of_string != '\0')
    {
        if (*copy_of_string != delimiter && *copy_of_string != '\0')
        {
            *tmp_dest = *copy_of_string;
            tmp_dest++;
        }
        else
        {
            *tmp_dest = '\0';
            i++;
            tmp_dest = ((*dest)[i]);
        }
        copy_of_string++;
    }
    
    return count;
}

