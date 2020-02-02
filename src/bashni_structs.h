
#ifndef bashni_structs_h
#define bashni_structs_h

#define COURT_SIZE 8
#define FIELD_ID_SIZE 3
#define TOWERS_SIZE 25
#define THINKING 1
#define NOT_THINKING 0
#define MAX_DRAFT_LENGTH 38

typedef struct {
    int fd;
    char *server_reply;
    char splited_reply[10][25];
    int count_elements;
}phase_data;

typedef struct {
    int value;
    char move_id[MAX_DRAFT_LENGTH];
}move_value;

typedef struct {
    int set_game_name;
    int set_players;
}prolog_data;

typedef struct{
    char *player_name;
    int number;
    int flag;
} player;

typedef struct{
    char field_id[FIELD_ID_SIZE];
    char towers[TOWERS_SIZE];
} field;

typedef struct {
    char *game_name;
    int player_number;
    int player_count;
    int process_id_thinker;
    int process_id_connector;
    int flag_thinking;
    int move_time;
    field court[COURT_SIZE][COURT_SIZE];
    int players_shm_ids;
}game_state;

#endif /* bashni_structs_h */
