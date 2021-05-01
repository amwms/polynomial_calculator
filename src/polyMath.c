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
        // if (!PolyIsZero(&monos[i].p)) {
            if (monos[i].exp != monos[i - 1].exp)
                diff++;
        // }
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
    Poly result = PolyClone(p);
    Poly res = PolyAdd(&p->arr[size - 1].p, q);

    //MEMORY!!!!!!!!!!!
    PolyDestroy(&result.arr[size - 1].p);

    result.arr[size - 1].p = res;

    if (PolyIsZero(&res)) {
        result.arr = (Mono*) safeRealloc(result.arr, sizeof(Mono) * (size - 1));
        result.size--;
    }

    if (result.size == 0) {
        //MEMORY
        PolyDestroy(&result);

        return PolyZero();
    }

    return result;


    // if (PolyIsCoeff(&p->arr[size - 1].p)) {
    //     int res = p->arr[size - 1].p.coeff + q->coeff;
    //
    //     //jeśli ostanti składnik to 0 - zbijamy
    //     if (res == 0) {
    //         Poly result = PolyCreate(size - 1);
    //
    //         for(int i = 0; i < size - 2; i++) {
    //             result.arr[i] = MonoClone(&p->arr[i]);
    //         }
    //
    //         return result;
    //     }
    //
    //     Poly result = PolyClone(p);
    //     result.arr[size - 1].p.coeff = res;
    //
    //     return result;
    // }

///NOWEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    // printf("AAAAAAAAAAAAAAAAA\n" );
    // Poly res = NonCoeffAddCoeff(&p->arr[size - 1].p, q);
    //
    // //jeśli ostanti składnik to 0 - zbijamy
    // if (PolyIsZero(&res)) {
    //     Poly result = PolyCreate(size - 1);
    //
    //     for(int i = 0; i < size - 2; i++) {
    //         result.arr[i] = MonoClone(&p->arr[i]);
    //     }
    //
    //     return result;
    // }
    //
    // Poly result = PolyClone(p);
    // result.arr[size - 1].p = res;
    //
    // return result;

//STAREEEEEEEEEEEEEEE
    // return NonCoeffAddCoeff(&p->arr[size - 1].p, q);
}

//TODO
Poly NonCoeffAddNonCoeff(const Poly *p, const Poly *q) {
    size_t size = p->size + q->size;
    // Mono *monos = safeMalloc(sizeof(Mono) * size);
    Mono monos[size];

    for (size_t i = 0; i < size; i++) {
        if (i < p->size) {
            monos[i] = MonoClone(&p->arr[i]);
        }
        else {
            monos[i] = MonoClone(&q->arr[i - p->size]);
        }
    }

    // printf("NONCOEFNONCOEFFNONCOEFF\n");

    //MEMPORY
    return PolyAddMonos(size, monos);
    // Poly result = PolyAddMonos(size, monos);
    // free(monos);
    //
    // return result;
}

Poly CoeffMulCoeff(const Poly *p, const Poly *q) {
    int value = p->coeff * q->coeff;
    // printf("VALUE OF COEFF: %d \n", value);
    return PolyFromCoeff(value);
}

Poly NonCoeffMullCoeff(const Poly *p, const Poly *q) {
    size_t size = p->size;
    Poly result = PolyClone(p);

    for (size_t i = 0; i < size; i++) {
        result.arr[i].p = NonCoeffMullCoeff(&result.arr[i].p, q);
    }
}



