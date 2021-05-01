#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "polyMath.h"
#include "poly.h"

#include <stdio.h>
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

    Poly p2 = *p;

    // p2.arr = safeMalloc(sizeof(&(p->arr)));
    p2.arr = safeMalloc(sizeof(Mono) * p->size);

    for (size_t i = 0; i < p->size; i++) {
        p2.arr[i] = MonoClone(&p->arr[i]);
    }

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

    int diffExps = howManyDiffExp(count, myMonos);
    Poly result = PolyCreate(diffExps);
    // Poly helper = PolyClone(&myMonos[0].p);
    Poly helper = myMonos[0].p;
    poly_exp_t helper_exp = myMonos[0].exp;
    int result_id = 0;


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
    int zeros = 0;
    for (int i = 0; i < diffExps; i++) {
        if (PolyIsZero(&result.arr[i].p))
            zeros++;
    }

    // printf("ZEROS:%d\n", zeros);
    // printf("DIFF:%d\n", diffExps);
    Poly resultFinal = PolyCreate(diffExps - zeros);

    //tworzymy resultFinal za pomoca PŁYTKIEJ kopii result
    if (!PolyIsCoeff(&resultFinal)) {
        int id = 0;
        for(int i = 0; i < diffExps; i++) {
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
    return PolyAdd(p, &q_neg);
}
//
// poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
//
// }
//
// poly_exp_t PolyDeg(const Poly *p) {
//
// }
//
bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p))
        return (PolyIsCoeff(q) && p->coeff == q->coeff);

    if (!(p->size == q->size))
        return false;

    for (size_t i = 0; i < p->size; i++) {
        // printf("EXP1:%d  EXP2:%d \n", p->arr[i].exp, q->arr[i].exp);

        if (p->arr[i].exp == q->arr[i].exp) {
            PolyIsEq(&p->arr[i].p, &q->arr[i].p);
        }
        else {
            return false;
        }
    }

    return true;
}
//
// Poly PolyAt(const Poly *p, poly_coeff_t x) {
//
// }
//
