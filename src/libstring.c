#include <stdlib.h>
#include <string.h>

typedef struct String_s {
    char *s;
    size_t size;
    size_t capacity;
} String;

String new_string() {
    return (String){0};
}

String new_string_from_cstr(char *cstr, size_t sz) {
    return (String){
        .s = cstr,
        .size = sz,
        .capacity = sz,
    };   
}


