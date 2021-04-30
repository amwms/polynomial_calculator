#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "poly.h"
#include "memory.h"
#include "polyMath.h"

#include <stdio.h>


int howManyDiffExp(size_t count, Mono monos[]) {
    int diff = 1;

    for (size_t i = 1; i < count; i++) {
        if (!PolyIsZero(&monos[i].p)) {
            if (monos[i].exp != monos[i - 1].exp)
                diff++;
        }
    }

    return diff;
}

int compareMonosByExp(Mono *a, Mono *b) {
    return a->exp - b->exp;
}


Poly CoeffAddCoeff(const Poly *p, const Poly *q) {
    int value = p->coeff + q->coeff;

    // printf("VALUE OF COEFF: %d \n", value);

    return PolyFromCoeff(value);
}

Poly NonCoeffAddCoeff(const Poly *p, const Poly *q) {
    int size = p->size;

    //przypadek że q == 0
    if (PolyIsZero(q))
        return PolyClone(p);

    //przypadek że nie istnieje czynnik x_0^0(...)
    if (p->arr[size - 1].exp != 0) {
        Poly result = PolyClone(p);
        result.arr = (Mono*) safeRealloc(result.arr, sizeof(Mono) * (size + 1));

        result.size++;
        result.arr[size] = MonoFromPoly(q, 0);

        //DEBUG
        // printf("COEF ADD COEFF DEBUG: \n");
        // for(int i = 0; i < size+1; i++){
        //     printf("%ld ", result.arr[i].p.coeff);
        // }


        return result;
    }

    //przypadek że istnieje czynnik x_0^0(...)
    if (PolyIsCoeff(&p->arr[size - 1].p))
        return CoeffAddCoeff(&p->arr[size - 1].p, q);

    return NonCoeffAddCoeff(&p->arr[size - 1].p, q);
}

//TODO
Poly NonCoeffAddNonCoeff(const Poly *p, const Poly *q) {
    // Mono *monos = safeMalloc(sizeof(Mono) * 2);
    // monos[0] = (Mono){.p = *p, .exp = 0};
    // monos[1] = (Mono){.p = *q, .exp = 0};
    //
    // return PolyAddMonos(2, monos);
    //
    int size = p->size + q->size;
    Mono *monos = safeMalloc(sizeof(Mono) * size);

    for (int i = 0; i < size; i++) {
        if (i < p->size) {
            monos[i] = MonoClone(&p->arr[i]);
        }
        else {
            monos[i] = MonoClone(&q->arr[i - p->size]);
        }
    }

    // printf("NONCOEFNONCOEFFNONCOEFF\n");
    return PolyAddMonos(size, monos);

    //OLD CODE
    // int size = howManyEqualExp(2, [*p; *q]);
    // Poly result = PolyCreate(p->size + q->size);
    //
    // int j = 0;
    // int new_poly_id = 0;
    //
    // for (int i = 0; i < p->size; i++) {
    //     if (MonoGetExp(&p->arr[i]) > MonoGetExp(&q->arr[j])) {
    //
    //     }
    //
    //     if (MonoGetExp(&p->arr[i]) > MonoGetExp(&q->arr[j])) {
    //
    //     }
    //
    //     if (MonoGetExp(&p->arr[i]) == MonoGetExp(&q->arr[j])) {
    //
    //     }
    // }
}