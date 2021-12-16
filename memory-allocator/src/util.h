#ifndef _UTIL_H_
#define _UTIL_H_

#include <stddef.h>
#include "mem_internals.h"

/**
 * Команда поиска наибольшего размера
 */
inline block_size size_max(size_t x, size_t y) {
    return (x >= y) ? (block_size) {x} : (block_size) {y};
}

/**
 * Ничего не возвращающая функция вывода ошибки
 */
_Noreturn void err(const char *msg, ...);

#endif
