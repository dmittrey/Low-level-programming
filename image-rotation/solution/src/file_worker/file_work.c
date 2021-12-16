#include "file_work.h"

struct maybe_file file_read_open(const char *const file_name) {
    FILE *file;
    if ((file = fopen(file_name, "r")) == NULL) {
        return (struct maybe_file) {.status = OPEN_TO_READ_ERROR};
    }
    return (struct maybe_file) {.status = OK, .file = file};
}

struct maybe_file file_write_open(const char *const file_name) {
    FILE *file;
    if ((file = fopen(file_name, "w")) == NULL) {
        return (struct maybe_file) {.status = OPEN_TO_WRITE_ERROR};
    }
    return (struct maybe_file) {.status = OK, .file = file};
}

enum status file_close(FILE *file) {
    if (fclose(file) == EOF) {
        return CLOSE_ERROR;
    }
    return OK;
}
