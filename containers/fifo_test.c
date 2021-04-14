#include "fifo.h"
#include <stdio.h>
#include "../constant.h"

int main(int argc, char **argv)
{
        int a[5] = {0, 1, 2, 3, 4};

        // Creating fifo
        fifo_t *fifo = fifo_create(3, sizeof(*a));

        // Inserting values until full
        fifo_insert(fifo, a+1);

        fifo_insert(fifo, a+0);

        fifo_insert(fifo, a+3);

        if(fifo_insert(fifo, a+4) == -EFULL)
                printf("Fifo is full!\n");

        // Popping values
        int b;

        fifo_pop(fifo, &b);
        printf("b=%d\n",b);

        fifo_pop(fifo, &b);
        printf("b=%d\n",b);

        // Inserting a new value to see how react
        fifo_insert(fifo, a+4);

        // Pop everything + pop when empty to see how react
        fifo_pop(fifo, &b);
        printf("b=%d\n",b);

        fifo_pop(fifo, &b);
        printf("b=%d\n",b);

        if(fifo_pop(fifo, &b) == -EEMPTY)
                printf("Fifo is empty!\n");

        fifo_destroy(fifo);

        return 0;
}
