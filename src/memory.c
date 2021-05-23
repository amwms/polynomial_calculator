/** @file
  Implementacja bibioteki zarządzającej pamięcią.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 02/05/2021
*/
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

void error(void *ptr) {
    if (ptr == NULL)
        exit(1);
}

void* safeMalloc(size_t size) {
//    printf("I WANT %zu MALLOC BAJTS\n", size);
    void *ptr = (void *) malloc(size);

    error(ptr);

    return ptr;
}

void* safeRealloc(void *point, size_t size) {
//    printf("I WANT %zu REALLOC BAJTS\n", size);
    void *ptr = (void *) realloc(point, size);

    error(ptr);

    return ptr;
}