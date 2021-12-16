#ifndef LAB3_FILE_WORK_H
#define LAB3_FILE_WORK_H

#include "../util/errors.h"
#include <stdio.h>

struct maybe_file {
    enum status status;
    FILE *file;
};

struct maybe_file file_read_open(const char *file_name);

struct maybe_file file_write_open(const char *file_name);

enum status file_close(FILE *file);

#endif //LAB3_FILE_WORK_H
