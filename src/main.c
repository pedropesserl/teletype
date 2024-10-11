#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libtermio.h"
#include "teletype.h"

#include <assert.h>
#include <string.h>
static void center_line(char *line, char *pad, int pad_size) {
    int line_len = strlen(line);
    assert(line_len + 1 <= pad_size && "Length of line to center must not be bigger than padding space available.");
    memset(pad, ' ', pad_size - 1);
    memcpy(pad + (pad_size - line_len)/2 - 1, line, line_len);
    pad[pad_size - 1] = '\0';
    printf("%s\n", pad);
}



int main() {
    srand(time(0));

    GameState gs = {
        .timer = {
            .seconds = 10,
            .string = calloc(5 + 1, sizeof(char)),
        },
        .cursor = 0,
        .dict = read_dict_from_file("resources/dict_ascii.txt"),
        .playfield = initialize_playfield(gs.dict),
    };

    Vector2 term_size = get_terminal_size();
    initialize_screen(&gs, term_size);

    time_t time_control = time(NULL);
    while (gs.timer.seconds > 0) {
        if (time(NULL) > time_control) { // one second has passed
            time_control = time(NULL);
            update_timer(&gs.timer);
        }
        scroll_playfield(gs.playfield, gs.dict);

        // print_playfield() {
        Vector2 term_size = get_terminal_size();
        int term_rows = term_size.x;
        int term_cols = term_size.y;
        char *pad = calloc(term_cols + 1, sizeof(char));

        cursor_to((Vector2){(term_rows - GAME_HEIGHT) / 2 + 3, 1});
        center_line(gs.playfield[0], pad, term_cols + 1);
        center_line(gs.playfield[1], pad, term_cols + 1);
        center_line(gs.playfield[2], pad, term_cols + 1);

        free(pad);
        // }

    }

    free_game(&gs);
    return 0;
}
