/** @file
  Stos

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 24/05/2021
*/

#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include <stddef.h>
#include "poly.h"
#include "memory.h"

#define INITIAL_STACK_SIZE 4

/**
 * Struktura trzymająca stos. Implementacja stosu jest za pomocą tablicy.
 * Struktura trzyma tablicę, która jest rozszerzana dynamicznie, trzymającą elementy stosu
 * oraz ile elementów jest na stosie i ile jeszcze zostało miejsca na stosie (przed rozszerzeniem go).
 */
typedef struct Stack {
    Poly *polys;
    int sizeUsed;
    int sizeFree;
} Stack;

/**
 * Tworzy stos.
 * @return stworzony pusty stos
*/
Stack *createStack();

/**
 * Dodawanie do stosu.
 * @param[in] stos : stos
 * @param[in] p : wielomian
 * @return
*/
void addStack(Stack *stack, Poly p);

/**
 * Usuwanie pierwszego elementu ze stosu.
 * @param[in] stos : stos
 * @return usunięty element ze stosu
*/
Poly removeStack(Stack *stack);

/**
 * Bierze pierwszy element ze stosu.
 * @param[in] stos : stos
 * @return pierwszy element ze stosu
*/
Poly *getFirstFromStack(Stack *stack);

/**
 * Bierze drugi element ze stosu.
 * @param[in] stos : stos
 * @return drugi element ze stosu
*/
Poly *getSecondFromStack(Stack *stack);

/**
 * Sprawdza czy stos jest pusty.
 * @param[in] stos : stos
 * @return czy stos jest pusty
*/
bool isEmptyStack(Stack *stack);

/**
 * Sprawdza czy stos ma dwa elementy.
 * @param[in] stos : stos
 * @return czy stos ma dwa elementy
*/
bool hasTwoPolysStack(Stack *stack);

#endif