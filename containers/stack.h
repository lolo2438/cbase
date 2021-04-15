#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct stack stack_t;

stack_t *stack_create(const size_t stack_size, const size_t data_size);

int stack_insert(stack_t *stack, void *data);

int stack_pop(stack_t *stack, void *data);

void stack_destroy(stack_t *stack);

#endif
