#ifndef __matrix_H__
#define __matrix_H__

// Todo: matrix_t header before data array
#include <stddef.h>

struct matrix;

int matrix_create(struct matrix **M, const size_t count,
        const size_t rows, const size_t cols, const float *data);

int matrix_copy(struct matrix **M, const struct matrix *A);

void matrix_destroy(struct matrix *M, const size_t count);

// c = a + b
int matrix_add(struct matrix *C,
        const struct matrix *A, const struct matrix *B);

// c = a - b
int matrix_sub(struct matrix *C,
        const struct matrix *A, const struct matrix *B);

// cA = c * A
int matrix_scalarmult(struct matrix *cA,
        float c, const struct matrix *A);

// C = A * B
int matrix_mult(struct matrix *C,
        const struct matrix *A, const struct matrix *B);

// abs(a - b) < epsilon ?
int matrix_isequal(const struct matrix *A,
        const struct matrix *B, float epsilon);

// A -> At
int matrix_transpose(struct matrix *At, const struct matrix *A);

/* TODO
// det(A)
float matrix_GetDet(const struct matrix *A);

// X = rref(A|K)
int matrix_RRef(struct matrix *X,
        const struct matrix *A, const struct matrix *K);

// A^-1
int matrix_Inv(struct matrix *NA, const struct matrix *A);
*/

void matrix_Print(const struct matrix *A);

#endif
