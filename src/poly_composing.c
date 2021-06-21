/** @file
  Implementacja biblioteki do składania wielomianów.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/06/2021
*/
#include <stdlib.h>
#include "poly.h"
#include "poly_composing.h"
#include "memory.h"

/**
 * Znalezienie największej potęgi liczby dwa mniejszej od @p x.
 * @param[in] x : wykładnik
 * @return największa potęga liczby dwa mniejsza od @p x.
 */
static poly_exp_t greatestPowerOfTwoLeqX(poly_exp_t x) {
    for (poly_exp_t i = 30; i > 0; i--) {
        if ((1 << i) & x)
            return i;
    }

    return 0;
}

/**
 * Obliczenie wszytskich potęg wielomianu @p p od 0 do potęgi @p maxPower.
 * @param[in] p : wielomian
 * @param[in] maxPower : wykładnik
 * @return tablica wszytskich potęg wielomianu @p p od 0 do potęgi @p maxPower
 */
static Poly* polyPowersArray(Poly *p, poly_exp_t maxPower) {
    Poly *powersArray = safeMalloc(sizeof(Poly) * (maxPower + 1));

    powersArray[0] = PolyClone(p);

    for (poly_exp_t i = 1; i < maxPower + 1; i++) {
        powersArray[i] = PolyMul(&powersArray[i - 1], &powersArray[i - 1]);
    }

    return powersArray;
}

/**
 * Szybkie potęgowanie wielomianu.
 * @param[in] x : wykładnik
 * @param[in] maxPower : maksymalna potęga
 * @param[in] powersArray : tablica wszytskich potęg wielomianu @p p od 0 do potęgi @p maxPower
 * @return wielomian @p p podniesiony do @p x-tej potęgi
 */
static Poly polyQuickPower(poly_exp_t x, poly_exp_t maxPower, Poly *powersArray) {
    if (x == 0)
        return PolyFromCoeff(1);

    Poly result = PolyFromCoeff(1);

    for (poly_exp_t i = 0; i < maxPower + 1; i++) {
        if (x & (1 << i)) {
            Poly resMul = PolyMul(&result, &powersArray[i]);
            PolyDestroy(&result);
            result = resMul;
        }
    }

    return result;
}

Poly compose(const Poly *p, size_t k, Poly *q) {
    if (PolyIsCoeff(p))
        return *p;

    Poly result = PolyZero();
    Poly qI = k > 0 ? q[0] : PolyZero();

    poly_exp_t x = 0;
    for (size_t i = 0; i < p->size; i++)
        x = x > p->arr[i].exp ? x : p->arr[i].exp;

    poly_exp_t maxPower = greatestPowerOfTwoLeqX(x);
    Poly *powersArray = polyPowersArray(&qI, maxPower);

    for (size_t i = 0; i < p->size; i++) {
        Poly resPow = polyQuickPower(p->arr[i].exp, maxPower, powersArray);
        Poly res2 = compose(&p->arr[i].p, k > 0 ? k - 1 : 0, q + 1);
        Poly resMul = PolyMul(&res2, &resPow);

        PolyDestroy(&res2);
        PolyDestroy(&resPow);

        result = PolyAddProperty(&result, &resMul);
    }

    for (size_t i = 0; i < maxPower + 1u; i++)
        PolyDestroy(&powersArray[i]);

    free(powersArray);

    return result;
}



