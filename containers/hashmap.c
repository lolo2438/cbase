#include "hashmap.h"
#include "../constant.h"

#include <stdlib.h>
#include <string.h>


struct hdata {
        void *key;
        size_t key_size;
        void *data;
        size_t data_size;
        struct hdata *next;
};


static struct hdata *hdata_create(void *key, size_t key_size,
        void *data, size_t data_size)
{
        struct hdata *new = malloc(sizeof(*new));
        if(!new)
                goto hdata_ealloc;

        *new = (const struct hdata) {
                .key            = malloc(key_size),
                .key_size       = key_size,
                .data           = malloc(data_size),
                .data_size      = data_size,
                .next           = NULL
        };

        if (!new->key)
                goto hdata_key_ealloc;

        if (!new->data)
                goto hdata_data_ealloc;

        memcpy(new->key, key, new->key_size);
        memcpy(new->data, data, new->data_size);

        return new;

hdata_data_ealloc:
        free(new->key);
hdata_key_ealloc:
        *new = (const struct hdata) { 0 };
        free(new);
hdata_ealloc:
        return NULL;
}


static void hdata_destroy(struct hdata *hdata)
{
        if (hdata) {
                if(hdata->key)
                        free(hdata->key);
                if(hdata->data)
                        free(hdata->data);

                *hdata = (const struct hdata) { 0 };
                free(hdata);
        }
}


/* Hashmap */
struct hashmap {
        struct hdata **p_nodes;
        size_t size;
        size_t (*hash)(void *key, size_t key_size);
};

hashmap_t *hashmap_create(size_t hm_size,
        size_t (*hash_fnct)(void *key, size_t key_size))
{

        hashmap_t *hm = malloc(sizeof(*hm));
        if (!hm)
                goto hm_ealloc;

        *hm = (const struct hashmap) {
                .p_nodes = malloc(hm_size * sizeof(struct hdata*)),
                .size    = hm_size,
                .hash    = hash_fnct
        };

        if (!hm->p_nodes)
                goto hm_data_ealloc;

        memset(hm->p_nodes, 0, hm_size * sizeof(*(hm->p_nodes)));

        return hm;

hm_data_ealloc:
        free(hm);
hm_ealloc:
        return NULL;
}


int hashmap_resize(hashmap_t **hm, size_t new_size)
{
        if (!hm)
                return -EARG;

        if ((*hm)->size == new_size)
                return -EARG;

        hashmap_t *new_hm = hashmap_create(new_size, (*hm)->hash);
        if(!new_hm)
                return -EALLOC;

        for (size_t i = 0; i < (*hm)->size; i += 1) {

                if ((*hm)->p_nodes[i]) {

                        struct hdata *hn = (*hm)->p_nodes[i];

                        while (hn) {
                                size_t index =
                                new_hm->hash(hn->key, hn->key_size) %
                                        new_hm->size;

                                if (!new_hm->p_nodes[index]) {
                                        new_hm->p_nodes[index] = hn;
                                } else {
                                        struct hdata *n =
                                                new_hm->p_nodes[index];

                                        while(n->next)
                                                n = n->next;

                                        n->next = n;
                                }
                                hn = hn->next;
                        }
                }
        }

        if ((*hm)->p_nodes)
                free((*hm)->p_nodes);

        **hm = (struct hashmap) { 0 };
        free(*hm);

        *hm = new_hm;

        return EXIT_SUCCESS;
}


int hashmap_store(hashmap_t *hm, void *key, size_t key_size,
        void *data, size_t data_size)
{
        if (!hm || !key || !data)
                return -EARG;

        size_t index = hm->hash(key, key_size) % hm->size;

        // First hit
        if (!(hm->p_nodes[index])) {

                struct hdata *new_node;
                new_node = hdata_create(key, key_size, data, data_size);
                if(!new_node)
                        return -EALLOC;

                hm->p_nodes[index] = new_node;

                return EXIT_SUCCESS;
        }

        // Else check all nodes for one that already exists
        struct hdata *hnode = hm->p_nodes[index];
        struct hdata *l_hnode;
        while (hnode) {
                if (hnode->key_size == key_size) {
                        if (memcmp(hnode->key, key, key_size) == 0) {
                                return -3; //already there
                        }
                }
                l_hnode = hnode;
                hnode = hnode->next;
        }

        // None found -> append to list
        struct hdata *new_node;
        new_node = hdata_create(key, key_size, data, data_size);
        if (!new_node)
                return -EALLOC;

        l_hnode->next = new_node;

        return EXIT_SUCCESS;
}


int hashmap_access(hashmap_t *hm, void *key, size_t key_size,
        void **data, size_t *data_size)
{
        if (!hm || !key || !data || !data_size)
                return -EARG;

        size_t index = hm->hash(key, key_size) % hm->size;

        struct hdata *hnode = hm->p_nodes[index];

        while (hnode) {
                if (hnode->key_size == key_size) {
                        if (memcmp(hnode->key, key, key_size) == 0){

                                *data = hnode->data;
                                *data_size = hnode->data_size;

                                return EXIT_SUCCESS;
                        }
                }
                hnode = hnode->next;
        }

        *data = NULL;
        *data_size = 0;

        return EXIT_SUCCESS;
}


int hashmap_remove(hashmap_t *hm, void *key, size_t key_size)
{
        if (!hm || !key)
                return -EARG;

        size_t index = hm->hash(key, key_size) % hm->size;

        struct hdata *hnode = hm->p_nodes[index];
        struct hdata *last_hnode = NULL;

        while (hnode) {
                if (hnode->key_size == key_size) {
                        if (memcmp(hnode->key, key, key_size) == 0){

                                if (!last_hnode)
                                        hm->p_nodes[index] = hnode->next;
                                else
                                        last_hnode->next = hnode->next;

                                hdata_destroy(hnode);

                                return EXIT_SUCCESS;
                        }
                }
                last_hnode = hnode;
                hnode = hnode->next;
        }

        return EXIT_SUCCESS;
}

/* Issue:
 * At the moment, the destroy function will free all nodes, but if the data
 * is a dynamically object, it will loose the pointer to that object forever.
 */
void hashmap_destroy(hashmap_t *hashmap)
{
        if (!hashmap)
                return;

        if(hashmap->p_nodes) {
                for (size_t i = 0; i < hashmap->size; i += 1) {

                        if (hashmap->p_nodes[i]) {

                                struct hdata *node = hashmap->p_nodes[i];
                                struct hdata *next_node = NULL;

                                do {
                                        next_node = node->next;
                                        hdata_destroy(node);

                                } while (next_node);
                        }
                }
                free(hashmap->p_nodes);
        }

        *hashmap = (struct hashmap) { 0 };

        free(hashmap);
}
