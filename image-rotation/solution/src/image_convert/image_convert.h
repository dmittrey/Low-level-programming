#ifndef ASSIGNMENT_IMAGE_ROTATION_IMAGE_CONVERT_H
#define ASSIGNMENT_IMAGE_ROTATION_IMAGE_CONVERT_H

#include "../bmp/bmp_convert.h"
#include "../image/image.h"
#include "../util/errors.h"

typedef enum status (*to_format_convert_func)(FILE *const out, struct image const *const img);

typedef enum status (*from_format_convert_func)(FILE *const in, struct image *const img);

enum formats {
    BMP = 0
};

to_format_convert_func to_format_convertor[] = {
        [BMP] = to_bmp
};

from_format_convert_func from_format_convertor[] = {
        [BMP] = from_bmp
};

#endif //ASSIGNMENT_IMAGE_ROTATION_IMAGE_CONVERT_H
