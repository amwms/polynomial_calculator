/** @file
  Implementacja stosu.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/05/2021
*/

#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"
#include "poly.h"
#include "stack.h"
#include "parse.h"

/**
 * Parsuje operację kalkulatora wielomianów i jeśli jest to poprawna opercja, to ją wykonuje.
 * @param[in] stack : stos
 * @param[in] verse : parsowany wiersz
 * @param[in] nr : numer wiersza
 */
void parseAndDoOperation(Stack *stack, char **verse, int nr);

#endif
