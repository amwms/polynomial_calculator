#ifndef POLYMATH_H
#define POLYMATH_H

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "poly.h"
#include "memory.h"

/**
 * Sprawdza ile jest równych wykładników wśród wszystkich jednomianów w tablicy.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return liczba równych wykładników
 */
int howManyDiffExp(size_t count, Mono monos[]);

/**
 * Porównuje dwa jednomiany po wykładniku.
 * @param[in] a : jednomian
 * @param[in] b : jednomian
 * @return różnica wykładników jednomianów
 */
int compareMonosByExp(Mono *a, Mono *b);

/**
 * Dodaje dwa wielomiany które są współczynnikikami.
 * @param[in] p : wielomian (współczynnik) @f$p@f$
 * @param[in] q : wielomian (współczynnik) @f$q@f$
 * @return @f$p + q@f$
 */
Poly CoeffAddCoeff(const Poly *p, const Poly *q);

/**
 * Dodaje dwa wielomiany - wielomian (nie będący współczynnikiem) i wpółczynnik.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian (współczynnik) @f$q@f$
 * @return @f$p + q@f$
 */
Poly NonCoeffAddCoeff(const Poly *p, const Poly *q);

/**
 * Dodaje dwa wielomiany, które nie są współczynnikami.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p + q@f$
 */
Poly NonCoeffAddNonCoeff(const Poly *p, const Poly *q);
#endif