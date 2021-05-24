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

typedef struct Stack {
    Poly *polys;
    int sizeUsed;
    int sizeFree;
} Stack;

/**
 * Tworzy stos.
 * @return Zwraca stworzony stos.
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
 * @return Zwraca usunięty element.
*/
Poly removeStack(Stack *stack);

/**
 * Bierze pierwszy element ze stosu.
 * @param[in] stos : stos
 * @return Zwraca pierwszy element ze stosu.
*/
Poly *getFirstFromStack(Stack *stack);

/**
 * Bierze drugi element ze stosu.
 * @param[in] stos : stos
 * @return Zwraca drugi element ze stosu.
*/
Poly *getSecondFromStack(Stack *stack);

/**
 * Sprawdza czy stos jest pusty.
 * @param[in] stos : stos
 * @return Zwraca czy stos jest pusty.
*/
bool isEmptyStack(Stack *stack);

/**
 * Sprawdza czy stos ma dwa elementy.
 * @param[in] stos : stos
 * @return Zwraca czy stos ma dwa elementy.
*/
bool hasTwoPolysStack(Stack *stack);

#endif