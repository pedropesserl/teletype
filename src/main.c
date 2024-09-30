#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libtermio.h"

#define MEM_ERR do { \
        fprintf(stderr, "ERROR: Could not allocate memory: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1); \
    } while (0)
#define FOPEN_ERR(path) do { \
        fprintf(stderr, "ERROR: Could not open file %s\n", (path)); \
        exit(1); \
    } while (0)

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

int main() {
    srand(0);

    char **dict = malloc(1000 * sizeof(char*));
    if (!dict) {
        MEM_ERR;
    }
    const char *dict_path = "resources/dict_sem_acentos.txt";
    FILE *dict_file = fopen(dict_path, "r");
    if (!dict_path) {
        FOPEN_ERR(dict_path);
    }
    size_t len = 0;
    for (int i = 0; getline(&dict[i], &len, dict_file) != -1; i++) {
        // replace \n with ' '
        dict[i][strlen(dict[i]) - 1] = ' ';
    }

    char **playfield = malloc(3 * sizeof(char*));
    if (!playfield) {
        MEM_ERR;
    }

    for (int i = 0; i < 3; i++) {
        playfield[i] = string_from_dict(dict);
    }
    int idx_current_string = 0;
    for (;;) {
        for (int i = 0; i < 3; i++) {
            printf("%s\n", playfield[(idx_current_string + i) % 3]);
        }
        printf("\n");

        free(playfield[idx_current_string]);
        playfield[idx_current_string] = string_from_dict(dict);
        idx_current_string = (idx_current_string + 1) % 3;

        getchar();
    }

    for (int i = 0; i < 3; i++) {
        free(playfield[i]);
    }
    free(playfield);
    for (int i = 0; i < 1000; i++) {
        free(dict[i]);
    }
    free(dict);
    return 0;
}
