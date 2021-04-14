#include "list.h"
#include "../constant.h"

#include <stdio.h>

int main(int argc, char **argv)
{

        int a[5] = {0,1,2,3,4};

        list_t *list = list_create(sizeof(int));
        if(!list){
                printf("Error allocating list\n");
                return -1;
        }

        list_prepend(list, a+0);

        list_prepend(list, a+2);

        list_append(list, a+4);

        list_insert(list, 2, a+3);

        // Out of bound
        list_insert(list, 8, a+1);

        printf("Inserted all values\n");

        // [2][0][4][3][1]

        int b;
        list_get(list, 3, &b);
        printf("%d\n",b);

        list_pop(list, 2, &b);
        printf("%d\n",b);

        list_pop_last(list, &b);
        printf("%d\n",b);

        list_pop_first(list, &b);
        printf("%d\n",b);

        // [0][3]

        list_pop(list, 2, &b);
        printf("%d\n",b);

        // [0]

        list_append(list, a + 4);

        while(list_get_size(list)){
                list_pop_first(list, &b);
                printf("%d\n",b);
        }




        list_destroy(list);
}
