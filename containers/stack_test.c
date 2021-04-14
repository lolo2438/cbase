#include "stack.h"
#include <stdio.h>
#include "../constant.h"

int main(int argc, char **argv)
{
        int a[5] = {0, 1, 2, 3, 4};

        // Creating stack
        stack_t *stack = stack_create(3, sizeof(*a));

        // Inserting values until full
        stack_insert(stack, a+1);

        stack_insert(stack, a+0);

        stack_insert(stack, a+3);

        if(stack_insert(stack, a+4) == -EFULL)
                printf("stack is full!\n");

        // Popping values
        int b;

        stack_pop(stack, &b);
        printf("b=%d\n",b);

        stack_pop(stack, &b);
        printf("b=%d\n",b);

        // Inserting a new value to see how react
        stack_insert(stack, a+4);

        // Pop everything + pop when empty to see how react
        stack_pop(stack, &b);
        printf("b=%d\n",b);

        stack_pop(stack, &b);
        printf("b=%d\n",b);

        if(stack_pop(stack, &b) == -EEMPTY)
                printf("stack is empty!\n");

        stack_destroy(stack);

        return 0;
}
