/** @file
  Implementacja działań w kalkulatorze.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/05/2021
*/
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "poly.h"
#include "stack.h"
#include "parse.h"

/**
 * Sprawdza czy wzór jest prefiksem stringa.
 * @param[in] str : string
 * @param[in] template : wzór
 * @return czy wzór jest prefiksem stringa
*/
static bool hasPrefix(char **str, char *template) {
    char *begin = *str;
    size_t size = strlen(template);

    for (size_t i = 0; i < size; i++) {
        if (**str != template[i]) {
            (*str) = begin;
            return false;
        }

        (*str)++;
    }

    return true;
}

/**
 * Implementacja komendy ZERO z treści zadania.
 * @param[in] stack : stos
 */
static void ZERO(Stack *stack) {
    addStack(stack, PolyZero());
}

/**
 * Implementacja komendy IS_COEFF z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void IS_COEFF(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyIsCoeff(getFirstFromStack(stack)));
}

/**
 * Implementacja komendy IS_ZERO z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void IS_ZERO(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyIsZero(getFirstFromStack(stack)));
}

/**
 * Implementacja komendy CLONE z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
void CLONE(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = PolyClone(getFirstFromStack(stack));
    addStack(stack, p);
}

/**
 * Implementacja komendy ADD z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void ADD(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = removeStack(stack);
    Poly q = removeStack(stack);
    addStack(stack, PolyAdd(&p, &q));

    PolyDestroy(&p);
    PolyDestroy(&q);
}

/**
 * Implementacja komendy MUL z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void MUL(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = removeStack(stack);
    Poly q = removeStack(stack);
    addStack(stack, PolyMul(&p, &q));

    PolyDestroy(&p);
    PolyDestroy(&q);
}

/**
 * Implementacja komendy NEG z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void NEG(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = PolyNeg(getFirstFromStack(stack));
    Poly toRemove = removeStack(stack);
    addStack(stack, p);

    PolyDestroy(&toRemove);
}

/**
 * Implementacja komendy SUB z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void SUB(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = removeStack(stack);
    Poly q = removeStack(stack);
    addStack(stack, PolySub(&p, &q));

    PolyDestroy(&p);
    PolyDestroy(&q);
}

/**
 * Implementacja komendy IS_EQ z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void IS_EQ(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyIsEq(getFirstFromStack(stack), getSecondFromStack(stack)));
}

/**
 * Implementacja komendy DEG z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void DEG(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyDeg(getFirstFromStack(stack)));
}

/**
 * Implementacja komendy DEG_BY z treści zadania.
 * @param[in] stack : stos
 * @param[in] varIdx : numer indeksu według, którego liczymy stopień
 * @param[in] nr : numer wiersza
 */
static void DEG_BY(Stack *stack, size_t varIdx, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyDegBy(getFirstFromStack(stack), varIdx));
}

/**
 * Implementacja komendy AT z treści zadania.
 * @param[in] stack : stos
 * @param[in] x : wartość, którą podstawiamy pod współczynnik x_0
 * @param[in] nr : numer wiersza
 */
static void AT(Stack *stack, poly_coeff_t x, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = PolyAt(getFirstFromStack(stack), x);
    Poly toRemove = removeStack(stack);
    addStack(stack, p);

    PolyDestroy(&toRemove);
}

/**
 * Implementacja komendy PRINT z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void PRINT(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printPoly(getFirstFromStack(stack));
    printf("\n");
}

/**
 * Implementacja komendy POP z treści zadania.
 * @param[in] stack : stos
 * @param[in] nr : numer wiersza
 */
static void POP(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly toRemove = removeStack(stack);
    PolyDestroy(&toRemove);
}

void parseAndDoOperation(Stack *stack, char **verse, int nr) {
    if (hasPrefix(verse, "ZERO") && **verse == '\0')
        ZERO(stack);
    else if (hasPrefix(verse, "IS_COEFF") && **verse == '\0')
        IS_COEFF(stack, nr);
    else if (hasPrefix(verse, "IS_ZERO") && **verse == '\0')
        IS_ZERO(stack, nr);
    else if (hasPrefix(verse, "CLONE") && **verse == '\0')
        CLONE(stack, nr);
    else if (hasPrefix(verse, "ADD") && **verse == '\0')
        ADD(stack, nr);
    else if (hasPrefix(verse, "MUL") && **verse == '\0')
        MUL(stack, nr);
    else if (hasPrefix(verse, "NEG") && **verse == '\0')
        NEG(stack, nr);
    else if (hasPrefix(verse, "SUB") && **verse == '\0')
        SUB(stack, nr);
    else if (hasPrefix(verse, "IS_EQ") && **verse == '\0')
        IS_EQ(stack, nr);
    else if (hasPrefix(verse, "DEG_BY") && (**verse == '@' || **verse == ' ' || **verse == '\0')) {
        if (**verse == ' ') {
            (*verse)++;
            unsigned long long idx;
            if (isParseNumberULL(verse, &idx) && **verse == '\0') {
                DEG_BY(stack, idx, nr);
                return;
            }
        }
        fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", nr);
    }
    else if (hasPrefix(verse, "DEG") && **verse == '\0')
        DEG(stack, nr);
    else if (hasPrefix(verse, "AT") && (**verse == '@' || **verse == ' ' || **verse == '\0')) {
        if (**verse == ' ') {
            (*verse)++;
            long long x;
            if (isParseNumberLL(verse, &x) && **verse == '\0') {
                AT(stack, x, nr);
                return;
            }
        }
        fprintf(stderr, "ERROR %d AT WRONG VALUE\n", nr);
    }
    else if (hasPrefix(verse, "PRINT") && **verse == '\0')
        PRINT(stack, nr);
    else if (hasPrefix(verse, "POP") && **verse == '\0')
        POP(stack, nr);
    else
        fprintf(stderr, "ERROR %d WRONG COMMAND\n", nr);
}