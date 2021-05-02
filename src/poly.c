#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "poly.h"

#include <stdio.h>
//MEMORY_C
void error(void *ptr) {
    if (ptr == NULL)
        exit(1);
}

void* safeMalloc(size_t size) {
    void *ptr = (void *) malloc(size);

    error(ptr);

    return ptr;
}

void* safeRealloc(void *point, size_t size) {
    void *ptr = (void *) realloc(point, size);

    error(ptr);

    return ptr;
}

//POLYMATH_C

size_t howManyDiffExp(size_t count, Mono monos[]) {
    size_t diff = 1;

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

poly_exp_t maxOfExp(poly_exp_t a, poly_exp_t b) {
    return a < b ? b : a;
}

Poly CoeffAddCoeff(const Poly *p, const Poly *q) {
    poly_coeff_t value = p->coeff + q->coeff;
    // printf("VALUE OF COEFF: %d \n", value);

    return PolyFromCoeff(value);
}

Poly NonCoeffAddCoeff(const Poly *p, const Poly *q) {
    size_t size = p->size;

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
    Mono *monos = safeMalloc(sizeof(Mono) * size);
    // Mono monos[size];

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
    // return PolyAddMonos(size, monos);
    Poly result = PolyAddMonos(size, monos);
    free(monos);

    return result;
}

Poly CoeffMulCoeff(const Poly *p, const Poly *q) {
    poly_coeff_t value = p->coeff * q->coeff;

    return PolyFromCoeff(value);
}

Poly NonCoeffMulCoeff(const Poly *p, const Poly *q) {
    if (PolyIsZero(q))
        return PolyZero();

    // printf("%zu\n", p->size);

    size_t size = p->size;
    Poly result = PolyCreate(size);

    size_t zeros = 0;
    size_t id = 0;
    for (size_t i = 0; i < size; i++) {
        Poly poly = PolyMul(&p->arr[i].p, q);


        // if (PolyIsCoeff(&p->arr[i].p))
        //     poly = CoeffMulCoeff(&p->arr[i].p, q);
        // else
        //     poly = NonCoeffMulCoeff(&p->arr[i].p, q);

        // printf("AAAAAAAA\n");
        // fflush(stdout);

        if (PolyIsZero(&poly)) {
            PolyDestroy(&poly);
            zeros++;

            continue;
        }

        result.arr[id].p = poly;
        result.arr[id].exp = p->arr[i].exp;

        id++;
    }

    if (size == zeros) {
        free(result.arr);
        return PolyZero();
    }

    result.arr = (Mono*) safeRealloc(result.arr, sizeof(Mono) * (size - zeros));
    result.size = size - zeros;

    if (result.size == 1) {
        if (PolyIsCoeff(&result.arr[0].p) && result.arr[0].exp == 0) {
            Poly resultCoeff = result.arr[0].p;
            PolyDestroy(&result);

            return resultCoeff;
        }
    }

    return result;
}

Poly NonCoeffMulNonCoeff(const Poly *p, const Poly *q) {
    size_t size = p->size * q->size;
    Mono *multi = safeMalloc(sizeof(Mono) * size);
    size_t last = 0;

    for (size_t i = 0; i < p->size; i++) {
        for (size_t j = 0; j < q->size; j++) {
            Poly value = PolyMul(&p->arr[i].p, &q->arr[j].p);
            poly_exp_t exp = p->arr[i].exp + q->arr[j].exp;

            if (PolyIsZero(&value))
                exp = 0;

            multi[last++] = MonoFromPoly(&value, exp);
        }
    }

    Poly result = PolyAddMonos(size, multi);
    free(multi);
    return result;
}

poly_coeff_t power(poly_coeff_t a, poly_exp_t x) {
    if (x == 0)
        return (poly_coeff_t) 1;

    if (x % 2 == 0)
        return power(a * a, x / 2);
    return power(a * a, x / 2) * a;
}

/**
 * Komparator do qsorta - sortowanie po wykładnikach malejaco.
 * @param[in] a : jednomian
 * @param[in] b : jednomian
 * @return różnica wykładników jednomianów
 */
int compareMonosByExpQsort(const void *a, const void *b) {
    return -compareMonosByExp((Mono*) a, (Mono*) b);
}

/**
 * Sortuje tablicę jednomianów po wykładnikach.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return
 */
void sortMonosByExp(size_t count, Mono *monos) {
    qsort(monos, count, sizeof(Mono), compareMonosByExpQsort);
}

/**
 * Robi płytką kopię tablicy jednomianów.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return skopiowana tablica jednomianów
 */
Mono* copyMonoArray(size_t count, const Mono monos[]) {
    Mono *result = safeMalloc(sizeof(Mono) * count);

    for (size_t i = 0; i < count; i++) {
        result[i] = monos[i];
    }

    return result;
}

Poly PolyCreate(size_t size) {
    if (size == 0)
        return (Poly) {.coeff = 0, .arr = NULL};

    return (Poly) {.size = size, .arr = safeMalloc(sizeof(Mono) * size)};
}

void PolyDestroy(Poly *p) {
    if (p->arr == NULL){
        return;
    }

    for (size_t i = 0; i < p->size; i++) {
        MonoDestroy(&p->arr[i]);
    }

    free(p->arr);
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p))
        return (Poly) {.coeff = p->coeff, .arr = NULL};

    // printf("WSKAŹNIK: %p\n", p);
    Poly p2 = *p;

    // p2.arr = safeMalloc(sizeof(&(p->arr)));
    p2.arr = safeMalloc(sizeof(Mono) * p->size);

    // printf("P_SIZE_BEFORE: %zu \n", p->size);
    for (size_t i = 0; i < p->size; i++) {
        p2.arr[i] = MonoClone(&p->arr[i]);
    }
    // printf("P_SIZE_AFTER: %zu \n", p->size);
    return p2;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsZero(p))
        return PolyClone(q);
    if (PolyIsZero(q))
        return PolyClone(p);

    if (PolyIsCoeff(p)) {
        if (PolyIsCoeff(q)) {
            return CoeffAddCoeff(p, q);
        }
        // printf("SHOULD BE HERE\n");
        return NonCoeffAddCoeff(q, p);
    }

    if (PolyIsCoeff(q))
        return NonCoeffAddCoeff(p, q);

    // printf("AAAAAAAAAAAAAAAAAAAAA");
    return NonCoeffAddNonCoeff(p, q);
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    Mono *myMonos = copyMonoArray(count, monos);
    sortMonosByExp(count, myMonos);

    //jeśli wieloian jest współczynnikiem "zbijamy go" zwracamy współczynnik
    if (count == 1) {
        if (PolyIsCoeff(&monos[0].p) && monos[0].exp == 0) {
            //MEMORY!!!!!!!!!!!!!!!
            poly_coeff_t coeffFinal = myMonos[0].p.coeff;
            free(myMonos);

            return PolyFromCoeff(coeffFinal);
        }
        // return (Poly) {.size = 1, .arr = myMonos};
    }

    if (count < 1) {
        //MEMORY
        free(myMonos);

        return PolyZero();
    }

    size_t diffExps = howManyDiffExp(count, myMonos);
    Poly result = PolyCreate(diffExps);
    // Poly helper = PolyClone(&myMonos[0].p);
    Poly helper = myMonos[0].p;
    poly_exp_t helper_exp = myMonos[0].exp;
    size_t result_id = 0;


    // idziemy po kolei i sprawdzamy czy sąsiednie wykładniki czy są równe jeśli nie
    // to wstawiamy ja tak to robimy poly PolyAdd
    for (size_t i = 1; i < count; i++) {

        //NWM CO TU POWINNO BYĆ BO CHYBA POTEM TO SPRAWDZAM DOPIERO JAK PRZEPISUJE
        // if (PolyIsZero(&myMonos[i].p))
        //     continue;

        //DEBUUUUUUUUUUUUUUUUUUUUUUGGGGGGGGGGGGGGGGG
        // printf("POLY ADD MONOS EXP COMPARE: %d %d\n", myMonos[i-1].exp, myMonos[i].exp);
        if (compareMonosByExp(&myMonos[i - 1], &myMonos[i]) == 0) {
            //PAMIEC!!!!!!!!!!!!!!!
            // helper = PolyAdd(&helper, &myMonos[i].p);

            Poly pom = PolyAdd(&helper, &myMonos[i].p);
            PolyDestroy(&helper);
            PolyDestroy(&myMonos[i].p);
            helper = pom;

            // printf("WORKED THIS IS HELPER:\n");

            continue;
        }

        result.arr[result_id] = (Mono) {.p = PolyClone(&helper), .exp = helper_exp};

        //PAMIEC!!!!!!!!!!!!!!!
        //zmieniamy wartość helper więc trzeba zrobic free
        PolyDestroy(&helper);

        // helper = PolyClone(&myMonos[i].p);
        helper = myMonos[i].p;
        helper_exp = myMonos[i].exp;
        result_id++;
    }


    // printf("THIS IS RESULT_ID: %d\n", result_id);
    // printf("THIS IS DIFFEXP: %d\n", diffExps);
    if (result_id < diffExps /*- 1*/) {
        result.arr[diffExps - 1] = (Mono) {.p = PolyClone(&helper), .exp = helper_exp};
    }

    //PAMIEC!!!!!!!!!!!!!!!
    PolyDestroy(&helper);

    //NA RAZIE W INNYM MIEJSCU
    //jeśli wieloian jest współczynnikiem "zbijamy go" zwracamy współczynnik
    // if (PolyIsCoeff(&result.arr[0].p) && diffExps == 1 && result.arr[0].exp == 0)
    //     return PolyFromCoeff(result.arr[0].p.coeff);


        //NOWEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    //przepisujemy tak żeby nie było wśród współczynników zer i żeby wielomian się zgadzał rozmiarem
    size_t zeros = 0;
    for (size_t i = 0; i < diffExps; i++) {
        if (PolyIsZero(&result.arr[i].p))
            zeros++;
    }

    // printf("ZEROS:%d\n", zeros);
    // printf("DIFF:%d\n", diffExps);
    Poly resultFinal = PolyCreate(diffExps - zeros);

    //tworzymy resultFinal za pomoca PŁYTKIEJ kopii result
    if (!PolyIsCoeff(&resultFinal)) {
        size_t id = 0;
        for(size_t i = 0; i < diffExps; i++) {
            if (!PolyIsZero(&result.arr[i].p)) {
                resultFinal.arr[id] = result.arr[i];
                id++;
            }
        }

        //jeśli wieloian jest współczynnikiem "zbijamy go" zwracamy współczynnik
        if (resultFinal.size == 1 && PolyIsCoeff(&resultFinal.arr[0].p) && resultFinal.arr[0].exp == 0) {
            //MEMORY!!!!!!!!!!!
            poly_coeff_t coeffFinal = resultFinal.arr[0].p.coeff;
            PolyDestroy(&resultFinal);
            free(result.arr);
            free(myMonos);

            // return PolyFromCoeff(resultFinal.arr[0].p.coeff);
            return PolyFromCoeff(coeffFinal);
        }
    }

    // PolyDestroy(&result);
    //myMonos i result skopiowaliśmy do wyniku za pomoca PŁYTKIEJ kopii
    free(myMonos);
    free(result.arr);

    return resultFinal;
}


Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsZero(p))
        return PolyZero();
    if (PolyIsZero(q))
        return PolyZero();

    if (PolyIsCoeff(p)) {
        if (PolyIsCoeff(q))
            return CoeffMulCoeff(p, q);
        return NonCoeffMulCoeff(q, p);
    }

    if (PolyIsCoeff(q))
        return NonCoeffMulCoeff(p, q);

    return NonCoeffMulNonCoeff(p, q);
}

//GOOD TODO
Poly PolyNeg(const Poly *p) {
    Poly p_neg = {.coeff = (-1), .arr = NULL};
    Poly result = PolyMul(p, &p_neg);

    PolyDestroy(&p_neg);

    return result;
}

//GOOD TODO
Poly PolySub(const Poly *p, const Poly *q) {
    Poly q_neg = PolyNeg(q);

    Poly result = PolyAdd(p, &q_neg);
    PolyDestroy(&q_neg);

    return result;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    if (var_idx == 0)
        return p->arr[0].exp;

    poly_exp_t maxExp = 0;
    for (size_t i = 0; i < p->size; i++) {
        maxExp = maxOfExp(PolyDegBy(&p->arr[i].p, var_idx - 1), maxExp);
    }

    return maxExp;
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    poly_exp_t maxExp = 0;
    for (size_t i = 0; i < p->size; i++) {
        maxExp = maxOfExp(PolyDeg(&p->arr[i].p) + p->arr[i].exp, maxExp);
    }

    return maxExp;
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p))
        return (PolyIsCoeff(q) && p->coeff == q->coeff);

    if (PolyIsCoeff(q))
        return false;

    if (!(p->size == q->size))
        return false;

    for (size_t i = 0; i < p->size; i++) {
        // printf("EXP1:%d  EXP2:%d \n", p->arr[i].exp, q->arr[i].exp);

        if (p->arr[i].exp == q->arr[i].exp) {
            if (!PolyIsEq(&p->arr[i].p, &q->arr[i].p))
                return false;
        }
        else {
            return false;
        }
    }

    return true;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff);

    size_t size = p->size;
    Mono *monosResult = safeMalloc(sizeof(Mono) * size);

    for (size_t i = 0; i < size; i++) {
        Poly q = PolyFromCoeff(power(x, p->arr[i].exp));

        Poly value = PolyMul(&p->arr[i].p, &q);

        monosResult[i] = MonoFromPoly(&value, 0);

        PolyDestroy(&q);
    }

    Poly result = PolyAddMonos(size, monosResult);

    free(monosResult);

    if (PolyIsCoeff(&result)) {
        return result;
    }

    Poly resultFinal = result.arr[0].p;
    free(result.arr);

    return resultFinal;
}

