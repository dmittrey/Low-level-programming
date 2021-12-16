/**
 * Header для файла в котором определены все функции для alloc памяти
 */

#ifndef _MEM_H_
#define _MEM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <sys/mman.h>

/**
 * Определяем адрес начала кучи
 */
#define HEAP_START ((void*)0x04040000)

/**
 * Выделение памяти на куче
 * @param query Кол-во памяти в байтах
 */
void *_malloc(size_t query);

/**
 * Освобождение памяти на куче
 * @param mem Адрес начала освобождаемого региона
 */
void _free(void *mem);

/**
 * Инициализация кучи
 * @param initial_size Начальный размер кучи
 */
void *heap_init(size_t initial_size);

/**
 * Кол-во байтов выводимых при дебаге блока памяти
 */
#define DEBUG_FIRST_BYTES 4

void debug_struct_info(FILE *f, void const *address);

void debug_heap(FILE *f, void const *ptr);

void debug_line(FILE *f);

void debug_label(FILE *f, const char *msg);

void debug(const char *fmt, ...);

#endif
