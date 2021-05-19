/** @file
  Bibioteka zarządzająca bezpiecznie pamięcią.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 02/05/2021
*/
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>

/**
 * Sprawdza czy skończya się pamięć.
 * @param[in] ptr : pionter na miejsce w pamięci
 */
void error(void *ptr);
/**
 * Mallocuje blok pamięci sprawdzając czy nie skończyła się pamięć.
 * @param[in] size : rozmiar pamięci, który chcemy zaallocować
 * @return miejsce zaalocowanej pamięci
 */
void* safeMalloc(size_t size);

/**
 * Reallocuje blok pamięci sprawdzając czy nie skończyła się pamięć.
 * @param[in] size : rozmiar pamięci, który chcemy zaallocować
 * @param[in] point : miejsce w pamięci gdzie chcemy allocować
 * @return miejsce zaalocowanej pamięci
 */
void* safeRealloc(void *point, size_t size);

#endif