#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "teletype.h"

static void swap_strings(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

static void destroy_string_array(char ***arr, int size) {
    for (int i = 0; i < size; i++) {
        free((*arr)[i]);
    }
    free(*arr);
    *arr = NULL;
}

static void center_line(char *line, char *pad, int pad_size) {
    int line_len = strlen(line);
    assert(line_len + 1 <= pad_size && "Length of line to center must not be bigger than padding space available.");
    memset(pad, ' ', pad_size - 1);
    memcpy(pad + (pad_size - line_len)/2 - 1, line, line_len);
    pad[pad_size - 1] = '\0';
    printf("%s\n", pad);
}

static Vector2 screen_pos_from_cursor(GameState *gs) {
    Vector2 term_size = get_terminal_size();
    int term_rows = term_size.x, term_cols = term_size.y;
    return (Vector2){
        gs->cursor_pos.x + (term_rows - GAME_HEIGHT) / 2 + 3,
        gs->cursor_pos.y + (term_cols + 1 - strlen(gs->playfield[gs->cursor_pos.x])) / 2,
    };
}

char **read_dict_from_file(const char *dict_path) {
    char **dict = malloc(1000 * sizeof(char*));
    if (!dict) {
        MEM_ERR;
    }
    FILE *dict_file = fopen(dict_path, "r");
    if (!dict_path) {
        FOPEN_ERR(dict_path);
    }
    size_t len = 0;
    for (int i = 0; getline(&dict[i], &len, dict_file) != -1; i++) {
        // replace \n with ' '
        dict[i][strlen(dict[i]) - 1] = ' ';
    }
    return dict;
}

char *string_from_dict(char **dict) {
    char *s = calloc(1, sizeof(char));
    size_t sz = 0;
    while (sz < 70) {
        char *next = dict[rand() % 1000];
        sz += strlen(next) + 1;
        char *new_s = realloc(s, sz);
        if (!new_s) {
            MEM_ERR;
        }
        s = new_s;
        strcat(s, next);
    }
    // removing last space
    s[sz - 1] = '\0';
    return s;
}

char **initialize_playfield(char **dict) {
    char **playfield = malloc(3 * sizeof(char*));
    if (!playfield) {
        MEM_ERR;
    }
    for (int i = 0; i < 3; i++) {
        playfield[i] = string_from_dict(dict);
    }
    return playfield;
}

void free_game(GameState *gs) {
    destroy_string_array(&gs->playfield, 3);
    destroy_string_array(&gs->dict, 1000);
    free(gs->timer.string);
}

void scroll_playfield(GameState *gs) {
    // scroll lines
    swap_strings(&gs->playfield[0], &gs->playfield[1]);
    swap_strings(&gs->playfield[1], &gs->playfield[2]);
    free(gs->playfield[2]);
    gs->playfield[2] = string_from_dict(gs->dict);

    // print playfield
    Vector2 term_size = get_terminal_size();
    int term_rows = term_size.x;
    int term_cols = term_size.y;
    char *pad = calloc(term_cols + 1, sizeof(char));
    hide_cursor();
    cursor_to((Vector2){(term_rows - GAME_HEIGHT) / 2 + 3, 1});
    printf("%s", FG_WHITE);
    center_line(gs->playfield[0], pad, term_cols + 1);
    printf("%s", FG_LIGHT_GREY);
    center_line(gs->playfield[1], pad, term_cols + 1);
    center_line(gs->playfield[2], pad, term_cols + 1);
    free(pad);

    // restore cursor
    cursor_to(screen_pos_from_cursor(gs));
    show_cursor();
}

void initialize_screen(GameState *gs) {
    Vector2 term_size = get_terminal_size();
    int term_rows = term_size.x;
    int term_cols = term_size.y;

    char *blank_line = calloc(term_cols + 1, sizeof(char));
    memset(blank_line, ' ', term_cols);
    char *pad = calloc(term_cols + 1, sizeof(char));
    sprintf(gs->timer.string, "%02ld:%02ld", gs->timer.seconds / 60, gs->timer.seconds % 60);

    cursor_to((Vector2){1, 1});
    printf("%s", BG_GREY);
    for (int i = 0; i < (term_rows - GAME_HEIGHT) / 2; i++) {
        printf("%s", blank_line);
        cursor_down(1);
        cursor_left(term_cols);
    }
    printf("%s", FG_WHITE);
    center_line(gs->timer.string, pad, term_cols + 1);
    gs->timer.pos = (Vector2){(term_rows - GAME_HEIGHT) / 2 + 1, term_cols / 2 - 2};
    printf("%s", FG_LIGHT_GREY);
    printf("%s\n", blank_line);
    center_line(gs->playfield[0], pad, term_cols + 1);
    center_line(gs->playfield[1], pad, term_cols + 1);
    center_line(gs->playfield[2], pad, term_cols + 1);
    for (int i = (term_rows + GAME_HEIGHT) / 2 + 1; i < term_rows; i++) {
        printf("%s\n", blank_line);
    }
    printf("%s", blank_line);
    fflush(stdout);

    free(blank_line);
    free(pad);
}

void restart(GameState *gs) {
    gs->timer.seconds = 30;
    gs->cursor_pos = (Vector2){0};
    gs->playfield = initialize_playfield(gs->dict);
    gs->started = false;
    gs->word_count = 0;
    gs->no_error = true;
}

void update_timer(GameState *gs) {
    gs->timer.seconds--;
    sprintf(gs->timer.string, "%02ld:%02ld", gs->timer.seconds / 60, gs->timer.seconds % 60);
    cursor_to(gs->timer.pos);
    hide_cursor();
    printf("%s%s", FG_WHITE, gs->timer.string);
    fflush(stdout);
    cursor_to(screen_pos_from_cursor(gs));
    show_cursor();
}

void print_typed_char(GameState *gs, char typed_char) {
    cursor_to(screen_pos_from_cursor(gs));
    printf("%s%c", FG_WHITE, typed_char);
    fflush(stdout);
}

Vector2 next_cursor_pos(GameState *gs) {
    int cursor_row = gs->cursor_pos.x;
    int cursor_col = gs->cursor_pos.y;
    int current_row_length = (int)strlen(gs->playfield[cursor_row]);
    if (cursor_row == 0 && cursor_col == current_row_length - 1) {
        return (Vector2){
            .x = gs->cursor_pos.x + 1,
            .y = 0,
        };
    }
    if (cursor_col == current_row_length - 1) {
        scroll_playfield(gs);
        return (Vector2){
            .x = gs->cursor_pos.x,
            .y = 0,
        };
    }
    return (Vector2){
        .x = gs->cursor_pos.x,
        .y = gs->cursor_pos.y + 1,
    };
}
