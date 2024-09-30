#include <stdio.h>
#include <stdlib.h>
#include "libtermio.h"
#include "teletype.h"

int main() {
    srand(0);

    char **dict = read_dict_from_file("resources/dict_ascii.txt");
    char **playfield = initialize_playfield(dict);

    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);
    initialize_screen(term_cols, playfield);

    for (int i = 10; i--;) {
        update_playfield(playfield, dict);
        getchar();

        cursor_up(7);
        cursor_left(term_cols);
        initialize_screen(term_cols, playfield);
    }

    destroy_string_array(&playfield, 3);
    destroy_string_array(&dict, 1000);
    return 0;
}
