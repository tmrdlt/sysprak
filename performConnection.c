
#include "performConnection.h"
#include "shared_memory_segment.h"
#define BUFFERSIZE 1024

char in_buffer[BUFFERSIZE];
size_t in_buffer_used =0;

bool quit = false;

player *_player;
player *opponent_players;
game *gameparams;
phase _phase = PROLOG;
prolog_data _prolog_data;
int _player_number;
char *shm_data;

char *_game_id;	

phase_func_t* const phase_table[3] = {
    handle_prolog, handle_course, handle_draft
};



/*
 * performConnection holds client connection to Gameserver.
 */
void performConnection(int fd, char *game_id, int player_number, int shm_id){
 
    shm_data = address_shm (shm_id);
    
    _player_number= player_number;
    
    _game_id = game_id;
    gameparams = malloc(sizeof(gameparams));
    if(gameparams==NULL){
        printf("Memorry Allocation 'Gameparams' failed\n");
        disconnect(fd);
        exit(EXIT_FAILURE);
    }

    
    while(1){
        
        size_t buffer_remain = sizeof(in_buffer) - in_buffer_used;
        if (buffer_remain == 0) {
            fprintf(stderr, "Line exceeded buffer length!\n");
            abort();
        }
        
        ssize_t recv_size = recv(fd, &in_buffer[in_buffer_used], buffer_remain, 0);
        
        // = malloc(sizeof(char)*100);
        //empfange neue Nachricht
        
        if(recv_size < 0){
            perror("Receiving Message failed \n");
            break;
            
            //Teste auf Fehlemeldung vom Server
        }else if(recv_size>0){
            in_buffer_used += recv_size;
            
            char *line_start = in_buffer;
            char *line_end;
            while ( (line_end = (char*)memchr((void*)line_start, '\n', in_buffer_used - (line_start - in_buffer))))
            {
                *line_end = 0;
                process_line(line_start, fd);
                line_start = line_end + 1;

            }
            /* Shift buffer down so the unprocessed data is at the start */
            in_buffer_used -= (line_start - in_buffer);
            memmove(in_buffer, line_start, in_buffer_used);
        }
        
        
        if(quit){
            printf("Quit Flag während des Handlings gesetzt ... beende Client\n");
            disconnect(fd);
            return;
        } else{
            printf("no Flag\n");
            
        }
    }
}

/**
 * Verarbeite die nächste komplett gelesene Zeile (Eine Nachricht)
 */
void process_line(char *server_reply, int fd){
    printf("S: %s \n", server_reply);
    
    if (server_reply[0] == '-'){
        printf("Server sendet Negativnachricht: \n");
        //printf(" %s \n", server_reply);
        quit = true;
        //Teste ob die Nachricht valide ist
    }else if (server_reply[0] == '+'){
        
        
        char **splited_reply;
        int count_elements = split(server_reply, ' ', &splited_reply);
        
        phase_data *data = (phase_data*) malloc(sizeof(phase_data));
        
        if(data == NULL){
            perror("Memory allocation Phase data failed \n");
            quit = true;
            return;
        }
        
        data->count_elements = count_elements;
        data->splited_reply = splited_reply;
        data->server_reply = server_reply;
        data->fd = fd;
        
        
        _phase = run_phase(_phase,data);
        
        free(data);
        free(splited_reply);
        // Beende Verbindung wenn Nachricht invalid
    }else{
        printf("Error - Ungültiges Zeichen zum Beginn der Nachricht! %s\n",server_reply);
        quit = true;
        //printf(" %s \n", server_reply);
        
    }
}


/**
 * Handle Phase Prolog
 */
phase handle_prolog(phase_data *data ){
    phase new_phase = _phase;
    
    if(strstr(data->splited_reply[1], "MNM")) {
        if(_prolog_data.version_check != 1){
            printf("Versions Nummer des MNM Webservers: %s\n" , data->splited_reply[3]);
            //Check if fitting Versions
            data->splited_reply[3]++;
            double version_server = string_to_float(data->splited_reply[3]);
            double version_client = string_to_float(version);
            
            if((int)version_server != (int) version_client){
                quit = true;
                printf("Version des Clienten: %s ist nicht mit der des Servers: v%s kompatibel!\n",version ,  data->splited_reply[3] );
                return new_phase;
            }
            
            // send Clients version
            char *msg = create_msg_version();
            if( send_to_gameserver(data->fd, msg) < 0){
                printf("Version des Clienten konnte nicht gesendet werden!\n");
                quit = true;
            }
            _prolog_data.version_check = 1;
        }else{
            printf("Versionsnummer bereits erhalten!\n");
            quit= true;
        }
        
        
        // Server allows entering Game
    }else if(strstr(data->splited_reply[1], "PLAYING")) {
        if(_prolog_data.playing != 1){
            if(!strstr(data->splited_reply[2], _config.gamekindname)){
                printf("Falsches Spiel!\n");
                quit = true;
                return new_phase;
                
            }else{
                printf("Client und Server Spiel-Typ stimmen überein!\n");
            }
            
            _prolog_data.playing =1;
        }else{
            printf("Spiel bereits gecheckt!\n");
            quit= true;
        }
        
        // Player id- name allocation
    }else if(strstr(data->splited_reply[1], "YOU")) {
        if(_prolog_data.you != 1){
            
            _player= (player*) malloc(sizeof(player*));
            _player->number = string_to_int(data->splited_reply[2]);
            _player->player_name = data->splited_reply[3];
            printf("Hi (%i) %s!\n" ,_player->number, _player->player_name);
            _prolog_data.you =1;
        } else{
            perror("Böser Server ... Player wurde bereits gestezt\n");
            quit = true;
        }
        
        // Count Players in Game
    }else if(strstr(data->splited_reply[1], "TOTAL")) {
        if(_prolog_data.players != 1){
            
            int players_in_game = string_to_int(data->splited_reply[2]);
            
            //set num players in gameparams
            opponent_players = (player*)malloc(sizeof(player)*players_in_game);
            if(opponent_players == NULL) perror("Players cant be allocated");
            gameparams->player_count = players_in_game;
            
            if(players_in_game != 1){
                printf("In dem von dir gewählten Spiel befinden sich nun %i Spieler\n" , players_in_game);
            }else{
                printf("In dem von dir gewählten Spiel befindet sich nun %i Spieler\n" , players_in_game);
            }
            _prolog_data.players =1;
        }else{
            printf("Spieler bereits übermittelt!\n");
            quit= true;
        }
        
        // All Players in Game transfered to Client
    }else if(strstr(data->splited_reply[1], "ENDPLAYERS")) {
        // Todo set Flag in Gameparams
        new_phase = COURSE;
        printf("All Players read, switch to next State! \n");
        
        //Neuer Spielerim Spiel
    } else if(data->count_elements == 4){
        if(_prolog_data.players ==1){
            char *end;
            long nr_l = strtol(data->splited_reply[1], &end, 1);
            
            int p_nr = (int)nr_l;
            player *p = (player*) malloc(sizeof(player));
            p->number = p_nr;
            p->player_name = data->splited_reply[2];
            long flag_l = strtol(data->splited_reply[3], &end, 1);
            int p_flag = (int)flag_l;
            p->flag = p_flag;
            
            if(p_flag == 1)
                printf("Spieler (%d) %s ist bereit\n" , p_nr, data->splited_reply[2]);
            else
                printf("Spieler (%d) %s ist noch nicht bereit\n" , p_nr, data->splited_reply[2]);
            
            *opponent_players = *p;
            opponent_players++;
            
            // int tmp = gameparams->player_count;
            
        }else{
            
            printf("Spieler werden nicht gesetzt!\n");
            quit= true;
            
        }
        
        // Name des Spiels
    }else if (data->count_elements == 2){
        if(_prolog_data.playing == 1){
            printf("Bot betritt das Spiel: %s!" , data->splited_reply[1]);
            
            //sende gewünschte Spielernummer (noch leer)
            char *message = create_msg_player(_player_number);
            if( send_to_gameserver(data->fd, message) < 0){
                perror("Initialisierung Spieler fehlgeschlagen\n");
                quit = true;
            }
        } else{
            printf("Bot bereits in einem Spiel\n");
            quit = true;
        }
        
        // Client Version wurde akzeptiert
    } else if (strstr(data->server_reply, "Client version accepted")){
        if(_prolog_data.version_accepted != 1){
            printf("Die aktuelle Version des Clienten wurde vom Gameserver akzeptiert! Jetzt gehts los!\n");
            // Sende die Game-ID zum Server

            char *id_msg = create_msg_id(_game_id);
            _prolog_data.version_accepted = 1;
            if( send_to_gameserver(data->fd, id_msg) < 0){
                perror("Fehler bei der Übertragung der Game Id!\n");
                quit = true;
            }
            
        }
        // Nicht erkannte Nachricht - fehler
    }else{
        printf("Nachricht konnte nicht erkannt werden oder ist für die aktuelle Phase nicht zulässig:\n %s \n", data->server_reply);
        quit = true;
    }
    
    return new_phase;
}

/**
 * Handle phase Spielverlauf
 */
phase handle_course(phase_data *data ){
    phase new_phase = _phase;
    if(strstr(data->splited_reply[1], "WAIT")) {
        
        printf("Warte auf Gameserver\n");
        if( send_to_gameserver(data->fd, create_msg_okwait()) < 0){
            perror("Quittung für Wait konnte nicht gesendet werden\n!");
            quit = true;
        }
        
        //Server allows Client to make next Move
    }else if(strstr(data->splited_reply[1], "MOVE")) {
        // TODO
        printf("Du bist am Zug und hast %s sekunden\n" ,data->splited_reply[2]);
        
        //Changed Gamestate - Server sends changed pieces
    }else if(strstr(data->splited_reply[1], "ENDPIECESLIST")) {
        //TODO Flag
        
        printf("Alle Steine gelesen und gesetzt\n");
        //Move Brick
    }else if(strstr(data->splited_reply[1], "@")) {
        printf("Stein auf %s setzen\n", data->splited_reply[1]);
        //TODO change gameState
        
        //Server erlaubt berechnung des nächsten Zuges
    }else if(strstr(data->splited_reply[1], "OKTHINKING")) {
        // thinking();
        printf("Berechne deinen Zug\n");
        
        //Game over
    }else if(strstr(data->splited_reply[1], "GAMEOVER")) {
        printf("das Spiel ist vorbei\n");
        quit = true;
        
        //Changed Game Pieces transfered
    }else if(strstr(data->splited_reply[1], "PIECESLIST")) {
        //TODO Flag setzen
        printf("Steine setzen\n");
        
        //Gewinner Spiel
    }else if(strstr(data->splited_reply[1], "PLAYER0WON")) {
        
        if (strstr(data->splited_reply[2], "Yes")){
            printf("Player 0 gewinnt\n");
        } else{
            printf("Player 0 verliert\n");
        }
        
        //Gewinner Spiel
    }else if(strstr(data->splited_reply[1], "PLAYER1WON")) {
        if (strstr(data->splited_reply[2], "Yes")){
            printf("Player 1 gewinnt\n");
        } else{
            printf("Player 1 verliert\n");
        }
        //Quit Client and connection
    }else if(strstr(data->splited_reply[1], "QUIT")) {
        printf("Beende deinen Clienten\n");
        quit = true;
        
        //new opponent player
    }else{
        printf("Nachricht konnte nicht erkannt werden oder ist für die aktuelle Phase nicht zulässig:\n %s", data->server_reply);
        quit = true;
    }
    return new_phase;
}

/**
 * Handle Phase Spielzug
 */
phase handle_draft(phase_data *data ){
    phase new_phase = _phase;
    
    if(strstr(data->splited_reply[1], "MOVEOK")) {
    }
    
    return new_phase;
}

/**
 * Ermittelt anhand der aktuellen Phase die Methode die ausgeführt werden soll
 */
phase run_phase( phase cur_phase, phase_data *data ) {
    return phase_table[ cur_phase ]( data );
}



/**
 * Tatsächliches senden der Nachricht
 */
int send_to_gameserver(int fd, char *message){
    printf("\n C: %s",message);
    int res =( int)send(fd, message, strlen(message),0);
    //printf("%d bytes übertragen \n" , res);
    if(res<0){
        printf("Nachricht konnte nicht gesendet werden");
        quit = true;
    }
    free(message); 
    
    return res;
}

/**
 *  Disconnect from Gameserver
 */
void disconnect(int fd){
    close(fd);
    free(_player);
    free(gameparams);
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
