#ifndef LAB3_ERRORS_H
#define LAB3_ERRORS_H

enum status {
    OK = 0,
    OPEN_TO_WRITE_ERROR,
    OPEN_TO_READ_ERROR,
    CLOSE_ERROR,
    PARSE_ERROR,
    CONVERT_ERROR
};

#endif //LAB3_ERRORS_H
