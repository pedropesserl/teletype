#ifndef TELETYPE_H_
#define TELETYPE_H_

#include <time.h> // time_t
#include "libtermio.h" // Vector2

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
    int cursor;
    char **dict;
    char **playfield;
} GameState;

void free_game(GameState *gs);

char **read_dict_from_file(const char *dict_path);

char *string_from_dict(char **dict);

char **initialize_playfield(char **dict);

void scroll_playfield(char **playfield, char **dict);

void initialize_screen(GameState *gs, Vector2 term_size);

void update_timer(Timer *timer);

#endif // TELETYPE_H_
