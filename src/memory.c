#include <stdlib.h>
#include "memory.h"

void error(void *ptr) {
    if (ptr == NULL)
        exit(1);
}

void* safeMalloc(size_t size) {
    void *ptr = (void *) malloc(size);

    error(ptr);

    return ptr;
}

void* safeRealloc(void *point, size_t size) {
    void *ptr = (void *) realloc(point, size);

    error(ptr);

    return ptr;
}