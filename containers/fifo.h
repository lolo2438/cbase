#ifndef FIFO_H
#define FIFO_H

#include <stddef.h>

typedef struct fifo fifo_t;

fifo_t *fifo_create(size_t fifo_size, size_t data_size);

int fifo_insert(fifo_t *fifo, void *data);

int fifo_pop(fifo_t *fifo, void *data);

void fifo_destroy(fifo_t *fifo);

#endif
