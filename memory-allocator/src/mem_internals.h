#ifndef _MEM_INTERNALS_
#define _MEM_INTERNALS_

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 * Минимальный размер памяти, выделяемой под кучу
 */
#define REGION_MIN_SIZE (2 * 4096)

/**
 * Структура регион(большая область памяти, выделенная под кучу), туда входит:
 * <p>
 * 1) Адрес начала
 * <p>
 * 2) Размер региона
 * <p>
 * 3) Флаг расширимости?
 */
struct region {
    void *addr;
    size_t size;
    bool extends;
};

/**
 * Показатель неправильного региона
 */
static const struct region REGION_INVALID = {0};

/**
 * Метод определения корректности региона
 * @param r Указатель на регион
 * @return Флаг корректности
 */
inline bool region_is_invalid(const struct region *r) { return r->addr == NULL; }

/**
 * Новый тип (вместимость блока), для упрощения понимания
 */
typedef struct { size_t bytes; } block_capacity;

/**
 * Новый тип (размер блока = вместимость блока + заголовок), для упрощения понимания
 */
typedef struct { size_t bytes; } block_size;

/**
 * Заголовок блока памяти, в который входят:
 * <p>
 * 1) Ссылка на следующий заголовок
 * <p>
 * 2) Вместимость блока памяти
 * <p>
 * 3) Флаг(свободен или занят)
 * <p>
 * 4) Массив неопределенного размера с контентом блока
 */
struct block_header {
    struct block_header *next;
    block_capacity capacity;
    bool is_free;
    uint8_t contents[];
};

/**
 * Метод, вычисляющий размер блока из его вместимости
 */
inline block_size size_from_capacity(block_capacity cap) {
    return (block_size) {cap.bytes + offsetof(struct block_header, contents)};
}

/**
 * Метод, вычисляющий вместимость блока из его размера
 */
inline block_capacity capacity_from_size(block_size sz) {
    return (block_capacity) {sz.bytes - offsetof(struct block_header, contents)};
}

#endif
