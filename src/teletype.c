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

void destroy_string_array(char ***arr, int size) {
    for (int i = 0; i < size; i++) {
        free((*arr)[i]);
    }
    free(*arr);
    *arr = NULL;
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

static void center_line(char *line, char *pad, int pad_size) {
    int line_len = strlen(line);
    assert(line_len + 1 <= pad_size && "Length of line to center must not be bigger than padding space available.");
    memset(pad, ' ', pad_size - 1);
    memcpy(pad + (pad_size - line_len)/2 - 1, line, line_len);
    pad[pad_size - 1] = '\0';
    printf("%s\n", pad);
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

void initialize_screen(int term_cols, char **playfield) {
    char *blank_line = (char*)calloc(term_cols + 1, sizeof(char));
    char *pad = (char*)calloc(term_cols + 1, sizeof(char));
    memset(blank_line, ' ', term_cols);

    printf("%s%s", BG_GREY, FG_LIGHT_GREY);
    printf("%s\n", blank_line);
    printf("%s\n", blank_line);
    center_line(playfield[0], pad, term_cols + 1);
    center_line(playfield[1], pad, term_cols + 1);
    center_line(playfield[2], pad, term_cols + 1);
    printf("%s\n", blank_line);
    printf("%s", blank_line);

    free(blank_line);
    free(pad);
}

void update_playfield(char **playfield, char **dict) {
    swap_strings(&playfield[0], &playfield[1]);
    swap_strings(&playfield[1], &playfield[2]);
    free(playfield[2]);
    playfield[2] = string_from_dict(dict);
}
