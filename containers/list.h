#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct list list_t;

typedef unsigned int pos_t;

list_t *list_create(size_t data_size);

int list_prepend(list_t *list, void *data);

int list_append(list_t *list, void *data);

int list_insert(list_t *list, pos_t pos, void *data);

int list_get(list_t *list, pos_t pos, void *data);

int list_pop_first(list_t *list, void *data);

int list_pop_last(list_t *list, void *data);

int list_pop(list_t *list, pos_t pos, void *data);

void list_destroy(list_t *list);

int list_get_size(list_t *list);

#endif
