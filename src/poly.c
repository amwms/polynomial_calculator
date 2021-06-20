/** @file
  Implementacja biblioteki podstawowych operacji na wielomianach wielu zmiennych.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 02/05/2021
*/
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "memory.h"
#include "parse.h"
#include "poly.h"
#include "poly_composing.h"

/**
* Sprawdza czy jednomiany w wielomianie są posortowane malejąco po wykładnikach
* @param[in] p : wielomian
* @return czy jednomiany w wielomianie są posortowane malejąco po wykładnikach
*/
static inline bool isSorted(const Poly *p) {
    if(PolyIsCoeff(p))
        return true;

    for (size_t i = 1; i < p->size; i++) {
        if (p->arr[i - 1].exp < p->arr[i].exp)
        return false;
    }

    return true;
}

/**
 * Sprawdza ile jest równych wykładników wśród wszystkich jednomianów w tablicy.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return liczba równych wykładników
 */
static size_t howManyDiffExp(size_t count, Mono monos[]) {
    size_t diff = 1;

    for (size_t i = 1; i < count; i++) {
        if (!PolyIsZero(&monos[i].p)) {
            if (monos[i].exp != monos[i - 1].exp)
                diff++;
        }
    }

    return diff;
}

/**
 * Porównuje dwa jednomiany po wykładniku.
 * @param[in] a : jednomian
 * @param[in] b : jednomian
 * @return różnica wykładników jednomianów
 */
static int compareMonosByExp(Mono *a, Mono *b) {
    return a->exp - b->exp;
}

/**
 * Porównuje dwa wykładniki.
 * @param[in] a : wykładnik
 * @param[in] b : wykładnik
 * @return większy wykładnik
 */
static poly_exp_t maxOfExp(poly_exp_t a, poly_exp_t b) {
    return a < b ? b : a;
}

static Poly PolyAddProperty(Poly *a, Poly *b);

/**
 * Dodaje dwa wielomiany które są współczynnikami.
 * @param[in] a : wielomian (współczynnik)
 * @param[in] b : wielomian (współczynnik)
 * @return @f$a + b@f$
 */
static Poly CoeffAddCoeff(Poly *a, Poly *b) {
    assert(PolyIsCoeff(a) && PolyIsCoeff(b));

    return PolyFromCoeff(a->coeff + b->coeff);
}

/**
 * Dodaje dwa wielomiany - wielomian (nie będący współczynnikiem) i wpółczynnik.
 * @param[in] a : wielomian
 * @param[in] b : wielomian (współczynnik)
 * @return @f$a + b@f$
 */
static Poly NonCoeffAddCoeff(Poly *a, Poly *b) {
//    assert(!PolyIsCoeff(p) && PolyIsCoeff(q));
//    assert(isSorted(p));
//
//    size_t size = p->size;
//
//    //przypadek że q == 0
//    if (PolyIsZero(q))
//        return PolyClone(p);
//
//    //przypadek że nie istnieje czynnik x_0^0(...)
//    if (p->arr[size - 1].exp != 0) {
//        Poly result = PolyClone(p);
//        result.arr = (Mono*) safeRealloc(result.arr, sizeof(Mono) * (size + 1));
//
//        result.size++;
//        result.arr[size] = MonoFromPoly(q, 0);
//
//        return result;
//    }
//
//    //przypadek że istnieje czynnik x_0^0(...)
//    Poly result = PolyClone(p);
//    Poly res = PolyAdd(&p->arr[size - 1].p, q);
//
//    PolyDestroy(&result.arr[size - 1].p);
//    result.arr[size - 1].p = res;
//
//    if (PolyIsZero(&res)) {
//        result.arr = (Mono*) safeRealloc(result.arr, sizeof(Mono) * (size - 1));
//        result.size--;
//    }
//
//    if (result.size == 0) {
//        PolyDestroy(&result);
//
//        return PolyZero();
//    }
//
//    return result;

    assert(!PolyIsCoeff(a) && PolyIsCoeff(b));
    assert(isSorted(a));

    Poly helper = {.size = 1, .arr = safeMalloc(sizeof(Mono))};
    helper.arr[0].exp = 0;
    helper.arr[0].p = *b;

    return PolyAddProperty(&helper, a);
}

//funkcja pomocnicza do komentowania TODO
static Poly DeleteAllZeroFromPoly(size_t diffExps, Poly result) {
    //przepisujemy tak, żeby nie było wśród jednomianów jednomianów zerowych
    size_t zeros = 0;
    for (size_t i = 0; i < diffExps; i++) {
        if (PolyIsZero(&result.arr[i].p))
            zeros++;
    }

    Poly resultFinal = PolyCreate(diffExps - zeros);

    if (!PolyIsCoeff(&resultFinal)) {
        size_t id = 0;
        for(size_t i = 0; i < diffExps; i++) {
            if (!PolyIsZero(&result.arr[i].p)) {
                resultFinal.arr[id] = result.arr[i];
                id++;
            }
        }

        //jeśli wielomian jest współczynnikiem, zwracamy współczynnik
        if (resultFinal.size == 1 && PolyIsCoeff(&resultFinal.arr[0].p) && resultFinal.arr[0].exp == 0) {
            poly_coeff_t coeffFinal = resultFinal.arr[0].p.coeff;
            PolyDestroy(&resultFinal);
            free(result.arr);

            return PolyFromCoeff(coeffFinal);
        }
    }

    free(result.arr);

    return  resultFinal;
}


/**
 * Dodaje dwa wielomiany, które nie są współczynnikami.
 * @param[in] a : wielomian
 * @param[in] b : wielomian
 * @return @f$a + b@f$
 */
static Poly NonCoeffAddNonCoeff(Poly *a, Poly *b) {
    assert(!PolyIsCoeff(a) && !PolyIsCoeff(b));
    assert(isSorted(a) && isSorted(b));

    size_t size = a->size + b->size;

    Poly result = PolyCreate(size);
    size_t counter = 0;
    size_t pId = 0, qId = 0;

    while (pId < a->size && qId < b->size) {
        if (compareMonosByExp(&a->arr[pId], &b->arr[qId]) == 0) {
            result.arr[counter].p = PolyAddProperty(&a->arr[pId].p, &b->arr[qId].p);
            result.arr[counter].exp = a->arr[pId].exp;

            pId++;
            qId++;
        } else {
            if (compareMonosByExp(&a->arr[pId], &b->arr[qId]) > 0) {
                result.arr[counter] = a->arr[pId];
                pId++;
            } else {
                result.arr[counter] = b->arr[qId];
                qId++;
            }
        }
        counter++;
    }

    while (pId < a->size) {
        result.arr[counter] = a->arr[pId];
        pId++;
        counter++;
    }
    while (qId < b->size) {
        result.arr[counter] = b->arr[qId];
        qId++;
        counter++;
    }

    free(a->arr);
    free(b->arr);

    return DeleteAllZeroFromPoly(counter, result);
}

// TODO
static Poly PolyAddProperty(Poly *a, Poly *b) {

    if (PolyIsZero(a)) {
        PolyDestroy(a);
        return *b;
    }
    else if (PolyIsZero(b)) {
        PolyDestroy(b);
        return *a;
    }

    if (PolyIsCoeff(a) && PolyIsCoeff(b))
        return CoeffAddCoeff(a, b);
    else if (!PolyIsCoeff(a) && PolyIsCoeff(b))
        return NonCoeffAddCoeff(a, b);
    else if (PolyIsCoeff(a) && !PolyIsCoeff(b))
        return NonCoeffAddCoeff(b, a);
    else if (!PolyIsCoeff(a) && !PolyIsCoeff(b))
        return NonCoeffAddNonCoeff(a, b);

    assert (false);
}

/**
 * Mnoży dwa wielomiany które są współczynnikikami.
 * @param[in] p : wielomian (współczynnik)
 * @param[in] q : wielomian (współczynnik)
 * @return @f$p \cdot q@f$
 */
static Poly CoeffMulCoeff(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));

    poly_coeff_t value = p->coeff * q->coeff;

    return PolyFromCoeff(value);
}

/**
 * Mnoży dwa wielomiany - wielomian (nie będący współczynnikiem) i wpółczynnik.
 * @param[in] p : wielomian
 * @param[in] q : wielomian (współczynnik)
 * @return @f$p \cdot q@f$
 */
static Poly NonCoeffMulCoeff(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && PolyIsCoeff(q));
    assert(isSorted(p));

    if (PolyIsZero(q))
        return PolyZero();

    size_t size = p->size;
    Poly result = PolyCreate(size);

    size_t zeros = 0;
    size_t id = 0;
    for (size_t i = 0; i < size; i++) {
        Poly poly = PolyMul(&p->arr[i].p, q);

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

/**
 * Mnoży dwa wielomiany, które nie są współczynnikami.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return @f$p \cdot q@f$
 */
static Poly NonCoeffMulNonCoeff(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));
    assert(isSorted(p) && isSorted(q));

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

// TODO
Poly PolyNegProperty(Poly *a) {
    if (PolyIsCoeff(a)) {
        a->coeff *= -1;
    }
    else {
        for (size_t i = 0; i < a->size; ++i) {
            PolyNegProperty(&a->arr[i].p);
        }
    }

    return *a;
}

/**
 * Podnosi a do potęgi x.
 * @param[in] a : podstawa
 * @param[in] x : wykładnik
 * @return @f$ a^x @f$
 */
static poly_coeff_t power(poly_coeff_t a, poly_exp_t x) {
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
static int compareMonosByExpQsort(const void *a, const void *b) {
    return -compareMonosByExp((Mono*) a, (Mono*) b);
}

/**
 * Sortuje tablicę jednomianów po wykładnikach.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return
 */
static void sortMonosByExp(size_t count, Mono *monos) {
    qsort(monos, count, sizeof(Mono), compareMonosByExpQsort);
}

/**
 * Robi płytką kopię tablicy jednomianów.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return skopiowana tablica jednomianów
 */
static Mono* copyMonoArray(size_t count, const Mono monos[]) {
    Mono *result = safeMalloc(sizeof(Mono) * count);

    for (size_t i = 0; i < count; i++)
        result[i] = monos[i];

    return result;
}

// TODO
static Mono* deepCopyMonoArray(size_t count, const Mono monos[]) {
    Mono *result = safeMalloc(sizeof(Mono) * count);

    for (size_t i = 0; i < count; i++)
        result[i] = MonoClone(&monos[i]);

    return result;
}

// TODO
static Poly PolyAddMonosForAll(size_t count, Mono *myMonos) {
    sortMonosByExp(count, myMonos);

    if (count == 1) {
        if (PolyIsCoeff(&myMonos[0].p) && myMonos[0].exp == 0) {
            poly_coeff_t coeffFinal = myMonos[0].p.coeff;
            free(myMonos);

            return PolyFromCoeff(coeffFinal);
        }
    }

    if (count < 1) {
        free(myMonos);
        return PolyZero();
    }

    size_t diffExps = howManyDiffExp(count, myMonos);
    Poly result = PolyCreate(diffExps);
    Poly helper = myMonos[0].p;
    poly_exp_t helperExp = myMonos[0].exp;
    size_t resultId = 0;

    for (size_t i = 1; i < count; i++) {
        if (PolyIsZero(&myMonos[i].p))
            continue;

        if (compareMonosByExp(&myMonos[i - 1], &myMonos[i]) == 0) {
            Poly pom = PolyAddProperty(&helper, &myMonos[i].p);
            helper = pom;

            continue;
        }

        result.arr[resultId] = (Mono) {.p = helper, .exp = helperExp};

        helper = myMonos[i].p;
        helperExp = myMonos[i].exp;
        resultId++;
    }

    if (resultId < diffExps)
        result.arr[diffExps - 1] = (Mono) {.p = helper, .exp = helperExp};

    free(myMonos);
    return DeleteAllZeroFromPoly(diffExps, result);
}

Poly PolyCreate(size_t size) {
    if (size == 0)
        return (Poly) {.coeff = 0, .arr = NULL};

    return (Poly) {.size = size, .arr = safeMalloc(sizeof(Mono) * size)};
}

void PolyDestroy(Poly *p) {
    if (p->arr == NULL)
        return;

    for (size_t i = 0; i < p->size; i++)
        MonoDestroy(&p->arr[i]);

    free(p->arr);
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p))
        return (Poly) {.coeff = p->coeff, .arr = NULL};

    Poly p2 = *p;

    p2.arr = safeMalloc(sizeof(Mono) * p->size);

    for (size_t i = 0; i < p->size; i++)
        p2.arr[i] = MonoClone(&p->arr[i]);

    return p2;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly a = PolyClone(p);
    Poly b = PolyClone(q);

    return PolyAddProperty(&a, &b);
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    Mono *myMonos = copyMonoArray(count, monos);

    return PolyAddMonosForAll(count, myMonos);
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    return PolyAddMonosForAll(count, monos);
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    Mono *myMonos = deepCopyMonoArray(count, monos);

    return PolyAddMonosForAll(count, myMonos);
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

Poly PolyNeg(const Poly *p) {
    Poly a = PolyClone(p);

    return PolyNegProperty(&a);
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly q_neg = PolyNeg(q);
    Poly result = PolyAdd(p, &q_neg);
    PolyDestroy(&q_neg);

    return result;
}

poly_exp_t PolyDegBy(const Poly *p, size_t varIdx) {
    assert(isSorted(p));

    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    if (varIdx == 0)
        return p->arr[0].exp;

    poly_exp_t maxExp = 0;
    for (size_t i = 0; i < p->size; i++)
        maxExp = maxOfExp(PolyDegBy(&p->arr[i].p, varIdx - 1), maxExp);

    return maxExp;
}

poly_exp_t PolyDeg(const Poly *p) {
    assert(isSorted(p));

    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    poly_exp_t maxExp = 0;
    for (size_t i = 0; i < p->size; i++)
        maxExp = maxOfExp(PolyDeg(&p->arr[i].p) + p->arr[i].exp, maxExp);

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

    if (PolyIsCoeff(&result))
        return result;

    Poly resultFinal = result.arr[0].p;
    free(result.arr);

    return resultFinal;
}

Poly PolyCompose(const Poly *p, size_t k, Poly q[]) {
    return compose(p, k, q);
}