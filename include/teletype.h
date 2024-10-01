#define MEM_ERR do { \
        fprintf(stderr, "ERROR: Could not allocate memory: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1); \
    } while (0)
#define FOPEN_ERR(path) do { \
        fprintf(stderr, "ERROR: Could not open file %s\n", (path)); \
        exit(1); \
    } while (0)

#define BG_GREY       "\033[48;5;234m"
#define FG_LIGHT_GREY "\033[38;5;243m"

// frees all strings in a string array pointed to by `arr` (and the array itself)
// and assigns NULL to it.
void destroy_string_array(char ***arr, int size);

// reads a dictionary of words from a file that has one word per line
// and returns it in a string array.
char **read_dict_from_file(const char *dict_path);

// returns a string with approximately 80 characters made from words in `dict`.
char *string_from_dict(char **dict);

// returns an array of three strings with random words from `dict`.
char **initialize_playfield(char **dict);

// prints the initial screen.
void initialize_screen(int term_cols, char **playfield);

// updates the strings in `playfield` in a barrel fashion, with new words from `dict`.
void update_playfield(char **playfield, char **dict);
