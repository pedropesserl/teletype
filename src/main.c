#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libtermio.h"
#include "teletype.h"

int cursor;

int main() {
    srand(time(0));

    char **dict = read_dict_from_file("resources/dict_ascii.txt");
    char **playfield = initialize_playfield(dict);

    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);
    initialize_screen(term_cols, playfield);

    /* cursor = 0; */

    /* time_t timer = 30; */
    /* printf("%02ld:%02ld\n", timer / 60, timer % 60); */
    /* time_t now = time(NULL); */
    /* while (timer > 0) { */
    /*     if (time(NULL) > now) { */
    /*         timer--; */
    /*         now = time(NULL); */
    /*         printf("%02ld:%02ld\n", timer / 60, timer % 60); */
    /*     } */
    /* } */

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
