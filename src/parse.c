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

    for (size_t i = 0; i < poly->size; i++) {
        printMono(&poly->arr[i]);

        if (i < poly->size - 1)
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
    if (**str == '(') {
        (*str)++;

        Poly p;
        if (**str != '\0' && isParsePoly(str, &p)) {
            mono->p = p;
            if (**str == ',') {
                (*str)++;
                unsigned long long exp;
                if (**str != '\0' && isParseNumberULL(str, &exp)) {
                    if (exp >= 0 && exp <= INT_MAX) {
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

    return false;
}

void dynamicAddToMonos(size_t *free, size_t *used, Mono *monos, Mono addMono) {
    if (*free == 0) {
        monos = (Mono*) safeRealloc((monos), sizeof(Mono) * *used * 2);
        *free = *used;
    }

    monos[(*used)++] = addMono;
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
    size_t used = 0;
    size_t free = 4;

    if (**str != '\0' && isParseMono(str, &mono)) {
        dynamicAddToMonos(&free, &used, monos, mono);

        while (**str == '+') {
            (*str)++;
//            printf("%c   ", **str);
            if (!(**str != '\0' && isParseMono(str, &mono)))
                return false;
//            printMono(&mono);
            dynamicAddToMonos(&free, &used, monos, mono);
        }

        monos = (Mono*) safeRealloc(monos, sizeof(Mono) * used);
        *poly = PolyAddMonos(used, monos);

        return true;
    }

    return false;
}

bool isParseVerse(char **str, Poly *poly) {
    if (isParsePoly(str, poly)) {
        if (**str == '\0')
            return true;
    }

    return false;
}