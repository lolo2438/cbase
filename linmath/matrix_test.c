#include "matrix.h"
#include <stdio.h>

int main(int argc, char **argv)
{
        struct matrix *A, *B, *C;

        matrix_create(&A, 1, 2, 2);
        matrix_create(&B, 1, 2, 2);
        matrix_create(&C, 1, 2, 2);

        matrix_add(C, A, B);
        matrix_print(C);

        matrix_sub(C, A, B);
        matrix_print(C);

        print("isequal? %d\n", matrix_isequal(A, B, 0.00001));

        matrix_scalarmult(C, 3, A);
        matrix_print(C);

        matrix_mult(C, A, B);
        matrix_print(C);

        matrix_transpose(C, A);
        matrix_print(C);

        return 0;
}
