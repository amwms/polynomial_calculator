/** @file
  Implementacja biblioteki do składania wielomianów.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/06/2021
*/
#include "poly.h"
#include "poly_composing.h"

// TODO
Poly polyQuickPower(Poly p, poly_exp_t x) {
    if (x == 0)
        return PolyFromCoeff(1);

    Poly result = polyQuickPower(PolyMul(&p, &p), x / 2);

    if (x & 1)
        result = PolyMul(&result, &p);

    return result;
}

Poly compose(Poly *p, size_t k, Poly *q) { // q to tablica wielomianów
    if (PolyIsCoeff(p))
        return *p;

    Poly result = PolyZero();
    Poly sub = k > 0 ? q[0] : PolyZero();

    for (size_t i = 0; i < p->size; i++) {
         Poly resPow = polyQuickPower(sub, p->arr[i].exp);
         Poly res2 = compose(&p->arr[i].p, k > 0 ? k - 1 : 0, q + 1);
         Poly resMull = PolyMul(&res2, &resPow);
         result = PolyAdd(&result, &resMull);
    }

    return result;
}





