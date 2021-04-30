/* File name: hashmap.h
 * Author: Laurent Tremblay
 * Date: 26-04-2021
 * Description:
 * Interface functions for the hashmap module. This module provides a typeless
 * hashmap. Data is copied inside the hashmap, therefore it is stack
 * and heap friendly. Try to use references to only store the addresses of your
 * heap allocated objects. The hashmap uses linked lists as collision
 * mitigation.
 */
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef struct hashmap hashmap_t;

/* Function: hashmap_create
 * Arguments:
 *      size_t hm_size:
 *              The ammount of memory that will be initially allocated
 *              for the hashmap. Every item stored will be put at a position in
 *              the map as a "node" for collision management.
 *
 *      size_t (*hash_fnct)(void *key, size_t key_size):
 *              The hash function to be used by the hashmap. This function must
 *              be supplied by the end user.
 *
 *              They key argument takes a pointer to the "key", which should be
 *              interpreted as a series of unsigned bytes.
 *              Key_size is the size in bytes of the key.
 *
 * Return Value:
 *      A valid hashmap_t object or NULL on failure.
 *
 * Description:
 *      Creates an opaque hashmap object of hm_size using the hash function
 *      hash_fnct.
 *
 * Example use:
 *      size_t hash(void *key, size_t key_size) { function body };
 *
 *      hashmap_t *h = hashmap_create(5, hash);
 */
hashmap_t *hashmap_create(size_t hm_size,
        size_t (*hash_fnct)(void *key, size_t key_size));


/* Function: hashmap_resize
 * Argument:
 *      hashmap_t **hm:
 *              A reference to a hashmap object to be resized.
 *
 *      size_t new_size:
 *              The new size of the hashmap.
 *
 * Return value:
 *      EXIT_SUCCESS: Successfully resized the hashmap
 *      -EARG : Argument supplied are invalid
 *      -EALLOC: Error while allocating memory
 *
 * Description:
 *      Resizes the hasmpap supplied to the new size by creating a new hashmap
 *      and transfering every items stored the old hashmap to the new one.
 *
 * Example use:
 *      hashmap_t *h = hashmap_create(5, hash);
 *      hashmap_resize(&h, 9);
 */
int hashmap_resize(hashmap_t **hm, size_t new_size);


/* Function: hashmap_store
 * Argument:
 *      hashmap_t *hm:
 *              A valid hashmap object.
 *
 *      void *key:
 *              The key associated with the data
 *
 *      size_t key_size:
 *              The size in BYTES of the key
 *
 *      void *data:
 *              The data to be stored in the hashmap
 *
 *      size_t data_size:
 *              The size in Bytes of the data to be stored
 *
 * Return value:
 *      EXIT_SUCCESS: Successfully resized the hashmap
 *      -EARG : Argument supplied are invalid
 *      -EALLOC: Error while allocating memory
 *
 * Description:
 *      Stores the data in the hashmap alongside it's access key.
 *      The data presented is copied
 *      (HINT) Use addesses of heap allocated
 *
 * Example use:
 *      int a = 5;
 *      int *b = malloc(...)
 *      // storing the value 5
 *      hashmap_store(hm, a_key, a_key_size, &a, sizerof(a));
 *      // storing the address of the object! -> less space used in the hm
 *      hashmap_store(hm, b_key, b_key_size, &b, sizeof(b));
 */
int hashmap_store(hashmap_t *hm, void *key, size_t key_size,
        void *data, size_t data_size);


/* Function: hashmap_access
 * Argument:
 *      hashmap_t *hm:
 *              A valid hashmap object
 *
 *      void *key:
 *              The key associated with the data
 *
 *      size_t key_size:
 *              The size in BYTES of the key
 *
 *      void **data:
 *              A reference to store the pointer to data
 *
 *      size_t *data_size:
 *              A reference to store the size of the data pointer
 *
 * Return value:
 *      EXIT_SUCCESS: Function exited correctly
 *      -EARG: Argument error
 *
 * Description:
 *      Looks for the data associated with it's key in the hashmap.
 *      Function will return EXIT_SUCCESS if it either finds or does not find
 *      data. If no data is found, Data will be équal to NULL and data_size
 *      to 0.
 *
 * Example use:
 *      // Ex: we stored a=5
 *      int *data;
 *      size_t data_size;
 *      hashmap_access(hm, key, key_size, &data, &data_size);
 *      if(data != NULL && data_size == sizeof(int))
 *              printf("%d", a)
 */
int hashmap_access(hashmap_t *hm, void *key, size_t key_size,
        void **data, size_t *data_size);


/* Function: hashmap_remove
 * Argument:
 *      hashmap_t *hm:
 *              A valid hashmap object
 *
 *      void *key:
 *              The key associated with the data
 *
 *      size_t key_size:
 *              The size in BYTES of the key
 *
 * Return value:
 *      EXIT_SUCCESS: Function exited correctly
 *      -EARG: Argument error
 *
 * Description:
 *      Removes the data associated with the specified key.
 *      Warning: If a dynamically allocated object is stored, you will need
 *               access it first and then free it if you do not have access the
 *               data pointer before hand
 */
int hashmap_remove(hashmap_t *hm, void *key, size_t key_size);


/* Function: hashmap_destroy
 * Argument:
 *      hashmap_t *hm:
 *              A valid hashmap object
 *
 * Return value:
 *      EXIT_SUCCESS: Function exited correctly
 *      -EARG: Argument error
 *
 * Description:
 *      Removes all the data in the hashmap.
 *      Warning: If a dynamically allocated object is stored, you will need
 *               access it first and then free it if you do not have access the
 *               data pointer before hand. Otherwise there will be memory leaks.
 */
void hashmap_destroy(hashmap_t *hm);

#endif
