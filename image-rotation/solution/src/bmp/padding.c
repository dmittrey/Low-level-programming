#include "bmp_convert.h"

uint8_t calculate_padding(size_t img_width) {

    const size_t pixel_row_size = img_width * sizeof(struct pixel);

    return 4 - (pixel_row_size % 4);
}
