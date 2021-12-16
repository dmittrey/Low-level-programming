#ifndef LAB3_BMP_CONVERT_H
#define LAB3_BMP_CONVERT_H

#include "../image/image.h"
#include "../util/errors.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>

enum status from_bmp(FILE *in, struct image *img);

enum status to_bmp(FILE *out, struct image const *img);

#endif //LAB3_BMP_CONVERT_H
