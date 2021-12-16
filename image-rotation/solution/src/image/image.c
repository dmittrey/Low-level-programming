#include "image.h"

static size_t get_pixel_offset(struct image an_image, size_t a_width_index, size_t a_height_index);

struct image image_create(size_t const a_width, size_t const a_height) {
    return (struct image) {.width = a_width, .height = a_height, .data= malloc(
            a_width * a_height * sizeof(struct pixel))};
}

void image_destroy(struct image an_image) {
    free(an_image.data);
}

void image_set_pixel(struct image an_image, struct pixel const a_pixel, size_t const a_width_index,
                     size_t const a_height_index) {
    const size_t pixel_address = get_pixel_offset(an_image, a_width_index, a_height_index);
    an_image.data[pixel_address] = a_pixel;
}

struct pixel image_get_pixel(struct image const an_image, size_t const a_width_index, size_t const a_height_index) {
    const size_t pixel_address = get_pixel_offset(an_image, a_width_index, a_height_index);
    return an_image.data[pixel_address];
}

static size_t
get_pixel_offset(struct image const an_image, size_t const a_width_index, size_t const a_height_index) {
    return (a_height_index * an_image.width + a_width_index);
}
