#include "bmp_convert.h"
#include "bmp_header.h"

bool read_header(FILE *const f, struct bmp_header *const header) {
    return fread(header, sizeof(struct bmp_header), 1, f);
}
