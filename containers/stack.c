#include "stack.h"
#include "../constant.h"

#include <stdlib.h>
#include <string.h>

struct stack{
	void *data;
	size_t data_size;
	size_t data_count;

	size_t stack_size;
	size_t stack_ptr;
};


stack_t *stack_create(size_t stack_size, size_t data_size){

	struct stack *stack = malloc(sizeof(*stack));
	if(!stack)
		goto stack_ealloc;

	struct stack _stack = {
		.data 		= malloc(data_size * stack_size),
		.data_size 	= data_size,
		.data_count 	= 0,
		.stack_size 	= stack_size,
		.stack_ptr 	= 0
	};

	if (!_stack.data)
		goto stack_data_ealloc;

	*stack = _stack;

	return stack;

stack_data_ealloc:
	free(stack);
stack_ealloc:
	return NULL;
}


int stack_insert(stack_t *stack, void *data){

	if(!stack || !data)
		return -EARG;

	if(stack->data_count < stack->stack_size){

		char *addr = stack->data;
		size_t offset = stack->data_size * stack->stack_ptr++;

		memcpy(addr + offset, data, stack->data_size);

		stack->data_count += 1;

		return EXIT_SUCCESS;
	} else {
		return -EFULL;
	}

}


int stack_pop(stack_t *stack, void *data){

	if(!stack || !data)
		return -EARG;

	if(stack->data_count > 0){

		char* addr = stack->data;
		size_t offset = stack->data_size * --stack->stack_ptr;

		memcpy(data, addr + offset, stack->data_size);

		stack->data_count -= 1;

		return EXIT_SUCCESS;
	} else {
		return -EEMPTY;
	}
}


void stack_destroy(stack_t *stack){

	if(stack){
		if(stack->data)
			free(stack->data);

		free(stack);
	}
}
