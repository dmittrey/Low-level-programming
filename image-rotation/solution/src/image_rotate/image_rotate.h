#ifndef ASSIGNMENT_IMAGE_ROTATION_IMAGE_ROTATE_H
#define ASSIGNMENT_IMAGE_ROTATION_IMAGE_ROTATE_H

#include "../image/image.h"
#include "../image_rotate_90/image_rotate_90.h"

typedef void (*image_rotate_func)(struct image *img);

image_rotate_func img_rotate[] = {
        [90] = rotate_90
};

void rotate(size_t degree, struct image *img) {
    img_rotate[degree % 360](img);
}

#endif //ASSIGNMENT_IMAGE_ROTATION_IMAGE_ROTATE_H
