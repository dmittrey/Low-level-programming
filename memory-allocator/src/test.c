#define _DEFAULT_SOURCE

#include "test.h"

#include <unistd.h>

#include "mem_internals.h"
#include "mem.h"
#include "util.h"

const size_t HEAP_INITIAL_SIZE = 12000;

static void extra_end(char *err_msg, void *heap) {
    debug_heap(stderr, heap);
    debug_line(stderr);
    err(err_msg);
}

static void simple_end(char *info_msg) {
    debug_label(stderr, info_msg);
    fprintf(stderr, "\n");
}

static struct block_header *block_get_header(const void *const contents) {
    return (struct block_header *) (((uint8_t *) contents) - offsetof(struct block_header, contents));
}

static void *block_after(struct block_header const *const block) {
    return (void *) (block->contents + block->capacity.bytes);
}

static struct block_header *init_test_heap(size_t initial_size) {
    debug_label(stderr, "Initializing heap");

    struct block_header *heap = heap_init(initial_size);

    if (heap == NULL) extra_end("Heap isn't initialized!", heap);

    debug_heap(stderr, heap);

    simple_end("Heap initialized:");

    return heap;
}

/**
 * Обычное успешное выделение памяти.
 */
void test1(struct block_header *heap_start) {
    debug_label(stderr, "Test 1");

    void *data1 = _malloc(2000);
    struct block_header *data1_header = block_get_header(data1);
    if (data1 == NULL) extra_end("_malloc returned NULL!", heap_start);
    if (data1_header->is_free == true) extra_end("_malloc returned free block!", heap_start);
    if (data1_header->next == NULL) extra_end("_malloc returned not linked block!", heap_start);
    if (data1_header->capacity.bytes != 2000) extra_end("_malloc returned block with wrong capacity", heap_start);

    debug_heap(stderr, heap_start);

    _free(data1);

    if (data1_header->is_free == false) extra_end("_free didn't free block!", heap_start);

    _free(data1);
    simple_end("Passed");
}

/**
 * Освобождение одного блока из нескольких выделенных.
 */
void test2(struct block_header *heap_start) {
    debug_label(stderr, "Test 2");

    void *data1 = _malloc(2000);
    void *data2 = _malloc(2000);
    if (data1 == NULL || data2 == NULL) extra_end("_malloc returned NULL!", heap_start);

    debug_heap(stderr, heap_start);

    struct block_header *data1_header = block_get_header(data1);
    struct block_header *data2_header = block_get_header(data2);

    _free(data2);

    if (data1_header->is_free == true) extra_end("_free free extra block!", heap_start);
    if (data2_header->is_free == false) extra_end("_free didn't free block!", heap_start);

    debug_heap(stderr, heap_start);

    _free(data2);
    _free(data1);
    simple_end("Passed");
}

/**
 * Освобождение двух блоков из нескольких выделенных.
 */
void test3(struct block_header *heap_start) {
    debug_label(stderr, "Test 3");

    void *data1 = _malloc(2000);
    void *data2 = _malloc(3000);
    void *data3 = _malloc(4000);
    if (data1 == NULL || data2 == NULL || data3 == NULL) extra_end("_malloc returned NULL!", heap_start);

    debug_heap(stderr, heap_start);

    struct block_header *data1_header = block_get_header(data1);
    struct block_header *data2_header = block_get_header(data2);
    struct block_header *data3_header = block_get_header(data3);

    if (data1_header->capacity.bytes != 2000
        || data2_header->capacity.bytes != 3000
        || data3_header->capacity.bytes != 4000)
        extra_end("_malloc returned block with wrong capacity!", heap_start);

    _free(data2);

    if (data2_header->is_free == false) extra_end("_free didn't free block!", heap_start);

    debug_heap(stderr, heap_start);

    _free(data1);

    if (data1_header->is_free == false) extra_end("_free didn't free block!", heap_start);
    if (data1_header->next != data3_header) extra_end("_free isn't merge free blocks!", heap_start);

    debug_heap(stderr, heap_start);

    _free(data3);
    _free(data2);
    _free(data1);
    simple_end("Passed");
}

/**
 * Память закончилась, новый регион памяти расширяет старый.
 */
void test4(struct block_header *heap_start) {
    debug_label(stderr, "Test 4");

    void *data1 = _malloc(10000);
    void *data2 = _malloc(5000);
    void *data3 = _malloc(5000);
    if (data1 == NULL || data2 == NULL || data3 == NULL) extra_end("_malloc returned NULL!", heap_start);

    debug_heap(stderr, heap_start);

    struct block_header *data1_header = block_get_header(data1);
    struct block_header *data2_header = block_get_header(data2);
    struct block_header *data3_header = block_get_header(data3);

    if (data1_header->next != data2_header || data2_header->next != data3_header)
        extra_end("_malloc returned not linked blocks", heap_start);

    if (data1_header->capacity.bytes != 10000
        || data2_header->capacity.bytes != 5000
        || data3_header->capacity.bytes != 5000)
        extra_end("_malloc returned block with wrong capacity!", heap_start);

    if (block_after(data1_header) != data2_header || block_after(data2_header) != data3_header)
        extra_end("_malloc returned non-sequentially placed blocks", heap_start);

    _free(data3);
    _free(data2);
    _free(data1);
    simple_end("Passed");
}

void test5(struct block_header *heap_start) {
    debug_label(stderr, "Test 5");

    void *region_between_start_adr = block_after(heap_start);

    debug("bet_reg :%10p", region_between_start_adr);

    void *adr = mmap(region_between_start_adr,
         50000,
         PROT_READ | PROT_WRITE,
         MAP_PRIVATE | MAP_ANONYMOUS,
         0,
         0);

    debug("bet_reg adr:%10p", adr);

    void *data1 = _malloc(10000);
    void *data2 = _malloc(20000);

    if (data1 == NULL || data2 == NULL) extra_end("_malloc returned NULL!", heap_start);

    debug_heap(stderr, heap_start);

    struct block_header *data1_header = block_get_header(data1);
    struct block_header *data2_header = block_get_header(data2);

    debug("first_next :%10p", data1_header->next);
    debug("second :%10p", data2_header);

    if (data1_header->next == data2_header)
        extra_end("_malloc missed block between additional region and first block", heap_start);

    if (data1_header->capacity.bytes != 10000
        || data2_header->capacity.bytes != 20000)
        extra_end("_malloc returned block with wrong capacity!", heap_start);

    if (block_after(data1_header) == data2_header)
        extra_end("_malloc ignore between region when grow", heap_start);

    _free(data2);
    _free(data1);
    simple_end("Passed");
}

void test_all() {
    struct block_header *heap = init_test_heap(HEAP_INITIAL_SIZE);
    test1(heap);
    test2(heap);
    test3(heap);
    test4(heap);
    test5(heap);
    debug_label(stderr, "ALL TESTS PASSED!");
}
