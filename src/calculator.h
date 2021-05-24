/** @file
  Implementacja stosu.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/05/2021
*/

#ifndef POLYNOMIALS_CALCULATOR_H
#define POLYNOMIALS_CALCULATOR_H

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
 * @return
 */
void parseAndDoOperation(Stack *stack, char **verse, int nr);

#endif //POLYNOMIALS_CALCULATOR_H
