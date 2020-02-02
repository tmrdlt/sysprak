#include "performConnection.h"
#include "shared_memory_segment.h"

#define BUFFERSIZE 1024
#define MAX_EVENTS 64

fd_set readfds;
size_t in_buffer_used =0;

player _player;
game_state *_game_state;
phase _phase = PROLOG;
prolog_data _prolog_data;
player *players;

int fd_pipe_thinker;
int retval;
int max_fd;
int game_shm_id;

char in_buffer[BUFFERSIZE];
bool quit = false;

phase_func_t* const phase_table[3] = {
    handle_prolog, handle_course, handle_draft
};

/*
 * performConnection holds client connection to Gameserver.
 */
void performConnection(int fd, int _shm_id){

    game_shm_id = _shm_id;
    _game_state = address_shm(_shm_id);

    while(1){

        size_t buffer_remain = sizeof(in_buffer) - in_buffer_used;
        if (buffer_remain == 0) {
            fprintf(stderr, "Line exceeded buffer length!\n");
            abort();
        }

        //empfange neue Nachricht
        ssize_t recv_size = recv(fd, &in_buffer[in_buffer_used], buffer_remain, 0);

        if(recv_size < 0){
            perror("Receiving Message failed \n");
            break;

            //Teste auf Fehlemeldung vom Server
        }else if(recv_size>0){
            in_buffer_used += recv_size;

            char *line_start = in_buffer;
            char *line_end;
            while ( (line_end = (char*)memchr((void*)line_start, '\n', in_buffer_used - (line_start - in_buffer)))){
                *line_end = 0;
                process_line(line_start, fd);
                line_start = line_end + 1;

            }
            /* Shift buffer down so the unprocessed data is at the start */
            in_buffer_used -= (line_start - in_buffer);
            memmove(in_buffer, line_start, in_buffer_used);
        }

        if(quit){
            printf("\n...Beende Clienten...\n");
            disconnect(fd);
            return;
        }
    }
}


/**
 * Verarbeite die nächste komplett gelesene Zeile (Eine Nachricht)
 */
void process_line(char *server_reply, int fd){

    if (server_reply[0] == '-'){
        handle_negativs(server_reply);
        quit = true;
        //Teste ob die Nachricht valide ist
    }else if (server_reply[0] == '+'){

        int count = count_elements(server_reply, ' ');

        phase_data data;// = (phase_data*) malloc(sizeof(phase_data));

        data.count_elements = count;
        int i = 0;

        char copy_reply[128];
        strcpy(copy_reply, server_reply);

        char *act_word;
        act_word = strtok(copy_reply, " ");

        strcpy(data.splited_reply[i], act_word);

        i++;

        while (i < count ){
            act_word = strtok(NULL , " ");
            strcpy(data.splited_reply[i], act_word);
            i++;
        }

        data.server_reply = server_reply;
        data.fd = fd;

        _phase = run_phase(_phase, &data);

        // Beende Verbindung wenn Nachricht invalid
    }else{
        printf("Error - Ungültiges Zeichen zum Beginn der Nachricht! %s\n",server_reply);
        quit = true;
    }
}


/**
 * Handle Phase Prolog
 */
phase handle_prolog(phase_data *data ){
    phase new_phase = _phase;

    if(strstr(data->splited_reply[1], "MNM")) {

        printf("S: Willkommen auf dem MNM Gameserver %s. Erwarte Verbindungen.\n" , data->splited_reply[3]);

        // send Clients version
        char msg[MAX_MESSAGE_LENGTH];
        create_msg_version(msg);
        if( send_to_gameserver(data->fd, msg) < 0){
            printf("Version des Clienten konnte nicht gesendet werden!\n");
            quit = true;
        }

        // Server allows entering Game
    }else if(strstr(data->splited_reply[1], "PLAYING")) {
        if(!strstr(data->splited_reply[2], _config.gamekindname)){
            printf("S: Falsches Spiel!\n");
            quit = true;
            return new_phase;

        }else{
            printf("S: Client und Server Spiel-Typ stimmen überein!\n");

            _prolog_data.set_game_name = 1;
        }

    }else if (_prolog_data.set_game_name == 1){
        //ließ game-name
        char *game_name= data->server_reply;
        game_name++;
        game_name++;
        printf("S: Client betritt das Spiel '%s'\n" , game_name);
        strcpy(_game_state->game_name , game_name);

        // sende gewünschte Spielernummer (noch leer)
        char message[MAX_MESSAGE_LENGTH];

        create_msg_player(_game_state->player_number, message);
        if( send_to_gameserver(data->fd, message) < 0){
            perror("Initialisierung Spieler fehlgeschlagen\n");
            quit = true;
        }

        _prolog_data.set_game_name = 0;

        // Player id- name allocation
    }else if(strstr(data->splited_reply[1], "YOU")) {

        _player.number = string_to_int(data->splited_reply[2]);
        _player.player_name = data->splited_reply[3];
        printf("S: Du bist Spieler %i '%s'\n" ,_player.number, _player.player_name);
        _game_state->player_number = _player.number;
        // Count Players in Game
    }else if(strstr(data->splited_reply[1], "TOTAL")) {

        int players_in_game = string_to_int(data->splited_reply[2]);

        //shm segment für spieler anlegen
        int id_player_shm = shm_id(sizeof(player)*players_in_game);

        _game_state->players_shm_ids = id_player_shm;
        _game_state->player_count = players_in_game;
        players = address_shm(id_player_shm);
        players[_player.number]= _player;

        if(players_in_game != 1){
            printf("S: Im Spiel befinden sich nun %i Spieler\n" , players_in_game);
        }else{
            printf("S: Im Spiel befindet sich nun %i Spieler\n" , players_in_game);
        }

        _prolog_data.set_players = 1;

        // All Players in Game transfered to Client
    }else if(strstr(data->splited_reply[1], "ENDPLAYERS")) {
        new_phase = COURSE;
        printf("S: Alle Spieler bereit, das Spiel beginnt\n");

        _prolog_data.set_players = 0;

        // Client Version wurde akzeptiert
    }else if(_prolog_data.set_players == 1 ){

        char *p_nr_str =  data->splited_reply[1];
        int p_flag = string_to_int(data->splited_reply[data->count_elements-1]);
        int p_nr = string_to_int(p_nr_str);

        char name[28];
        strcpy(name, data->splited_reply[2]);
        if (data->count_elements > 4){
            for(int i = 3 ; i < data->count_elements-1; i ++){
                strcat(name, " ");
                strcat(name, data->splited_reply[i]);
            }
        }
        players[p_nr].player_name = name;

        if(p_flag == 1)
            printf("S: Spieler %d '%s' ist bereit\n", p_nr, name);
        else
            printf("S: Spieler %d '%s' ist noch nicht bereit\n", p_nr, name);

    }else if (strstr(data->server_reply, "Client version accepted")){
        printf("S: Version des Clients wurde vom Gameserver akzeptiert! Bitte GameID senden um Spiel beizutreten.\n");
        // Sende die Game-ID zum Server

        char id_msg[MAX_MESSAGE_LENGTH];
        create_msg_id(_game_state->game_name, id_msg);

        if( send_to_gameserver(data->fd, id_msg) < 0){
            perror("Fehler bei der Übertragung der Game Id!\n");
            quit = true;
        }
    }
    // Nicht erkannte Nachricht - fehler
    else{
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

        printf("S: Warte auf Gegenspieler\n");
        char msg[MAX_MESSAGE_LENGTH];
        create_msg_okwait(msg);
        if( send_to_gameserver(data->fd, msg) < 0){
            perror("Quittung für Wait konnte nicht gesendet werden\n!");
            quit = true;
        }

        //Server allows Client to make next Move
    }else if(strstr(data->splited_reply[1], "MOVE")) {

        int move_time = string_to_int(data->splited_reply[2]);

        _game_state->move_time = move_time;

        new_phase = DRAFT;

        printf("S: Du bist am Zug und hast %d Millisekunden\n" , move_time);

        //Changed Gamestate - Server sends changed pieces
    }else if(strstr(data->splited_reply[1], "ENDPIECESLIST")) {

        print_court(_game_state->court, COURT_SIZE, _game_state->player_number);

        //Move Brick
    }else if(strstr(data->splited_reply[1], "@")) {
        //Change gameState
        set_draft (_game_state->court, data->splited_reply[1]);

        //Game over
    }else if(strstr(data->splited_reply[1], "GAMEOVER")) {
        printf("S: Das Spiel ist vorbei\n");

        //Changed Game Pieces transfered
    }else if(strstr(data->splited_reply[1], "PIECESLIST")) {

        set_court(_game_state->court, COURT_SIZE, _game_state->player_number);
        printf("\nS: Aktuelles Spielfeld:\n");

        //Gewinner Spiel
    }else if(strstr(data->splited_reply[1], "PLAYER0WON")) {

        if (strstr(data->splited_reply[2], "Yes")){
            printf("S: Spieler 0 gewinnt\n");
        } else{
            printf("S: Spieler 0 verliert\n");
        }

        //Gewinner Spiel
    }else if(strstr(data->splited_reply[1], "PLAYER1WON")) {
        if (strstr(data->splited_reply[2], "Yes")){
            printf("S: Spieler 1 gewinnt\n");
        } else{
            printf("S: Spieler 1 verliert\n");
        }
        //Quit Client and connection
    }else if(strstr(data->splited_reply[1], "QUIT")) {
        printf("S: Der Client kann beendet werden\n");
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
        new_phase = COURSE;
        printf("S: Zug akzeptiert\n");

        _game_state->flag_thinking = NOT_THINKING;
    }     //Server erlaubt berechnung des nächsten Zuges
    else if(strstr(data->splited_reply[1], "OKTHINK")) {

        //_game_state->flag_thinking = THINKING;
        if (kill(getppid(), SIGUSR1) < 0) {
            perror ("Fehler bei Senden vom Signal).");
            exit(EXIT_FAILURE);
        }

        FD_ZERO(&readfds);
        FD_SET(data->fd, &readfds);
        FD_SET(fd_pipe_thinker, &readfds);

        if (data->fd < fd_pipe_thinker) {max_fd = fd_pipe_thinker;}
        else {max_fd = data->fd;}

        retval = select(max_fd, &readfds, NULL, NULL, NULL);
        if (retval == -1) perror("select()");

        if(FD_ISSET(data->fd, &readfds)) {
            quit = true;
        }

        char puffer[128];

        ssize_t size = read(fd_pipe_thinker, puffer, 128);
        if (size < 0){
            perror ("Fehler bei lesen aus pipe).");
            exit(EXIT_FAILURE);
        }
        puffer[size] = '\0';

        char play_msg[MAX_MESSAGE_LENGTH];
        create_msg_play(puffer, play_msg);

        if( send_to_gameserver(data->fd, play_msg) < 0){
            perror("Fehler bei der Übertragung der Game Id!\n");
            quit = true;
        }

    } else if(strstr(data->splited_reply[1], "ENDPIECESLIST")) {
        print_court(_game_state->court, COURT_SIZE, _game_state->player_number);
        _game_state->flag_thinking = THINKING;
        char message[MAX_MESSAGE_LENGTH];
        create_msg_thinking(message);
        if( send_to_gameserver(data->fd, message) < 0){
            perror("THINKING konnte nicht gesendet werden!\n");
            quit = true;
        }

        //Move Brick
    }else if(strstr(data->splited_reply[1], "@")) {
        //Change gameState
        set_draft (_game_state->court, data->splited_reply[1]);

        //Changed Game Pieces transfered
    }else if(strstr(data->splited_reply[1], "PIECESLIST")) {

        set_court(_game_state->court, COURT_SIZE, _game_state->player_number);
        printf("\nS: Aktuelles Spielfeld:\n");
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
int send_to_gameserver(int fd, char message[MAX_MESSAGE_LENGTH]){

    int res =( int)send(fd, message, strlen(message),0);

    if(res<0){
        printf("Nachricht konnte nicht gesendet werden");
        quit = true;
    }
    return res;
}

/**
 * Logging für Negativnachrichten
 */
void handle_negativs(char *reply){
    if(strstr(reply, "TIMEOUT")){
        if(strstr(reply,"be faster next time")){
            printf("S: Fehler: Timeout. Erlaubte Zugzeit überschritten.\n");
        } else{
            printf("S: Fehler: Timeout. Antworte schneller auf Nachrichten des Servers.\n");
        }
    }else if(strstr(reply, "No free player")){
        printf("S: Fehler: Die Spielernummer %d ist für das gewählte Spiel nicht verfügbar\n" , _game_state->player_number);
    }else if(strstr(reply , "Invalid move")){
        if(strstr(reply, "Weiterer Teilzug möglich")){
            printf("S: Fehler: Bei der Berechnung des Spielzugs wurde ein möglicher schlagender Teilzug vernachlässigt.\n");
        }else{
            printf("S: Fehler: Ungültiger Spielzug berechnet.\n");
        }
    }else if (strstr(reply, "Game does not exist")){
        printf("S: Fehler: Kein Spiel mit ID %s gefunden.\n" , _game_state->game_name);

    }else if(strstr(reply, "Client Version does not match server Version")) {
        printf("S: Fehler: Version des Clienten: v%s ist nicht mit der des Servers kompatibel.\n",version);

    } else
        printf("S: Fehler:%s\n", ++ reply);
}

/**
 *  Disconnect from Gameserver
 */
void disconnect(int fd){

    if(players != NULL){
        dettach_shm(players);
        int id_shm_player =_game_state->players_shm_ids;
        delete_shm(id_shm_player);
    }
    if(_game_state != NULL){
        dettach_shm(_game_state);
        delete_shm(game_shm_id);
    }
    if(fd > 0)
        close(fd);

}
