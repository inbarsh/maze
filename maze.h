#include <stdbool.h>

#define BUFFER_SIZE 256 
#define MSG_LEN     100

#define MSG_NORMAL  msg_normal[rand() % (sizeof(msg_normal) / sizeof(msg_normal[0]))]
#define MSG_QUIT msg_quit[rand() % (sizeof(msg_quit) / sizeof(msg_quit[0]))]
#define MSG_UNKNOWN msg_unknown[rand() % (sizeof(msg_unknown) / sizeof(msg_unknown[0]))]
#define MSG_HIT msg_hit[rand() % (sizeof(msg_hit) / sizeof(msg_hit[0]))]
#define MSG_WIN msg_win[rand() % (sizeof(msg_win) / sizeof(msg_win[0]))]
#define MSG_DOOR msg_door[rand() % (sizeof(msg_door) / sizeof(msg_door[0]))]
#define MSG_NOT msg_not[rand() % (sizeof(msg_not) / sizeof(msg_not[0]))]
#define MSG_TRAP msg_trap[rand() % (sizeof(msg_trap) / sizeof(msg_trap[0]))]
#define MSG_HOLLOW_WALL msg_hollow_wall[rand() % (sizeof(msg_hollow_wall) / sizeof(msg_hollow_wall[0]))]
#define MSG_VERTICAL msg_vertical[rand() % (sizeof(msg_vertical) / sizeof(msg_vertical[0]))]
#define MSG_HORIZONTAL msg_horizontal[rand() % (sizeof(msg_horizontal) / sizeof(msg_horizontal[0]))]
#define MSG_DOOR_NO_KEY msg_door_no_key[rand() % (sizeof(msg_door_no_key) / sizeof(msg_door_no_key[0]))]
#define MSG_KEY_USED msg_key_used[rand() % (sizeof(msg_key_used) / sizeof(msg_key_used[0]))]

typedef struct {
    unsigned row;
    unsigned col;
} location;

typedef struct {
    char message[MSG_LEN];
    location *player_location;
    int targets_counter;
    int available_keys;
    int size[2];
    int *arr;
} maze;

void set_terminal(bool start);
void print_maze(maze maze);
void print_horizontal_wall(int cols);
bool handle_input(maze *maze);
bool delete_maze(maze *maze);
void print_tile(char *tile, char *color);
maze *read_maze(const char *input_file);

/************************************
 * TERMINAL CODES ("MAGIC STRINGS") *
 ************************************/

#define COLOR_OFF       "\e[0m"
#define COLOR_BLACK     "\e[0;30m"
#define COLOR_GRAY      "\e[1;30m"
#define COLOR_RED       "\e[0;31m"
#define COLOR_LT_RED    "\e[1;31m"
#define COLOR_GREEN     "\e[0;32m"
#define COLOR_LT_GREEN  "\e[1;32m"
#define COLOR_BROWN     "\e[0;33m"
#define COLOR_YELLOW    "\e[1;33m"
#define COLOR_BLUE      "\e[0;34m"
#define COLOR_LT_BLUE   "\e[1;34m"
#define COLOR_PURPLE    "\e[0;35m"
#define COLOR_LT_PURPLE "\e[1;35m"
#define COLOR_CYAN      "\e[0;36m"
#define COLOR_LT_CYAN   "\e[1;36m"
#define COLOR_LT_GRAY   "\e[0;37m"
#define COLOR_WHITE     "\e[1;37m"

#define CLEAR_SCREEN    "\e[2J\e[1;1H"

/*****************************
 * SYMBOLS FOR GAME ELEMENTS *
 *****************************/

#define S_FLOOR           " "
#define S_TARGET          "$"
#define S_PLAYER          "@"
#define S_WALL            "#"
#define S_TRAP            "!"
#define S_SECRET_WALL     "?"
#define S_VERTICAL_WALL   "|"
#define S_HORIZONTAL_WALL "-"
#define S_KEY             "&"
#define S_DOOR            "+"