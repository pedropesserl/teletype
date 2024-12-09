#ifndef TELETYPE_H_
#define TELETYPE_H_

#include <stdbool.h>
#include <time.h>
#include "libtermio.h"

#define MEM_ERR do { \
        fprintf(stderr, "ERROR: Could not allocate memory: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1); \
    } while (0)
#define FOPEN_ERR(path) do { \
        fprintf(stderr, "ERROR: Could not open file %s:\n", (path)); \
        exit(1); \
    } while (0)

#define BG_GREY       "\033[48;5;234m"
#define FG_WHITE      "\033[38;5;15m"
#define FG_LIGHT_GREY "\033[38;5;243m"

#define GAME_HEIGHT 5

typedef struct Timer_s {
    time_t seconds;
    Vector2 pos;
    char *string;
} Timer;

typedef struct GameState_s {
    Timer timer;
    Vector2 cursor_pos;
    char **dict;
    char **playfield;
    bool started;
    int word_count;
    bool no_error;
} GameState;

void free_game(GameState *gs);

char **read_dict_from_file(const char *dict_path);

char *string_from_dict(char **dict);

char **initialize_playfield(char **dict);

void scroll_playfield(GameState *gs);

void initialize_screen(GameState *gs);

void restart(GameState *gs);

void update_timer(GameState *gs);

void print_typed_char(GameState *gs, char typed_char);

Vector2 next_cursor_pos(GameState *gs);

#endif // TELETYPE_H_
