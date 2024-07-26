#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtermio.h"

#define MEM_ERR do { \
        fprintf(stderr, "ERROR: Could not allocate memory: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1); \
    } while (0)
#define FOPEN_ERR(path) do { \
        fprintf(stderr, "ERROR: Could not open file %s\n", (path)); \
        exit(1); \
    } while (0)

int main() {
    char **dict = malloc(1000 * sizeof(char*));
    if (!dict) {
        MEM_ERR;
    }
    const char *dict_path = "dict.txt";
    FILE *dict_file = fopen(dict_path, "r");
    if (!dict_path) {
        FOPEN_ERR(dict_path);
    }
    size_t len = 0;
    for (int i = 0; getline(&dict[i], &len, dict_file) != -1; i++);

    for (int i = 0; i < 1000; i++) {
        printf("%s", dict[i]);
    }
    return 0;
}
