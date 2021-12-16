#include "bmp_convert.h"
#include "bmp_header.h"

/* PADDING */
uint8_t calculate_padding(size_t img_width);

/* BMP_READER */
bool read_header(FILE *f, struct bmp_header *header);

/* BMP_WRITER */
enum status create_bmp_header(struct image const *img, struct bmp_header *header);

struct maybe_header {
    enum status status;
    struct bmp_header bmp_header;
};

enum status from_bmp(FILE *const in, struct image *const img) {

    // Считали хэдер файла и положили в maybe_bmp_header
    struct maybe_header maybe_bmp_header;
    maybe_bmp_header.status = (read_header(in, &maybe_bmp_header.bmp_header)) ? OK : PARSE_ERROR;

    if (maybe_bmp_header.status == PARSE_ERROR) return PARSE_ERROR;

    // Распарсил ширину и длину
    const size_t src_width = maybe_bmp_header.bmp_header.width;
    const size_t src_height = maybe_bmp_header.bmp_header.height;

    //Создали картинку и положили в контейнер
    img[0] = image_create(src_width, src_height);

    // Вычислить пэддинг
    const uint8_t padding_size = calculate_padding(src_width);

    // Считывать в data и пропускать пэддинг для каждой строки
    for (uint64_t i = 0; i < src_height; i++) {
        //Вычислим индекс с которого записываем
        void *start_index = img->data + img->width * i;

        // Считали пикселей ровно по ширине и скипнули пэддинг
        fread(start_index, sizeof(struct pixel), src_width, in);
        fseek(in, padding_size, SEEK_CUR);
    }

    return OK;
}

enum status to_bmp(FILE *const out, struct image const *const img) {

    // Инициализировали хэдер
    struct bmp_header header = {0};
    create_bmp_header(img, &header);

    // Записали хэдер
    fwrite(&header, sizeof(struct bmp_header), 1, out); //Проверить записали ли
    fseek(out, header.data_offset, SEEK_SET);

    // Посчитали пэддинг
    const size_t padding = calculate_padding(img->width);

    // Аллоцировали строчку пэддинга и заполнили нулями
    uint8_t *line_padding = malloc(padding);

    if (!line_padding) return CONVERT_ERROR;

    for (size_t i = 0; i < padding; ++i) {
        *(line_padding + i) = 0;
    }

    // На каждой новой строке подсовываем в картинку полезную инфу и пэддинги
    if (img->data != NULL) {
        for (size_t i = 0; i < img->height; ++i) {
            fwrite(img->data + i * img->width, img->width * sizeof(struct pixel), 1, out);
            fwrite(line_padding, padding, 1, out);
        }
    }

    // Освободили место в куче
    free(line_padding);

    return OK;
}
