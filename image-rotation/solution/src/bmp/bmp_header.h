#ifndef ASSIGNMENT_IMAGE_ROTATION_BMP_HEADER_H
#define ASSIGNMENT_IMAGE_ROTATION_BMP_HEADER_H

#pragma pack(push, 1)
struct bmp_header {
    uint16_t signature;
    uint32_t filesize;
    uint32_t reserved;
    uint32_t data_offset;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_m;
    uint32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t colors_important;
};

#pragma pack(pop)

#endif //ASSIGNMENT_IMAGE_ROTATION_BMP_HEADER_H
