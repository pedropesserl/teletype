#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "libtermio.h"
#include "teletype.h"

static struct termios term_config;

void sigint_handler(int signum) {
    (void)signum;
    set_stdin_flush(&term_config);
    clear_screen();
    show_cursor();
    exit(0);
}

int main() {
    disable_canonical_stdin(&term_config);
    prepare_sigint(sigint_handler);
    srand(time(0));

    GameState gs = {
        .timer = {
            .seconds = 30,
            .string = calloc(5 + 1, sizeof(char)),
        },
        .cursor_pos = (Vector2){0},
        .dict = read_dict_from_file("resources/dict_ascii.txt"),
        .playfield = initialize_playfield(gs.dict),
        .started = false,
        .word_count = 0,
        .no_error = true,
    };

    initialize_screen(&gs);

    time_t time_control = time(NULL);
    int timer_seconds = gs.timer.seconds;
    while (gs.timer.seconds > 0) {
        if (gs.started && time(NULL) > time_control) { // one second has passed
            time_control = time(NULL);
            update_timer(&gs);
        }

        if (kb_hit()) {
            char input_char = fgetc(stdin);
            if (input_char == '\t') {
                restart(&gs);
                initialize_screen(&gs);
            }
            char cursor_char = gs.playfield[gs.cursor_pos.x][gs.cursor_pos.y];
            if (input_char == cursor_char) {
                if (cursor_char == ' ') { // finished word
                    if (gs.no_error) {
                        gs.word_count++;
                    }
                    gs.no_error = true; // reset no error flag
                }
                gs.started = true; // start timer after first correct hit
                print_typed_char(&gs, cursor_char);
                gs.cursor_pos = next_cursor_pos(&gs);
            } else {
                gs.no_error = false;
            }
        }
    }
    set_stdin_flush(&term_config);
    printf("\n\n%.2f WPM\n", (float)gs.word_count / timer_seconds * 60.0f);

    free_game(&gs);
    return 0;
}
