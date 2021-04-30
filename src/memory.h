#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

// function which closes the program with fatal error 1 if there is memory outage
void error(void *ptr);

// function which allocs memory and checks if there was no memory outage
void* safeMalloc(size_t size);

// function which reallocs memory and checks if there was no memory outage
void* safeRealloc(void *point, size_t size);

#endif