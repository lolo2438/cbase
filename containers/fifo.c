#include "fifo.h"
#include "../constant.h"

#include <stdlib.h>
#include <string.h>

struct fifo{
	void *data;
	size_t data_size;
	size_t data_count;
	size_t data_ptr;
	size_t fifo_size;
	size_t fifo_ptr;
};


fifo_t *fifo_create(const size_t fifo_size, const size_t data_size)
{
	struct fifo *fifo = malloc(sizeof(*fifo));
	if (!fifo)
		goto fifo_ealloc;

	*fifo = (const struct fifo) {
		.data 		= malloc(data_size * fifo_size),
		.data_size 	= data_size,
		.data_ptr 	= 0,
		.fifo_size 	= fifo_size,
		.fifo_ptr 	= 0
	};

	if (!fifo->data)
		goto fifo_data_ealloc;

	return fifo;

fifo_data_ealloc:
	free(fifo);

fifo_ealloc:
	return NULL;
}


int fifo_insert(fifo_t *fifo, void *data)
{
	if (!fifo || !data)
		return -EARG;

	if (fifo->data_count < fifo->fifo_size) {

		char* addr = fifo->data;
		size_t offset = fifo->data_size * fifo->data_ptr;

		memcpy(addr + offset, data, fifo->data_size);

		fifo->data_ptr += 1;
		if(fifo->data_ptr == fifo->fifo_size)
			fifo->data_ptr = 0;

		fifo->data_count += 1;

		return EXIT_SUCCESS;
	} else {
		return -EFULL;
	}
}


int fifo_pop(fifo_t *fifo, void *data)
{
	if (!fifo || !data)
		return -EARG;

	if (fifo->data_count > 0) {

		char* addr = fifo->data;
		size_t offset = fifo->data_size * fifo->fifo_ptr;

		memcpy(data, addr + offset, fifo->data_size);

		memset(addr + offset, 0, fifo->data_size);

		fifo->fifo_ptr += 1;

		if(fifo->fifo_ptr == fifo->fifo_size)
			fifo->fifo_ptr = 0;

		fifo->data_count -= 1;

		return EXIT_SUCCESS;

	} else {
		return -EEMPTY;
	}
}


void fifo_destroy(fifo_t *fifo)
{
	if (fifo) {
		if (fifo->data)
			free(fifo->data);

		*fifo = (const struct fifo) { 0 };
		free(fifo);
	}
}
