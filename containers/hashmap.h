#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef struct hashmap hashmap_t;

hashmap_t *hashmap_create(size_t hm_size,
        size_t (*hash_fnct)(void *key, size_t key_size));

int hashmap_resize(hashmap_t **hm, size_t new_size);

int hashmap_add(hashmap_t *hm, void *key, size_t key_size,
        void *data, size_t data_size);

int hashmap_access(hashmap_t *hm, void *key, size_t key_size,
        void **data, size_t *data_size);

int hashmap_remove(hashmap_t *hm, void *key, size_t key_size);

void hashmap_destroy(hashmap_t *hm);

#endif
