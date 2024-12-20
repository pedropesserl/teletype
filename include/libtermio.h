#ifndef LIBTERMIO_H_
#define LIBTERMIO_H_

#include <signal.h>
#include <termios.h>

typedef struct Vector2_s {
    int x;
    int y;
} Vector2;

void hide_cursor();

void show_cursor();

void cursor_up(int rows);

void cursor_down(int rows);

void cursor_right(int cols);

void cursor_left(int cols);

void cursor_to(Vector2 pos); 

void clear_screen();

// returns a Vector2 with x = number of rows and y = number of columns.
Vector2 get_terminal_size();

// takes a pointer to a function that describes what should happen when a
// SIGINT signal is detected; makes the change
void prepare_sigint(void (*sign_handler)(int));

// detects keyboard hits.
int kb_hit();

// sets the stdin configs based on what's stored in config, immediately.
void set_stdin_now(struct termios *config);

// sets the stdin configs based on what's stored in config, after all
// output has been transmitted.
void set_stdin_flush(struct termios *config);

// stores in currconfig a copy of the current stdin configs;
// returns a struct with its new configs.
struct termios disable_canonical_stdin(struct termios *currconfig);

#endif // LIBTERMIO_H_
