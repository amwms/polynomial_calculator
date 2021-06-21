/** @file
  Implementacja biblioteki do składania wielomianów.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/06/2021
*/
#include "poly.h"
#include "poly_composing.h"

/**
 * Szybkie potęgowanie wielomianu.
 * @param[in] p : wielomian
 * @param[in] x : wykładnik
 * @return wielomian @p p podniesiony do @p x-tej potęgi
 */
Poly polyQuickPower(Poly p, poly_exp_t x) {
    if (x == 0)
        return PolyFromCoeff(1);

    Poly resMul = PolyMul(&p, &p);
    Poly result = polyQuickPower(resMul, x / 2);
    PolyDestroy(&resMul);

    if (x & 1) {
        Poly helper = PolyMul(&result, &p);
        PolyDestroy(&result);
        result = helper;
    }

    return result;
}

Poly compose(const Poly *p, size_t k, Poly *q) {
    if (PolyIsCoeff(p))
        return *p;

    Poly result = PolyZero();
    Poly sub = k > 0 ? q[0] : PolyZero();

    for (size_t i = 0; i < p->size; i++) {
         Poly resPow = polyQuickPower(sub, p->arr[i].exp);
         Poly res2 = compose(&p->arr[i].p, k > 0 ? k - 1 : 0, q + 1);
         Poly resMull = PolyMul(&res2, &resPow);

        PolyDestroy(&res2);
        PolyDestroy(&resPow);

        Poly helper = PolyAdd(&result, &resMull);

        PolyDestroy(&result);
        PolyDestroy(&resMull);

        result = helper;
    }

    return result;
}





