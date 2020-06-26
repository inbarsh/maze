//My code supports vertical and horizontal walls, traps and secret walls, multi targets, keys and doors.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maze.h"

const char *msg_normal[] = {"Press A/S/D/W to move, Q to quit"};
const char *msg_quit[] = {"Goodbye! come back soon :)", "Goodbye!", "Hope you had fun, see you soon ;)", "byeeeee", "bayush", "So Long, and Thanks for All the Fish"};
const char *msg_unknown[] = {"Unknown key pressed", "Only A/S/D/W Buttons are allowed or Q for quiting"};
const char *msg_hit[] = {"Ouch!", "Sorry dude, you cannot go through a wall", "Be careful you may be injured"};
const char *msg_win[] = {"Hell yeah! you win!", "You are the best! you win", "Congratulations! you defeated the maze", "Pass on what you have learned"};
const char *msg_key_used[] = {"You used one key to open a door", "key is used"};
const char *msg_door_no_key[] = {"You dont have any keys to open the door", "Player without keys cannot enter a door tile"};
const char *msg_trap[] = {"The greatest teacher, failure is.", "Boom! You lose..."};
const char *msg_hollow_wall[] = {"This wall sounds hollow..."};
const char *msg_vertical[] = {"Vertical wall tile cannot be entered from left/right"};
const char *msg_horizontal[] = {"Horizontal wall tile cannot be entered from top/bottom"};

int main(int argc, char *argv[]) 
{
    set_terminal(true);
    maze *m = read_maze(argv[1]);
    print_maze(*m);
    handle_input(m);
    set_terminal(false);
    delete_maze(m);
    return 0;
}

#define puts_no_newline(s) fputs(s, stdout)

bool handle_input(maze *maze)
{
        bool game_over = false;
        do {

        // get user input
        int key = getchar();
        switch(key) {
            case EOF:
            case 'q':
            case 'Q':   // exit game
                strcpy(maze->message,MSG_QUIT);
                game_over = true;
                break;
            case 'a':
            case 'A':   // go left
                if (0 == maze->player_location->col || 1 == *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col-1)) 
                    strcpy(maze->message,MSG_HIT);
                else if (4 == *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col-1))
                    strcpy(maze->message,MSG_VERTICAL);
                else if (9 == *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col-1) && maze->available_keys == 0)
                    strcpy(maze->message,MSG_DOOR_NO_KEY);
                else
                    maze->player_location->col--;
                break;
            case 'd':
            case 'D':   // go right
                if (maze->size[1] == maze->player_location->col+1 || 1 == *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col+1)) 
                    strcpy(maze->message,MSG_HIT);
                else if (4 == *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col+1))
                    strcpy(maze->message,MSG_VERTICAL);
                else if (9 == *(maze->arr + maze->player_location->row+maze->size[1] + maze->player_location->col+1) && maze->available_keys == 0)
                    strcpy(maze->message,MSG_DOOR_NO_KEY);
                else
                    maze->player_location->col++;
                break;
            case 'w':
            case 'W':   // go up
                if (0 == maze->player_location->row || 1 == *(maze->arr + (maze->player_location->row-1)*maze->size[1] + maze->player_location->col)) 
                    strcpy(maze->message,MSG_HIT);
                else if (5 == *(maze->arr + (maze->player_location->row-1)*maze->size[1] + maze->player_location->col))
                    strcpy(maze->message,MSG_HORIZONTAL);
                else if (9 == *(maze->arr + (maze->player_location->row-1)*maze->size[1] + maze->player_location->col) && maze->available_keys == 0)
                    strcpy(maze->message,MSG_DOOR_NO_KEY);
                else
                    maze->player_location->row--;
                break;
            case 's':
            case 'S':   // go down
                if (maze->size[0] == maze->player_location->row+1 || 1 == *(maze->arr + (maze->player_location->row+1)*maze->size[1] + maze->player_location->col)) 
                    strcpy(maze->message,MSG_HIT);
                else if (5 == *(maze->arr + (maze->player_location->row+1)*maze->size[1] + maze->player_location->col))
                    strcpy(maze->message,MSG_HORIZONTAL);
                else if (9 == *(maze->arr + (maze->player_location->row+1)*maze->size[1] + maze->player_location->col) && maze->available_keys == 0)
                    strcpy(maze->message,MSG_DOOR_NO_KEY);
                else
                    maze->player_location->row++;
                break;
            default:
                strcpy(maze->message,MSG_UNKNOWN);
        }
        switch (*(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col)) {
            case 2:
                strcpy(maze->message,MSG_TRAP);
                game_over = true;
                break;
            case 3:
                strcpy(maze->message,MSG_HOLLOW_WALL);
                break;
            case 4:
                strcpy(maze->message,MSG_VERTICAL);
                break;
            case 5:
                strcpy(maze->message,MSG_HORIZONTAL);
                break;
            case 6:
                maze->targets_counter--;
                *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col) = 7;
                if (0 == maze->targets_counter) {
                    strcpy(maze->message,MSG_WIN);
                    game_over = true;
                }
                break;
            case 8:
                maze->available_keys++;
                *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col) = 0;
                break;
            case 9:
                maze->available_keys--;
                strcpy(maze->message,MSG_KEY_USED);
                *(maze->arr + maze->player_location->row*maze->size[1] + maze->player_location->col) = 0;
                break;
        }
        print_maze(*maze);
        
    } while (!game_over);
    return game_over;
}

void print_maze(maze maze)
{
    static unsigned frame_num; // this variable retains its value between calls
    // clear terminal
    puts_no_newline(CLEAR_SCREEN);
    // print frame number and message
    printf("%-*.*s%4d Available keys: %d\n", maze.size[1]-2, maze.size[1]-2, maze.message, frame_num++, maze.available_keys);
    // print header row
    print_horizontal_wall (maze.size[1]);
    // print maze rows (including player)
    for (int i = 0; i < maze.size[0]; i++) {
        print_tile(S_WALL, COLOR_GRAY);
        for (int j = 0; j < maze.size[1]; j++) {
            if (6 == *(maze.arr + i*maze.size[1] + j)) {
                if (i == maze.player_location->row && j == maze.player_location->col)
                    print_tile(S_TARGET, COLOR_BLUE);
                else
                    print_tile(S_TARGET, COLOR_PURPLE);
            } else if (1 == *(maze.arr + i*maze.size[1] + j)) {
                print_tile(S_WALL, COLOR_GRAY);
            } else if (3 == *(maze.arr + i*maze.size[1] + j)) {
                if (i == maze.player_location->row && j == maze.player_location->col)
                    print_tile(S_WALL, COLOR_BLUE);
                else
                    print_tile(S_WALL, COLOR_GRAY);
            } else if (4 == *(maze.arr + i*maze.size[1] + j)) {
                if (i == maze.player_location->row && j == maze.player_location->col)
                    print_tile(S_VERTICAL_WALL, COLOR_BLUE);
                else
                    print_tile(S_VERTICAL_WALL, COLOR_GRAY);
            } else if (5 == *(maze.arr + i*maze.size[1] + j)) {
                if (i == maze.player_location->row && j == maze.player_location->col)
                    print_tile(S_HORIZONTAL_WALL, COLOR_BLUE);
                else
                    print_tile(S_HORIZONTAL_WALL, COLOR_GRAY);
            } else if (7 == *(maze.arr + i*maze.size[1] + j)) {
                if (i == maze.player_location->row && j == maze.player_location->col)
                    print_tile(S_TARGET, COLOR_BLUE);
                else
                    print_tile(S_TARGET, COLOR_GREEN);
            } else if (8 == *(maze.arr + i*maze.size[1] + j)) {
                if (i == maze.player_location->row && j == maze.player_location->col)
                    print_tile(S_KEY, COLOR_BLUE);
                else
                    print_tile(S_KEY, COLOR_GRAY);
            } else if (9 == *(maze.arr + i*maze.size[1] + j)) {
                if (i == maze.player_location->row && j == maze.player_location->col)
                    print_tile(S_DOOR, COLOR_BLUE);
                else
                    print_tile(S_DOOR, COLOR_GRAY);
            } else if (i == maze.player_location->row && j == maze.player_location->col)
                print_tile(S_PLAYER, COLOR_BLUE);
            else
                puts_no_newline(S_FLOOR);
        }
        print_tile(S_WALL, COLOR_GRAY);
        putchar('\n');
    }
    // print footer row
    print_horizontal_wall (maze.size[1]);
}

void print_horizontal_wall(int cols)
{
    for (int j = 0; j < cols + 2; j++)
        print_tile(S_WALL, COLOR_GRAY);
    putchar('\n');
}

void print_tile(char *tile, char *color)
{
    char *str3 = (char *) malloc(1 + strlen(tile) + strlen(color) + strlen(COLOR_OFF));
    strcpy(str3, color);
    strcat(str3, tile);   
    strcat(str3, COLOR_OFF);
    puts_no_newline(str3);    
}

bool delete_maze(maze *maze)
{
    if (NULL == maze)
        return false;
    if (NULL != maze->arr)
        free(maze->arr);
    free(maze);
    return true;
}

maze *read_maze(const char *input_file)
{
    if (NULL == input_file) {
        perror("invalid file name");
        exit(EXIT_FAILURE);
    }
    FILE *fPtr = fopen(input_file, "r");
    
    char buffer[BUFFER_SIZE];
    int size[2], length = 0, player_counter = 0, row_counter = 0, c, i, bytesread;

    // fopen() return NULL if unable to open file in given mode
    if (NULL == fPtr) {
        perror("couldn't open file");
        exit(EXIT_FAILURE);
    }
    maze *maze = malloc(sizeof(*maze));
    if (NULL == maze) {
        return NULL;
    }
    if (NULL != (fgets(buffer, BUFFER_SIZE, fPtr))) {
        strcpy(maze->message,buffer);
    }
    if (NULL != (fgets(buffer, BUFFER_SIZE, fPtr))) {
        char* input1 = buffer;
        while (sscanf(input1, "%d%n", &c, &bytesread) > 0) {
            size[length++] = c;
            input1 += bytesread;
        }
    }
    maze->targets_counter = 0;
    maze->available_keys = 0;
    maze->size[0] = size[0];
    maze->size[1] = size[1];
    maze->player_location = malloc(sizeof(location));
    if (NULL == maze->player_location) {
        free(maze);
        return NULL;
    }
    maze->arr = (int *)malloc(maze->size[0] * maze->size[1] * sizeof(int)); 
    //Read line from source file
    while (NULL != (fgets(buffer, BUFFER_SIZE, fPtr))) {
        for (i = 0; buffer[i] != 0; i++) {
            char key = buffer[i];
            switch(key) {
                case '@':
                    player_counter++;
                    if (1 < player_counter) {
                        perror("file has more than one player");
                        exit(EXIT_FAILURE);
                    }
                    maze->player_location->row = row_counter;
                    maze->player_location->col = i;
                    break;
                case '$':
                    maze->targets_counter++;
                    *(maze->arr + row_counter*maze->size[1] + i) = 6;
                    break;
                case '&':
                    *(maze->arr + row_counter*maze->size[1] + i) = 8;
                    break;
                case '+':
                    *(maze->arr + row_counter*maze->size[1] + i) = 9;
                    break;
                case '#':
                    *(maze->arr + row_counter*maze->size[1] + i) = 1;
                    break;
                case '!':
                    *(maze->arr + row_counter*maze->size[1] + i) = 2;
                    break;
                case '?':
                    *(maze->arr + row_counter*maze->size[1] + i) = 3;
                    break;
                case '|':
                    *(maze->arr + row_counter*maze->size[1] + i) = 4;
                    break;
                case '-':
                    *(maze->arr + row_counter*maze->size[1] + i) = 5;
                    break;
                case ' ':
                case '\r':
                case '\n':
                case '\0':
                    *(maze->arr + row_counter*maze->size[1] + i) = 0;
                    break;
                default:
                    perror("the format is not correct");
                    exit(EXIT_FAILURE);
            }
        }
        row_counter++;
    }
    fclose(fPtr);
    if (0 == player_counter) {
        delete_maze(maze);
        perror("no player in file");
        exit(EXIT_FAILURE);
    }
    if (0 == maze->targets_counter) {
        delete_maze(maze);
        perror("no target in file");
        exit(EXIT_FAILURE);
    }
    return maze;
}