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
        sz += strlen(next);
        s = realloc(s, sz);
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

void scroll_playfield(char **playfield, char **dict) {
    swap_strings(&playfield[0], &playfield[1]);
    swap_strings(&playfield[1], &playfield[2]);
    free(playfield[2]);
    playfield[2] = string_from_dict(dict);
}

void initialize_screen(GameState *gs, Vector2 term_size) {
    int term_rows = term_size.x;
    int term_cols = term_size.y;

    char *blank_line = (char*)calloc(term_cols + 1, sizeof(char));
    memset(blank_line, ' ', term_cols);
    char *pad = (char*)calloc(term_cols + 1, sizeof(char));
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

void update_timer(Timer *timer) {
    timer->seconds--;
    sprintf(timer->string, "%02ld:%02ld", timer->seconds / 60, timer->seconds % 60);
    cursor_to(timer->pos);
    printf("%s%s", FG_WHITE, timer->string);
    fflush(stdout);
}
