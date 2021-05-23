/** @file
  Implementacja stosu.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/05/2021
*/
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "memory.h"
#include "poly.h"
#include "parse.h"

//TODO - PRINT FORM END TO BEGINING OF MONOS ARRAY
void freeMemoryUsedForMonos(Mono *monos, size_t used) {
    for (size_t i = 0; i < used; i++) {
        MonoDestroy(&monos[i]);
    }

    free(monos);
}

void printMono(Mono *mono) {
    printf("(");
    printPoly(&mono->p);
    printf(",%d)", mono->exp);
}

void printPoly(Poly *poly) {
    if (PolyIsCoeff(poly)) {
        printf("%ld", poly->coeff);
        return;
    }

    for (size_t i = poly->size; i > 0; i--) {
        printMono(&poly->arr[i - 1]);

        if (i > 1)
            printf("+");
    }

}

bool isDigit(char c) {
    return c <= '9' && c >= '0';
}

bool isParseNumberLL(char **str, long long *number) {
    char *const begin = *str;
    bool isMinus = false;

    if (*str[0] == '-') {
        isMinus = true;
        (*str)++;
    }

//    if (*str[0] == '0') {
//        *number = 0;
//        (*str)++;
//        return true;
//    }

    if (isDigit(*str[0])) {
        errno = 0;
        *number = strtoll(*str, str, 10);

        if (errno == 0) {
            if (isMinus)
                *number *= -1;
            return true;
        }
    }

    *str = begin;
    return false;
}

bool isParseNumberULL(char **str, unsigned long long *number) {
    char *const begin = *str;

//    if (*str[0] == '0') {
//        *number = 0;
//        (*str)++;
//        return true;
//    }

    if (isDigit(*str[0])) {
        errno = 0;
        *number = strtoull(*str, str, 10);

        if (errno == 0) {
            return true;
        }
    }

    *str = begin;
    return false;
}

bool isParseCoeff(char **str, Poly *p) {
    long long nrLL = 0;

    if (isParseNumberLL(str, &nrLL)) {
        *p = PolyFromCoeff(nrLL);
        return true;
    }

    return false;
}

bool isParseMono(char **str, Mono *mono) {
    Poly p = PolyZero();

    if (**str == '(') {
        (*str)++;

        if (**str != '\0' && isParsePoly(str, &p)) {
            mono->p = p;
            if (**str == ',') {
                (*str)++;
                unsigned long long exp;
                if (**str != '\0' && isParseNumberULL(str, &exp)) {
                    if (exp >= 0 && exp <= INT_MAX) { // checking if range of exp is correct
                        mono->exp = (poly_exp_t) exp;
                        if (**str == ')') {
                            (*str)++;
                            return true;
                        }
                    }
                }
            }
        }
    }

    PolyDestroy(&p);
    return false;
}

static void dynamicAddToMonos(size_t *free, size_t *used, Mono **monos, Mono addMono) {
//    printf("BEFORE USED: %zu, FREE: %zu\n", *used, *free);
    if (*free == 0) {
        *monos = (Mono*) safeRealloc(*monos, sizeof(Mono) * (*used) * 2);
        *free = *used;
    }
    (*monos)[(*used)++] = addMono;
//    printf("AFTER USED: %zu, FREE: %zu\n", *used, *free);
    (*free)--;
}

bool isParsePoly(char **str, Poly *poly) {
    long long number;

    if (isParseNumberLL(str, &number)) {
        *poly = PolyFromCoeff(number);
        return true;
    }
//    printf("%c   ", **str);
    Mono mono;
    Mono *monos = (Mono*) safeMalloc(sizeof (Mono) * 4);
    size_t usedSpace = 0;
    size_t freeSpace = 4;

    if (**str != '\0' && isParseMono(str, &mono)) {
        dynamicAddToMonos(&freeSpace, &usedSpace, &monos, mono);

        while (**str == '+') {
            (*str)++;
//            printf("%c   ", **str);
            if (!(**str != '\0' && isParseMono(str, &mono))) {
                freeMemoryUsedForMonos(monos, usedSpace);
                return false;
            }
//            printMono(&mono);
            dynamicAddToMonos(&freeSpace, &usedSpace, &monos, mono);
        }

        monos = (Mono*) safeRealloc(monos, sizeof(Mono) * usedSpace);
        *poly = PolyAddMonos(usedSpace, monos);
        free(monos);

        return true;
    }

    freeMemoryUsedForMonos(monos, usedSpace);
    return false;
}

bool isParseVerse(char **str, Poly *poly) {
    if (isParsePoly(str, poly)) {
        if (**str == '\0')
            return true;
    }

    return false;
}

void parseVerse(char **str, Stack  *stack, int nr) {
    Poly poly = PolyZero();
//    if (isParsePoly(str, &poly) && (**str) == '\0') { //making change - also check if it is the end of the line
    if (isParseVerse(str, &poly)) {
        addStack(stack, poly);
//        printPoly(&poly);
//        printf("\n");
        return;
    }

    PolyDestroy(&poly);
    fprintf(stderr, "ERROR %d WRONG POLY\n", nr);
}