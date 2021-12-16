#include "bmp_convert.h"
#include "bmp_header.h"

/* PADDING */
uint8_t calculate_padding(size_t img_width);

static uint32_t calculate_image_size(size_t img_width, size_t img_height, size_t padding_size);

static uint32_t calculate_file_size(uint32_t img_size);

static uint32_t SIGNATURE = 19778;
static uint32_t RESERVED = 0;
static uint32_t HEADER_SIZE = 40;
static uint16_t PLANES = 1;
static uint32_t COMPRESSION = 0;
static uint32_t PIXEL_PER_M = 2834;
static uint32_t COLORS_USED = 0;
static uint32_t COLORS_IMPORTANT = 0;
static size_t BIT_COUNT = 24;

enum status create_bmp_header(struct image const *img, struct bmp_header *header) {

    const size_t padding = calculate_padding(img->width);
    header->signature = SIGNATURE;
    header->image_size = calculate_image_size(img->width, img->height, padding);
    header->filesize = calculate_file_size(header->image_size);
    header->reserved = RESERVED;
    header->data_offset = sizeof(struct bmp_header);
    header->size = HEADER_SIZE;
    header->width = img->width;
    header->height = img->height;
    header->planes = PLANES;
    header->bit_count = BIT_COUNT;
    header->compression = COMPRESSION;
    header->x_pixels_per_m = PIXEL_PER_M;
    header->y_pixels_per_m = PIXEL_PER_M;
    header->colors_used = COLORS_USED;
    header->colors_important = COLORS_IMPORTANT;

    return OK;
}

static uint32_t calculate_image_size(size_t img_width, size_t img_height, size_t padding_size) {
    return (img_width * sizeof(struct pixel) + padding_size) * img_height;
}

static uint32_t calculate_file_size(uint32_t img_size) {
    return img_size + sizeof(struct bmp_header);
}
