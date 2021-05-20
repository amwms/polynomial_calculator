/** @file
  Implementacja stosu.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/05/2021
*/
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "poly.h"
#include "stack.h"
#include "parse.h"

bool hasPrefix(char **str, char *template) {
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

void ZERO(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    addStack(stack, PolyZero());
}

void IS_COEFF(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyIsCoeff(getFirstFromStack(stack)));
}

void IS_ZERO(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyIsZero(getFirstFromStack(stack)));
}

void CLONE(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = PolyClone(getFirstFromStack(stack));
    addStack(stack, p);
}

void ADD(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = removeStack(stack);
    Poly q = removeStack(stack);
    addStack(stack, PolyAdd(&p, &q));
}

void MUL(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = removeStack(stack);
    Poly q = removeStack(stack);
    addStack(stack, PolyMul(&p, &q));
}

void NEG(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    PolyNeg(&stack->polys[stack->sizeUsed - 1]);
}

void SUB(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = removeStack(stack);
    Poly q = removeStack(stack);
    addStack(stack, PolySub(&p, &q));
}

void IS_EQ(Stack *stack, int nr) {
    if (!hasTwoPolysStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyIsEq(getFirstFromStack(stack), getSecondFromStack(stack)));
}

void DEG(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyDeg(getFirstFromStack(stack)));
}

void DEG_BY(Stack *stack, size_t varIdx, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printf("%d\n", PolyDegBy(getFirstFromStack(stack), varIdx));
}

void AT(Stack *stack, poly_coeff_t x, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    Poly p = PolyAt(getFirstFromStack(stack), x);
    removeStack(stack);
    addStack(stack, p);
}

void PRINT(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    printPoly(getFirstFromStack(stack));
    printf("\n");
}

void POP(Stack *stack, int nr) {
    if (isEmptyStack(stack)) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", nr);
        return;
    }

    removeStack(stack);
}

void parseAndDoOperation(Stack *stack, char **verse, int nr) {
    if (hasPrefix(verse, "ZERO") && **verse == '\0') {
        ZERO(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "IS_COEFF") && **verse == '\0') {
        IS_COEFF(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "IS_ZERO") && **verse == '\0') {
        IS_ZERO(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "CLONE") && **verse == '\0') {
        CLONE(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "ADD") && **verse == '\0') {
        ADD(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "MUL") && **verse == '\0') {
        MUL(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "NEG") && **verse == '\0') {
        NEG(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "SUB") && **verse == '\0') {
        SUB(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "IS_EQ") && **verse == '\0') {
        IS_EQ(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "DEG") && **verse == '\0') {
        DEG(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "DEG_BY ")) {
        unsigned long long idx;
        if (isParseNumberULL(verse, &idx) && **verse == '\0') {
            DEG_BY(stack, idx, nr);
//            return;
        }
        fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", nr);
    }
    else if (hasPrefix(verse, "AT ")) {
        long long x;
        if (isParseNumberLL(verse, &x) && **verse == '\0') {
            AT(stack, x, nr);
//            return;
        }
        fprintf(stderr, "ERROR %d AT WRONG VALUE\n", nr);
    }
    else if (hasPrefix(verse, "PRINT") && **verse == '\0') {
        PRINT(stack, nr);
//        return;
    }
    else if (hasPrefix(verse, "POP") && **verse == '\0') {
        POP(stack, nr);
//        return
    }
    else {
        fprintf(stderr, "ERROR %d WRONG COMMAND\n", nr);
    }
}