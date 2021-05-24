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

/** To jest stała reprezentująca domyślą wielkość stosu przy jego tworzeniu*/
#define INITIAL_STACK_SIZE 4

/**
 * Struktura trzymająca stos. Implementacja stosu jest za pomocą tablicy.
 * Struktura trzyma tablicę, która jest rozszerzana dynamicznie, trzymającą elementy stosu
 * oraz ile elementów jest na stosie i ile jeszcze zostało miejsca na stosie (przed rozszerzeniem go).
 */
typedef struct Stack {
    /** To jest tablica trzymająca wielomiany znajdujące się na stosie.*/
    Poly *polys;
    /** To jest zmienna mówiąca ile wielomianów jest już na stosie.*/
    int sizeUsed;
    /** To jest zmienna mówiąca ile miejsca zostało na stosie (przed rozszerzeniem tablicy polys).*/
    int sizeFree;
} Stack;

/**
 * Tworzy stos.
 * @return stworzony pusty stos
*/
Stack *createStack();

/**
 * Dodawanie do stosu.
 * @param[in] stack : stos
 * @param[in] p : wielomian
 * @return
*/
void addStack(Stack *stack, Poly p);

/**
 * Usuwanie pierwszego elementu ze stosu.
 * @param[in] stack : stos
 * @return usunięty element ze stosu
*/
Poly removeStack(Stack *stack);

/**
 * Bierze pierwszy element ze stosu.
 * @param[in] stack : stos
 * @return pierwszy element ze stosu
*/
Poly *getFirstFromStack(Stack *stack);

/**
 * Bierze drugi element ze stosu.
 * @param[in] stack : stos
 * @return drugi element ze stosu
*/
Poly *getSecondFromStack(Stack *stack);

/**
 * Sprawdza czy stos jest pusty.
 * @param[in] stack : stos
 * @return czy stos jest pusty
*/
bool isEmptyStack(Stack *stack);

/**
 * Sprawdza czy stos ma dwa elementy.
 * @param[in] stack : stos
 * @return czy stos ma dwa elementy
*/
bool hasTwoPolysStack(Stack *stack);

#endif