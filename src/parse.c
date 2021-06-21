/** @file
  Implementacja parsowania wiersza potencjalnie zawierająca wielomian.

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

/**
 * Zwalnia pamięć użytą na tablicę elementów typu Mono.
 * @param[in] monos : tablica jednomianów
 * @param[in] used : liczba użytych miejsc w tablicy jednomianów
*/
static void freeMemoryUsedForMonos(Mono *monos, size_t used) {
    for (size_t i = 0; i < used; i++) {
        MonoDestroy(&monos[i]);
    }

    free(monos);
}

/**
 * Wypisuje jednomian.
 * @param[in] mono : jednomian
*/
static void printMono(Mono *mono) {
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

    if (isDigit(*str[0]) || *str[0] == '-') {
        errno = 0;
        *number = strtoll(*str, str, 10);

        if (errno == 0)
            return true;
    }

    *str = begin;
    return false;
}

bool isParseNumberULL(char **str, unsigned long long *number) {
    char *const begin = *str;

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

static inline bool isParsePoly(char **verse, Poly *p);

/**
 * Parsowanie potencjalnego jednomianu.
 * @param[in] str : wiersz
 * @param[in] mono : jednomian
 * @return czy to co było parsowane zostało poprawnie sparsowane jako jednomian
*/
static bool isParseMono(char **str, Mono *mono) {
    Poly p = PolyZero();

    if (**str == '(') {
        (*str)++;

        if (**str != '\0' && isParsePoly(str, &p)) {
            mono->p = p;
            if (**str == ',') {
                (*str)++;
                unsigned long long exp;
                if (**str != '\0' && isParseNumberULL(str, &exp)) {
                    if (exp <= INT_MAX) { // sprawdzanie czy zakres exp jest poprawny
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

/**
 * Dynamicznie rozszerza tablicę jednomianów (jeśli trzeba) i dodaje do niej jednomian.
 * @param[in] free : ilość wolnego miejsca (w tablicy jednomianów)
 * @param[in] used : ilość zużytego miejsca (w tablicy jednomianów)
 * @param[in] monos : tablica jednomianów
 * @param[in] addMono : jednomian, który chcemy dodać do tablicy
*/
static void dynamicAddToMonos(size_t *free, size_t *used, Mono **monos, Mono addMono) {
    if (*free == 0) {
        *monos = (Mono*) safeRealloc(*monos, sizeof(Mono) * (*used) * 2);
        *free = *used;
    }
    (*monos)[(*used)++] = addMono;
    (*free)--;
}

/**
 * Parsowanie potencjalnego wielomianu.
 * @param[in] str : wiersz
 * @param[in] poly : wielomian
 * @return czy to co było parsowane zostało poprawnie sparsowane jako wielomian
*/
static bool isParsePoly(char **str, Poly *poly) {
    long long number;

    if (isParseNumberLL(str, &number)) {
        *poly = PolyFromCoeff(number);
        return true;
    }

    Mono mono;
    Mono *monos = (Mono*) safeMalloc(sizeof (Mono) * INITIAL_MONOS_SIZE);
    size_t usedSpace = 0;
    size_t freeSpace = INITIAL_MONOS_SIZE;

    if (**str != '\0' && isParseMono(str, &mono)) {
        dynamicAddToMonos(&freeSpace, &usedSpace, &monos, mono);

        while (**str == '+') {
            (*str)++;

            if (!(**str != '\0' && isParseMono(str, &mono))) {
                freeMemoryUsedForMonos(monos, usedSpace);
                return false;
            }

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

/**
 * Parsowanie wiersza potencjalnie zawierającego wielomian.
 * @param[in] str : wiersz
 * @param[in] poly : wielomian
 * @return czy to co było parsowane zostało poprawnie sparsowane jako wielomian
*/
static bool isParseVerse(char **str, Poly *poly) {
    if (isParsePoly(str, poly)) {
        if (**str == '\0')
            return true;
    }

    return false;
}

void parseVerse(char **str, Stack  *stack, int nr) {
    Poly poly = PolyZero();

    if (isParseVerse(str, &poly)) {
        addStack(stack, poly);
        return;
    }

    PolyDestroy(&poly);
    fprintf(stderr, "ERROR %d WRONG POLY\n", nr);
}