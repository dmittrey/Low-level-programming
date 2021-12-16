#include "status_handler.h"

typedef char *(*error_text)(enum status);

static const char *const error_descriptions[] = {
        [OK] = "INFO: Program executed successful!",
        [OPEN_TO_WRITE_ERROR] = "ERROR: Unable to open bmp file to write!",
        [OPEN_TO_READ_ERROR] = "ERROR: Unable to open bmp file to read!",
        [CLOSE_ERROR] = "ERROR: Unable to close bmp file!",
        [PARSE_ERROR] = "ERROR: Unable to read from bmp file!",
        [CONVERT_ERROR] = "ERROR: Unable to write to bmp file!"
};

void print_status(enum status status) {
    printf("-->%s\n", error_descriptions[status]);
}
