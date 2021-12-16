#include "image_rotate_90.h"

static uint64_t calculate_height_index(uint64_t width_index);

static uint64_t calculate_width_index(uint64_t height_index, uint64_t img_height);

void rotate_90(struct image *const img) {

    // Вычисляем ширину и высоту изображения
    const uint64_t trg_height = img->width;
    const uint64_t trg_width = img->height;

    // Буфер для новой фотки(меняем размеры местами)
    struct image trg_img = image_create(trg_width, trg_height);

    // Поменяем пиксели между собой
    for (uint64_t i = 0; i < img->height; i++) { // height_index
        for (uint64_t j = 0; j < img->width; j++) { // width_index

            // Вычислим координаты соответствующей точки
            const uint64_t mapping_pixel_width_index = calculate_width_index(i, img->height);
            const uint64_t mapping_pixel_height_index = calculate_height_index(j);

            // Вычислим искомый пиксель
            const struct pixel src_pixel = image_get_pixel(img[0], j, i);

            // Заменим пиксель зеркальной точки на искомый
            image_set_pixel(trg_img, src_pixel, mapping_pixel_width_index, mapping_pixel_height_index);
        }
    }

    // Поменять картинку и освободить память занятой старой
    image_destroy(*img);
    *img = trg_img;
}

static uint64_t calculate_height_index(uint64_t width_index) {
    return width_index;
}

static uint64_t calculate_width_index(uint64_t height_index, uint64_t img_height) {
    return img_height - 1 - height_index;
}
