/** @file
  Implementacja stosu.

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

void printPoly(Poly *poly);

void printMono(Mono *mono);

void printPoly(Poly *poly);

bool isParseNumberLL(char **str, long long *number);

bool isParseNumberULL(char **str, unsigned long long *number);

bool isParseCoeff(char **str, Poly *p);

bool isParseMono(char **str, Mono *mono);

bool isParsePoly(char **str, Poly *poly);

bool isParseVerse(char **str, Poly *poly);

#endif