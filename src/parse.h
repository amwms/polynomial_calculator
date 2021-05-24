/** @file
  Implementacja parsowania wiersza potencjalnie zawierającego wielomian.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/05/2021
*/

#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdbool.h>
#include <stddef.h>
#include "memory.h"
#include "poly.h"
#include "stack.h"

/**
 * Wypisuje wielomiam.
 * @param[in] poly : wielomian
 * @return
*/
void printPoly(Poly *poly);

/**
 * Sprawdza czy znak jest cyfrą.
 * @param[in] c : znak
 * @return czy znak jest cyfrą
*/
bool isDigit(char c);

/**
 * Parsowanie liczby long long.
 * @param[in] number : liczba
 * @return czy udało się sparsować liczbę
*/
bool isParseNumberLL(char **str, long long *number);

/**
 * Parsowanie liczby unsigned long long.
 * @param[in] number : liczba
 * @return czy udało się sparsować liczbę
*/
bool isParseNumberULL(char **str, unsigned long long *number);

/**
 * Parsowanie wiersza z wejścia który potencjalnie zawiera wielomian (jeśli nie to wypisuje error)
 * @param[in] str : wiersz
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 * @return
*/
void parseVerse(char **str, Stack  *stack, int nr);

#endif