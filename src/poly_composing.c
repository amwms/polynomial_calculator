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
 * Szybkie potęgowanie wielomianu.
 * @param[in] p : wielomian
 * @param[in] x : wykładnik
 * @return wielomian @p p podniesiony do @p x-tej potęgi
 */
//Poly polyQuickPower(Poly p, poly_exp_t x) {
//    if (x == 0)
//        return PolyFromCoeff(1);
//
//    Poly resMul = PolyMul(&p, &p);
//    Poly result = polyQuickPower(resMul, x / 2);
//    PolyDestroy(&resMul);
//
//    if (x & 1) {
//        Poly helper = PolyMul(&result, &p);
//        PolyDestroy(&result);
//        result = helper;
//    }
//
//    return result;
//}
//
//Poly compose(const Poly *p, size_t k, Poly *q) {
//    if (PolyIsCoeff(p))
//        return *p;
//
//    Poly result = PolyZero();
//    Poly sub = k > 0 ? q[0] : PolyZero();
//
//    for (size_t i = 0; i < p->size; i++) {
//         Poly resPow = polyQuickPower(sub, p->arr[i].exp);
//         Poly res2 = compose(&p->arr[i].p, k > 0 ? k - 1 : 0, q + 1);
//         Poly resMull = PolyMul(&res2, &resPow);
//
//        PolyDestroy(&res2);
//        PolyDestroy(&resPow);
//
//        Poly helper = PolyAdd(&result, &resMull);
//
//        PolyDestroy(&result);
//        PolyDestroy(&resMull);
//
//        result = helper;
//    }
//
//    return result;
//}

static poly_exp_t greatestPowerOfTwoLeqX(poly_exp_t x) {
    for (poly_exp_t i = 30; i > 0; i--) {
        if ((1 << i) & x)
            return i;
    }

    return 0;
}

static Poly* polyPowersArray(Poly *p, poly_exp_t maxPower) {
    Poly *powersArray = safeMalloc(sizeof(Poly) * (maxPower + 1));

    powersArray[0] = PolyClone(p);

    for (poly_exp_t i = 1; i < maxPower + 1; i++) {
        powersArray[i] = PolyMul(&powersArray[i - 1], &powersArray[i - 1]);
    }

    return powersArray;
}

// quicker power of poly
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



