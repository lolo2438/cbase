#include "hashmap.h"
#include "../constant.h"

#include <stdio.h>

size_t sbdm(void *key, size_t key_size)
{
        unsigned long hash = 0;
        unsigned char* ckey = (unsigned char*) key;

        for (size_t i = 0; i < key_size; i += 1) {
                hash = ckey[i] + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
}

int main(int argc, char **argv)
{
        hashmap_t *hashmap = hashmap_create(5, sbdm);

        hashmap_add(hashmap, "hello", 6, "oh hi", 6);

        int a = 5;

        hashmap_add(hashmap, (void*)&a, sizeof(a), "number 5", 9);

        hashmap_add(hashmap, (int[]){1, 2, 3}, sizeof(int) * 3,
                                (int[]){4, 5, 6}, sizeof(int) * 3);

        void *data;
        size_t data_size;

        hashmap_access(hashmap, (void*)&a, sizeof(a), &data, &data_size);
        printf("Key=5, data=%s\n",(char*)data);

        if(!hashmap_access(hashmap, "69420", 6, &data, &data_size))
                printf("Sorry your dank key is not present\n");

        hashmap_access(hashmap, (int[]){1,2,3}, sizeof(int) * 3,
                                &data, &data_size);

        for(size_t i = 0; i < data_size / sizeof(int); i += 1){
                printf("Key[%lu]=%d\n",i,((int*)data)[i]);
        }
        fflush(stdout);

        hashmap_access(hashmap, "hello", 6, &data, &data_size);
        printf("Removing: Key=hello, Data=%.*s\n",
                (int)data_size, (char*)data);

        hashmap_remove(hashmap, "hello", 6);
        // Do not access data and data_size after removing: memory is not
        // owned anymore!

        if(!hashmap_access(hashmap, "hello", 6, &data, &data_size))
                printf("Yup it's not there anymore\n");

        hashmap_resize(&hashmap, 4);
        printf("Resized hashmap to size = 4\n");

        hashmap_access(hashmap, (int[]){1,2,3}, sizeof(int) * 3,
                                &data, &data_size);

        for(size_t i = 0; i < data_size / sizeof(int); i += 1){
                printf("Key[%lu]=%d\n",i,((int*)data)[i]);
        }
        fflush(stdout);

        hashmap_access(hashmap, (void*)&a, sizeof(a), &data, &data_size);
        printf("Key=5, data=%s\n",(char*)data);

        // Resize to bigger size
        hashmap_resize(&hashmap, 6);
        printf("Resized hashmap to size = 6\n");

        hashmap_access(hashmap, (int[]){1,2,3}, sizeof(int) * 3,
                                &data, &data_size);

        for(size_t i = 0; i < data_size / sizeof(int); i += 1){
                printf("Key[%lu]=%d\n",i,((int*)data)[i]);
        }
        fflush(stdout);

        hashmap_access(hashmap, (void*)&a, sizeof(a), &data, &data_size);
        printf("Key=5, data=%s\n",(char*)data);

        hashmap_destroy(hashmap);

        return EXIT_SUCCESS;
}
