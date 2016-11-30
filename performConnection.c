
#include "performConnection.h"
#include <stdbool.h>

bool quit = false;


player *_player;
player *opponent_players;
game *gameparams;
phase _phase = PROLOG;

char *_game_id;

char *version = "2.1";

phase_func_t* const phase_table[3] = {
    handle_prolog, handle_course, handle_draft
};


/*
 * performConnection holds client connection to Gameserver.
 */
void performConnection(int fd, char *game_id){
    //Placeholder
    //Buffer for reading/writing
    char  server_reply[2000];
    _game_id = game_id;
    while(1){


        if(recv(fd , server_reply , 2000 , 0) < 0){
            perror("Receiving Message failed \n");
            break;


        }else if (server_reply[0] == '-'){
            printf("Fehler auf dem Gameserver ... Verbindung trennen! \n");
            //printf(" %s \n", server_reply);
            break;
        }


        char **splited_reply;
        int count_elements = split(server_reply, ' ', &splited_reply);

        phase_data *data = (phase_data*) malloc(sizeof(phase_data));

        if(data == NULL){
            perror("Memory allocation Phase data failed");
            break;
        }

        data->count_elements = count_elements;
        data->splited_reply = splited_reply;
        data->server_reply = server_reply;
        data->fd = fd;

        _phase = run_phase(_phase,data);
        // handle(server_reply, fd);

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

phase handle_prolog(phase_data *data ){
    phase new_phase = _phase;

    if(strstr(data->splited_reply[1], "MNM")) {

        printf("Versions Nummer des MNM Webservers: %s" , data->splited_reply[3]);
        //Check if fitting Versions
        data->splited_reply[3]++;
        double version_server;
        sscanf(data->splited_reply[3], "%lf", &version_server);
        double version_client;
        sscanf(version, "%lf" , &version_client);

        if((int)version_server != (int) version_client){
            quit = true;
            printf("\n Version des Clienten: %s ist nicht mit der des Servers: v%s kompatibel!",version ,  data->splited_reply[3] );
            return new_phase;
        }

        // send Clients version
        char msg[13] = "VERSION ";
        strcat(msg, version);

        if( send_to_gameserver(data->fd, msg) < 0){
            printf("Version des Clienten konnte nicht gesendet werden!");
            quit = true;
        }

        // Server allows entering Game

    }else if(strstr(data->splited_reply[1], "PLAYING")) {

        if(!strstr(data->splited_reply[2], config.gamekindname)){
                printf("Falsches Spiel!");
                quit = true;
                return new_phase;

        }

        // Player id- name allocation
    }else if(strstr(data->splited_reply[1], "YOU")) {
        if(_player == NULL && _phase == PROLOG){
            char *end;

            long l = strtol(data->splited_reply[2], &end, 13);

            _player= (player*) malloc(sizeof(player*));
            _player->number = (int)l;
            _player->player_name = data->splited_reply[3];
            printf("Hi (%i) %s !" ,_player->number, _player->player_name);
        } else{
            perror("Böser Server ... Player wurde bereits gestezt");
            quit = true;
        }

        // Count Players in Game
    }else if(strstr(data->splited_reply[1], "TOTAL")) {

            char *end;
            long tmp = strtol(data->splited_reply[2], &end, 13);

            int players_in_game = (int)tmp;
            //set num players in gameparams
            opponent_players = (player*)malloc(sizeof(player)*players_in_game);

            if(players_in_game != 1){
                printf("In dem von dir gewählten Spiel befinden sich bereits %i Spieler" , players_in_game);
            }else{
                printf("In dem von dir gewählten Spiel befindet sich bereits %i Spieler" , players_in_game);
            }


        // All Players in Game transfered to Client
    }else if(strstr(data->splited_reply[1], "ENDPLAYERS")) {
        // Todo set Flag in Gameparams
        new_phase = COURSE;

        //Neuer Spielerim Spiel
    } else if(data->count_elements == 4){

        char *end;
        long nr_l = strtol(data->splited_reply[1], &end, 13);

        int p_nr = (int)nr_l;
        player *p = (player*) malloc(sizeof(player));
        p->number = p_nr;
        p->player_name = data->splited_reply[2];
        long flag_l = strtol(data->splited_reply[3], &end, 13);
        int p_flag = (int)flag_l;
        p->flag = p_flag;

        if(p_flag == 1)
            printf("Spieler (%d) %s ist bereit" , p_nr, data->splited_reply[2]);
        else
            printf("Spieler (%d) %s ist noch nicht bereit" , p_nr, data->splited_reply[2]);

        opponent_players[gameparams->player_count-1] = *p;
        gameparams->player_count=gameparams->player_count+1;

        // Name des Spiels
    }else if (data->count_elements == 2){

        printf("Bot betritt das Spiel: %s!" , data->splited_reply[2]);

        //sende gewünschte Spielernummer (noch leer)
        char *message = "Player";
        if( send_to_gameserver(data->fd, message) < 0){
            perror("Initialisierung Spieler fehlgeschlagen");
            quit = true;
        }

        // Client Version wurde akzeptiert
    } else if (strstr(data->server_reply, "Client version accepted")){

        printf("Die aktuelle Version des Clienten wurde vom Gameserver akzeptiert! Jetzt gehts los!");
        // Sende die Game-ID zum Server
        char id_msg[16]= "ID ";
        char id_game[13];
        sprintf(id_game, "%s", _game_id);
        strcat(id_msg, id_game);

        if( send_to_gameserver(data->fd, id_msg) < 0){
            perror("Fehler bei der Übertragung der Game Id!");
            quit = true;
        }
        // Nicht erkannte Nachricht - fehler
    }else{
        printf("Nachricht konnte nicht erkannt werden oder ist für die aktuelle Phase nicht zulässig:\n %s", data->server_reply);
        quit = true;
    }

    return new_phase;
}


phase handle_course(phase_data *data ){
    phase new_phase = _phase;
    if(strstr(data->splited_reply[1], "WAIT")) {

        printf("Warte auf Gameserver");
        if( send_to_gameserver(data->fd, "OKWAIT") < 0){
            perror("Quittung für Wait konnte nicht gesendet werden!");
            quit = true;
        }

        //Server allows Client to make next Move
    }else if(strstr(data->splited_reply[1], "MOVE")) {
        // TODO
        printf("Du bist am Zug und hast %s sekunden" ,data->splited_reply[2]);

        //Changed Gamestate - Server sends changed pieces
    }else if(strstr(data->splited_reply[1], "PIECESLIST")) {
        //TODO Flag
        printf("Steine setzen");

        //Move Brick
    }else if(strstr(data->splited_reply[1], "@")) {
        printf("Stein auf %s setzen", data->splited_reply[1]);
        //TODO change gameState

        //Server erlaubt berechnung des nächsten Zuges
    }else if(strstr(data->splited_reply[1], "OKTHINKING")) {
        // thinking();
        printf("Berechne deinen Zug");

        //Game over
    }else if(strstr(data->splited_reply[1], "GAMEOVER")) {
        printf("das Spiel ist vorbei");
        quit = true;

        //Changed Game Pieces transfered
    }else if(strstr(data->splited_reply[1], "ENDPIECESLIST")) {
        //TODO Flag setzen
        printf("Alle Steine gelesen und gesetzt");

        //Gewinner Spiel

    }else if(strcmp(data->splited_reply[1], "PLAYER0WON")) {

        if (strcmp(data->splited_reply[2], "Yes")){
            printf("Player 0 gewinnt");
        } else{
            printf("Player 0 verliert");
        }

        //Gewinner Spiel
    }else if(strstr(data->splited_reply[1], "PLAYER1WON")) {
        if (strstr(data->splited_reply[2], "Yes")){
            printf("Player 1 gewinnt");
        } else{
            printf("Player 1 verliert");
        }
        //Quit Client and connection
    }else if(strstr(data->splited_reply[1], "QUIT")) {
        printf("Beende deinen Clienten ");
        quit = true;

        //new opponent player
    }else{
        printf("Nachricht konnte nicht erkannt werden oder ist für die aktuelle Phase nicht zulässig:\n %s", data->server_reply);
        quit = true;
    }
    return new_phase;
}

phase handle_draft(phase_data *data ){
    phase new_phase = _phase;

    if(strstr(data->splited_reply[1], "MOVEOK")) {
    }
    return new_phase;
}

phase run_phase( phase cur_phase, phase_data *data ) {
    return phase_table[ cur_phase ]( data );
}
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

/**
 * Tatsächliches senden der Nachricht
 */
int send_to_gameserver(int fd, char *message){
    printf("\n send: %s \n",message);
    return (int)write(fd, message, strlen(message));
}


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
