/** @file
  Stos

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 02/05/2021
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

Stack *createStack();

void addStack(Stack *stack, Poly p);

Poly removeStack(Stack *stack);

Poly *getFirstFromStack(Stack *stack);

Poly *getSecondFromStack(Stack *stack);

bool isEmptyStack(Stack *stack);

bool hasTwoPolysStack(Stack *stack);

#endif