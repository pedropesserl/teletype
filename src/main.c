#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libtermio.h"
#include "teletype.h"

#include <string.h>

int main() {
    srand(time(0));

    GameState gs = {
        .timer = {
            .seconds = 10,
            .string = (char*)calloc(5 + 1, sizeof(char)),
        },
        .cursor = 0,
        .dict = read_dict_from_file("resources/dict_ascii.txt"),
        .playfield = initialize_playfield(gs.dict),
    };

    Vector2 term_size = get_terminal_size();
    initialize_screen(&gs, term_size);

    time_t time_control = time(NULL);
    while (gs.timer.seconds > 0) {
        if (time(NULL) > time_control) {
            time_control = time(NULL);
            scroll_playfield(gs.playfield, gs.dict);
            update_timer(&gs.timer);
        }
    }

    free_game(&gs);
    return 0;
}
