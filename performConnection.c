
#include "performConnection.h"
#include <stdbool.h>

bool quit = false;


player *_player;
player *opponent_players;
game *gameparams;
phase _phase = PROLOG;



/*
 * performConnection holds client connection to Gameserver.
 */
void performConnection(int fd){
    //Placeholder
    //Buffer for reading/writing
    char  server_reply[2000];

    while(1){


        if(recv(fd , server_reply , 2000 , 0) < 0){
            perror("Receiving Message failed \n");
            break;


        }else if (server_reply[0] == '-'){
            perror("Fehler auf dem Gameserver ... Verbindung trennen! \n");
            printf(" %s \n", server_reply);
            break;
        }


        handle(server_reply, fd);

        if(quit){
            break;
        }

    }
    close(fd);
    free(_player);
    free(opponent_players);
    free(gameparams);
    // TODO clean Memory here
//     TODO check if Messsage contains \n ...
}

/**
 * Method for handling messages as String
 *
 */
void handle(char *server_reply, int fd){

    printf("Received Message: %s \n" , server_reply);

    // Split Message in Array of words (for parsing)
    char **splited_reply;
    int count_elements = split(server_reply, ' ', &splited_reply);

     // Servers Gameversion
    if(strcmp(splited_reply[1], "MNM")) {
        if(_phase != PROLOG){
            printf("Protokollverletzung durch Gameserver Client befindet sich nicht mehr im Prolog");
        }
        printf("Versions Nummer des MNM Webservers: %s" , splited_reply[3]);
        //TODO Check if fitting Versions
        // send Clients version
        if( send(fd, "VERSION", strlen("VERSION\0"),0) < 0){
            perror("Version des Clienten konnte nicht gesendet werden!");
            quit = true;
        }

    // Server allows entering Game
    }else if(strcmp(splited_reply[1], "PLAYING")) {

        //TODO Start Game
        //printf("Protokollverletzung durch Gameserver, Spieler bereits in Spiel!");


    // Player id- name allocation
     }else if(strcmp(splited_reply[1], "YOU")) {
        if(_player == NULL && _phase == PROLOG){
            char *end;

            long l = strtol(splited_reply[2], &end, 13);

            _player= (player*) malloc(sizeof(player*));
            _player->number = (int)l;
            _player->player_name = splited_reply[3];
                      printf("Your Player id is: %i and your name: %s" ,_player->number, _player->player_name);
        } else{
            perror("Böser Server ... Player wurde bereits gestezt");
            quit = true;
        }

    // Count Players in Game
    }else if(strcmp(splited_reply[1], "TOTAL")) {
        if(_phase == PROLOG){
            char *end;
            long tmp = strtol(splited_reply[2], &end, 13);

            int players_in_game = (int)tmp;
            //set num players in gameparams
            opponent_players = (player*)malloc(sizeof(player)*players_in_game);

            if(players_in_game != 1){
                printf("In dem von dir gewählten Spiel befinden sich bereits %i Spieler" , players_in_game);
            }else{
                printf("In dem von dir gewählten Spiel befindet sich bereits %i Spieler" , players_in_game);
            }
        } else{
            perror("Client befindet sich nicht mehr in der Prolog-phase");
            quit = true;
        }

            // All Players in Game transfered to Client
    }else if(strcmp(splited_reply[1], "ENDPLAYERS")) {
        // Todo set Flag in Gameparams
        if(_phase == PROLOG){
             printf("Alle Gegenspieler eingelesen -> nächste Phase");
            _phase = COURSE;
        } else{
            perror("Client befindet sich nicht mehr in der Prolog-phase: Einlesen konnte nicht abgeschlossen werden");
            quit = true;
        }

        // Client Waits for Server
    }else if(strcmp(splited_reply[1], "WAIT")) {

        printf("Warte auf Gameserver");
        if( send(fd, "OKWAIT", strlen("OKWAIT\0"),0) < 0){
            perror("Quittung für Wait konnte nicht gesendet werden!");
            quit = true;
        }

        //Server allows Client to make next Move
    }else if(strcmp(splited_reply[1], "MOVE")) {
        // TODO
         printf("Du bist am Zug und hast %s sekunden" ,splited_reply[2]);

        //Changed Gamestate - Server sends changed pieces
    }else if(strcmp(splited_reply[1], "PIECESLIST")) {
        //TODO Flag
         printf("Steine setzen");

        //Move Brick
        //TODO strcmp -> zu contains
    }else if(strcmp(splited_reply[1], "@")) {
        printf("Stein auf %s setzen", splited_reply[1]);
        //TODO change gameState

        //Server erlaubt berechnung des nächsten Zuges
    }else if(strcmp(splited_reply[1], "OKTHINKING")) {
        // thinking();
        printf("Berechne deinen Zug");

        //Game over
    }else if(strcmp(splited_reply[1], "GAMEOVER")) {
        printf("das Spiel ist vorbei");
        quit = true;

        //Changed Game Pieces transfered
    }else if(strcmp(splited_reply[1], "ENDPIECESLIST")) {
        //TODO Flag setzen
        printf("Alle Steine gelesen und gesetzt");

        //Gewinner Spiel
    }else if(strcmp(splited_reply[1], "PLAYER0WON")) {

        if (strcmp(splited_reply[2], "Yes")){
            printf("Player 0 gewinnt");
        } else{
            printf("Player 0 verliert");
        }

         //Gewinner Spiel
    }else if(strcmp(splited_reply[1], "PLAYER1WON")) {
        if (strcmp(splited_reply[2], "Yes")){
            printf("Player 1 gewinnt");
        } else{
            printf("Player 1 verliert");
        }
        //Quit Client and connection
    }else if(strcmp(splited_reply[1], "QUIT")) {
        printf("Beende deinen Clienten ");
        quit = true;

        //new opponent player
    }else if(count_elements == 4){
        printf("Spieler mit Nr: %s und Name: %s ist bereit: %s" , splited_reply[1], splited_reply[2], splited_reply[3]);

        char *end;
        long tmp = strtol(splited_reply[1], &end, 13);

        int p_nr = (int)tmp;
        player *p = (player*) malloc(sizeof(player));
        p->number = p_nr;
        p->player_name = splited_reply[2];
        tmp = strtol(splited_reply[3], &end, 13);
        int p_flag = (int)tmp;
        p->flag = p_flag;


        opponent_players[gameparams->player_count-1] = *p;
        gameparams->player_count=gameparams->player_count+1;
        // Name des Spiels
    }else if (count_elements == 2){

        printf("Bot betritt das Spiel: %s!" , splited_reply[2]);

        //sende gewünschte Spielernummer (noch leer)
        char *message = "Player";
        if( send(fd, message, strlen(message),0) < 0){
            perror("Initialisierung Spieler fehlgeschlagen");
            quit = true;
        }

        // Client Version wurde akzeptiert
    } else if (strcmp(server_reply, "Client version accepted")){

            printf("Die aktuelle Version des Clienten wurde vom Gameserver akzeptiert! Jetzt gehts los!");
            // Sende die Game-ID zum Server
            char id_msg[16]= "ID ";
            char id_game[13];
            sprintf(id_game, "%s", GAME_ID);
            strcat(id_msg, id_game);

            if( send(fd, id_msg, strlen(id_msg),0) < 0){
                perror("Fehler bei der Übertragung der Game Id!");
                quit = true;
            }
        // Nicht erkannte Nachricht - fehler
    }else{
        printf("Nachricht konnte nicht erkannt werden:\n %s", server_reply);
        quit = true;
    }

    printf("\n");
    // Frei geben des tmp allozierten Speicherplatz für die aufgespaltene Nachricht
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

    // fill array with splited strings(words)
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


//int send_to_gameserver(int fd, char *message){
//    return (int)send(fd, message, strlen(message), 0);
//}


/*int main(int argc, const char * argv[]) {
    // insert code here...

    printf("Hello World\n");

    char *instructionsToCheck[] = {
        "+ MNM Gameserver 12551",
        "+ Client version accepted - please send Game-ID to join",
        "+ PLAYING 12451",
        "+ Mustergame",
        "+ YOU 2 Hans",
        "+ TOTAL 3"
        "+ 0 Peter Nein",
        "+ 1 Paul Ja",
        "+ 3 Helga JA",
        "+ ENDPLAYERS",
        "+ MOVE 2315",
        "+ PIECESLIST",
        "+ w@A1",
        "+ b@C1",
        "+ ENDPIECESLIST",
        "+ OKTHINK",
        "+ GAMEOVER"
        "+ PLAYER0WON Yes",
        "+ PLAYER1WON No",
        "+ PLAYER0WON No",
        "+ PLAYER1WON Yes",
        "+ QUIT"
    };
    for(int i = 0 ; i< 20; i++){
        handle(instructionsToCheck[i], 12);
    }

    return 0;
}
*/
