#include "list.h"
#include "../constant.h"

#include <stdlib.h>
#include <string.h>

struct node {
	void *data;

	struct node *next;
	struct node *prev;
};

struct list {
	struct node *first_node;
	struct node *current_node;
	struct node *last_node;

	size_t data_size;
	size_t size;
	pos_t pos;
};


/* Local Functions */

static struct node *node_create(void *data, size_t data_size,
	struct node *prev, struct node *next)
{
	struct node *new_node = malloc(sizeof(*new_node));
	if(!new_node)
		goto node_ealloc;

	*new_node = (const struct node){
			.data = malloc(sizeof(data_size)),
			.next = next,
			.prev = prev
		};

	if(!new_node->data)
		goto node_data_ealloc;

	memcpy(new_node->data, data, data_size);

	return new_node;

node_data_ealloc:
	free(new_node);
node_ealloc:
	return NULL;
}


static void node_destroy(struct node *node)
{
	if(node) {
		if(node->data)
			free(node->data);

		*node = (const struct node) { 0 };
		free(node);
	}
}


static int list_set_pos(list_t *list, pos_t pos)
{
	if(list->size < pos)
		return FAILED;

	if (pos < list->pos) {
		if (0 + pos < list->pos - pos){
			list->current_node = list->first_node;
			list->pos = 0;
		}
	} else {
		if (list->pos + pos > list->size - pos){
			list->current_node = list->last_node;
			list->pos = list->size - 1;
		}
	}

	int dp = pos - list->pos;

	if(dp > 0){
		for(;list->pos < pos; list->pos += 1)
			list->current_node = list->current_node->next;
	} else if (dp < 0){
		for(;list->pos > pos; list->pos -= 1)
			list->current_node = list->current_node->prev;
	}

	return SUCCEDED;
}


/* Interface Function */
list_t *list_create(size_t data_size)
{
	struct list *list = malloc(sizeof(*list));
	if(!list)
		return NULL;

	*list = (const struct list) {
			.data_size = data_size
		};

	return list;
}


int list_prepend(list_t *list, void *data)
{
	if (!list || !data)
		return -EARG;

	struct node *new_node = node_create(data, list->data_size,
		NULL, list->first_node);
	if (!new_node)
		return -EALLOC;

	if (list->first_node)
		list->first_node->prev = new_node;
	else
		list->last_node = new_node;

	list->first_node = new_node;

	list->size += 1;

	list->current_node = new_node;
	list->pos = 0;

	return EXIT_SUCCESS;
}


int list_append(list_t *list, void *data)
{
	if(!list || !data)
		return -EARG;

	struct node *new_node = node_create(data, list->data_size,
		list->last_node, NULL);
	if (!new_node)
		return -EALLOC;

	if (list->last_node)
		list->last_node->next = new_node;
	else
		list->first_node = new_node;

	list->last_node = new_node;

	list->size += 1;

	list->current_node = new_node;
	list->pos = list->size - 1;

	return EXIT_SUCCESS;
}


int list_insert(list_t *list, pos_t pos, void *data)
{
	if(!list || !data)
		return -EARG;

	if (pos <= 0)
		return list_prepend(list, data);
	else if (pos >= list->size - 1)
		return list_append(list, data);

	if (!list_set_pos(list, pos))
		return -ESIZE;

	struct node *new_node = node_create(data, list->data_size,
		list->current_node, list->current_node->next);
	if(!new_node)
		return -EALLOC;

	new_node->next->prev = new_node;
	new_node->prev->next = new_node;

	list->current_node = new_node;

	list->size += 1;

	return EXIT_SUCCESS;
}


int list_get(list_t *list, pos_t pos, void *data)
{
	if(!list || !data)
		return -EARG;

	if(!list_set_pos(list, pos))
		return -ESIZE;

	struct node *node = list->current_node;

	memcpy(data, node->data, list->data_size);

	return EXIT_SUCCESS;
}


int list_pop_first(list_t *list, void *data)
{
	if(!list)
		return -EARG;

	struct node *node = list->first_node;
	if(!node)
		return -EEMPTY;

	if(data)
		memcpy(data, node->data, list->data_size);

	if(node->next)
		node->next->prev = NULL;

	list->first_node = node->next;

	node_destroy(node);

	list->size -= 1;

	list->current_node = list->first_node;
	list->pos = 0;

	return EXIT_SUCCESS;
}


int list_pop_last(list_t *list, void *data)
{
	if(!list)
		return -EARG;

	struct node *node = list->last_node;
	if(!node)
		return -EEMPTY;

	if(data)
		memcpy(data, node->data, list->data_size);

	if(node->prev)
		node->prev->next = NULL;

	list->last_node = node->prev;

	node_destroy(node);

	list->size -= 1;

	list->current_node = list->last_node;
	list->pos = list->size - 1;

	return EXIT_SUCCESS;
}


int list_pop(list_t *list, pos_t pos, void *data)
{
	if(!list)
		return -EARG;

	if(pos <= 0)
		return list_pop_first(list, data);
	else if (pos >= list->size - 1)
		return list_pop_last(list, data);

	if(!list_set_pos(list, pos))
		return -ERROR;

	struct node *node = list->current_node;
	if(!node)
		return -EEMPTY;

	if(data)
		memcpy(data, node->data, list->data_size);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	list->current_node = node->next;

	node_destroy(node);

	list->size -= 1;

	return EXIT_SUCCESS;
}


void list_destroy(list_t *list)
{
	if(!list)
		return;

	struct node *node = list->first_node;
	struct node *next_node;

	for(int i = 0; i < list->size && node != NULL; i += 1){
		next_node = node->next;
		node_destroy(node);
		node = next_node;
	}

	*list = (const struct list) { 0 };

	free(list);
}


int list_get_size(list_t *list)
{
	return (list ? list->size : 0);
}
