/** @file
  Implementacja stosu.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/05/2021
*/
#include <stdbool.h>
#include "memory.h"
#include "poly.h"
#include "stack.h"

Stack *createStack() {
    Stack *stack = (Stack*) safeMalloc(sizeof(Stack));

    stack->polys = (Poly*) safeMalloc(sizeof(Poly) * INITIAL_STACK_SIZE);
    stack->sizeUsed = 0;
    stack->sizeFree = INITIAL_STACK_SIZE;

    return stack;
}

void addStack(Stack *stack, Poly p) {
    if (stack->sizeFree == 0) {
        stack->polys = (Poly*) safeRealloc(stack->polys, sizeof(Poly) * stack->sizeUsed * 2);
        stack->sizeFree = stack->sizeUsed;
    }

    stack->polys[stack->sizeUsed] = p;
    stack->sizeUsed += 1;
    stack->sizeFree -= 1;
}

Poly removeStack(Stack *stack) {
    assert(stack->sizeUsed > 0);

    Poly p = stack->polys[stack->sizeUsed - 1];
    stack->sizeUsed -= 1;
    stack->sizeFree += 1;

    if (stack->sizeUsed <= stack->sizeFree && stack->sizeUsed + stack->sizeFree > INITIAL_STACK_SIZE) {
        stack->polys = (Poly*) safeRealloc(stack->polys, sizeof(Poly) * (stack->sizeUsed + stack->sizeFree) / 2);
        stack->sizeFree = (stack->sizeUsed + stack->sizeFree) / 2 - stack->sizeUsed;
    }

    return p;
}

Poly *getFirstFromStack(Stack *stack) {
    return &stack->polys[stack->sizeUsed - 1];
}

Poly *getSecondFromStack(Stack *stack) {
    return &stack->polys[stack->sizeUsed - 2];
}

bool isEmptyStack(Stack *stack) {
    return stack->sizeUsed == 0;
}

bool hasTwoPolysStack(Stack *stack) {
    return stack->sizeUsed >= 2;
}

bool hasKPolysStack(Stack *stack, size_t k) {
    return stack->sizeUsed >= k;
}
