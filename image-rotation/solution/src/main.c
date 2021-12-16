#include <stdio.h>

#include "file_worker/file_work.h"
#include "image/image.h"
#include "image_convert/image_convert.h"
#include "image_rotate/image_rotate.h"
#include "util/status_handler.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "not enough args\n");
        return 0;
    }
    const char *const input_file_name = argv[1];
    const char *const output_file_name = argv[2];
    struct image img = {0};

    // Открыли файл для чтения
    struct maybe_file input_file = file_read_open(input_file_name);

    enum status ex_status;

    // Если он открылся успешно
    if (input_file.status == OK) {

        // Если считали успешно

        if (from_format_convertor[BMP](input_file.file, &img) == OK) {

            // Повернём согласно логику функции
            rotate(90, &img);

            // Откроем файл и сконструируем optional тип
            struct maybe_file output_file = file_write_open(output_file_name);

            // Если получилось открыть на запись
            if (output_file.status == OK) {

                // Пробуем перевести обратно в bmp
                ex_status = to_format_convertor[BMP](output_file.file, &img) == CONVERT_ERROR;
                file_close(output_file.file);
            } else ex_status = OPEN_TO_WRITE_ERROR;

        } else ex_status = PARSE_ERROR;

        file_close(input_file.file);
    } else ex_status = OPEN_TO_READ_ERROR;

    image_destroy(img);
    print_status(ex_status);
}
