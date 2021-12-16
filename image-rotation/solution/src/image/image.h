#ifndef LAB3_IMAGE_H
#define LAB3_IMAGE_H

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>

#pragma pack(push, 1)
struct image {
    size_t width, height;
    struct pixel *data;
};
#pragma pack(pop)

struct pixel {
    uint8_t b, g, r;
};

struct image image_create(size_t a_width, size_t a_height);

void image_destroy(struct image an_image);

void image_set_pixel(struct image an_image, struct pixel a_pixel, size_t a_width_index,
                     size_t a_height_index);

struct pixel image_get_pixel(struct image an_image, size_t a_width_index, size_t a_height_index);

#endif //LAB3_IMAGE_H
