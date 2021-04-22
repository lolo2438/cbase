#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "matrix.h"
#include "../constant.h"

#define EPSILON 0.000001

#define FLT_EQUAL(op1,op2,eps) 	(((op1)-(op2) < 0) ? 			\
				(((op2) - (op1) < (eps)) ? 1:0) : 	\
				(((op1) - (op2) < (eps)) ? 1:0))

struct matrix {
	float *data;
	size_t rows;
	size_t cols;
};


int matrix_create(struct matrix **M, const size_t count,
	const size_t rows, const size_t cols, const float *data)
{
	if (!M)
		return -EARG;

	*M = malloc(sizeof(**M) * count);
	if (!*M)
		goto m_ealloc;

	const size_t matrix_size = rows * cols;

	size_t i;
	for (i = 0; i < count; i += 1) {
		*M[i] = (const struct matrix) {
			.data = malloc(sizeof((*M)->data) * matrix_size),
			.rows = rows,
			.cols = cols
		};

		if (!(*M)->data)
			goto m_data_ealloc;
	}

	if (data)
		memcpy((*M)->data, data,
			sizeof(*((*M)->data)) * matrix_size);

	return EXIT_SUCCESS;

m_data_ealloc:
	for (size_t j = 0; j < i; j += 1) {
		free((*M)[j].data);
	}
	free(*M);

m_ealloc:
	return -EALLOC;
}

int matrix_copy(struct matrix **M, const struct matrix *A)
{
	if(!M || !A)
		return -EARG;

	if(matrix_create(M, 1, A->rows, A->cols, NULL))
		return -EALLOC;

	memcpy((*M)->data, A->data,
		sizeof((*M)->data) * (*M)->rows * (*M)->cols);

	return EXIT_SUCCESS;
}

void matrix_Destroy(struct matrix *M, const size_t count)
{
	if (!M)
		return;

	for (size_t i = 0; i < count; i += 1) {
		free(M[i].data);
	}

	memset(M, 0, sizeof(*M) * count);
	free(M);
}


int matrix_add(struct matrix *C,
	const struct matrix *A, const struct matrix *B)
{
	if (!C || !A || !B)
		return -EARG;

	if (A->rows != B->rows || A->cols != B->cols
		|| A->rows != C->rows  || A->cols != C->cols)
		return -EARG;

	size_t matrix_size = C->rows * C->cols;

	for (size_t i = 0; i < matrix_size; i += 1) {
		C->data[i] = A->data[i] + B->data[i];
	}

	return EXIT_SUCCESS;
}


int matrix_sub(struct matrix *C,
	const struct matrix *A, const struct matrix *B)
{
	if(!C || !A || !B)
		return -EARG;

	if (A->rows != B->rows || A->cols != B->cols
		|| A->rows != C->rows  || A->cols != C->cols)
		return -EARG;

	size_t matrix_size = C->rows * C->cols;

	for (size_t i = 0; i < matrix_size; i += 1) {
		C->data[i] = A->data[i] - B->data[i];
	}

	return EXIT_SUCCESS;
}


int matrix_scalarmult(struct matrix *cA,
	float c, const struct matrix *A)
{
	if(!cA|| !A)
		return -EARG;

	if (A->rows != cA->rows || A->cols != cA->cols)
		return -EARG;

	size_t matrix_size = cA->rows * cA->cols;

	for (size_t i = 0; i < matrix_size; i += 1) {
		cA->data[i] = c * A->data[i];
	}

	return EXIT_SUCCESS;
}


int matrix_mult(struct matrix *C,
	const struct matrix *A, const struct matrix *B)
{
	if(!C || !A || !B)
		return -EARG;

	if (A->cols != B->rows || A->rows != C->rows ||
		B->cols != C->cols)
		return -EARG;

	for (size_t i = 0; i < C->rows; i += 1) {
		for (size_t j = 0; j < C->cols; j += 1) {
			for (size_t k = 0; k < A->cols; k += 1) {

				C->data[i * C->rows + j] +=
					(A->data[i * A->cols + k] *
					B->data[k * A-> cols + j]);
			}
		}
	}

	return EXIT_SUCCESS;
}


int matrix_isequal(const struct matrix *A,
	const struct matrix *B, float epsilon)
{
	if (!A || !B)
		return -EARG;

	if (A->rows != B->rows || A->cols != B->cols)
		return FAILED;

	size_t matrix_size = A->rows * A->cols;

	for (size_t i = 0; i < matrix_size; i += 1) {
		if (!FLT_EQUAL(A->data[i], B->data[i], epsilon))
			return FAILED;
	}

	return SUCCEDED;
}

/*
	[ a00, a01 ]      [ a00, a10, a20 ]
	[ a10, a11 ]  ->  [ a01, a11, a21 ]
	[ a20, a21 ]
*/
int matrix_transpose(struct matrix *At, const struct matrix *A)
{
	if (!At || !A)
		return -EARG;

	if (At->rows != A->cols || At->cols != A->rows)
		return -EARG;

	for (size_t i = 0; i < At->rows; i += At->rows) {
		for (size_t j = 0; j < At->cols; j += At->cols) {
			At->data[i + j] = A->data[j + i];
		}
	}

	return EXIT_SUCCESS;
}


/* Strategie:

		L0: [ X0 Y0 Z0 ]
		L1: [ X1 Y1 Z1 ]
		L2: [ X2 Y2 Z2 ]

	1: L1 = L1 - X1/X0 * L0
	   L2 = L2 - X2/X0 * L0

	2: L2 - L2 - Y2/Y1 * L1

	Note 1: If X0 or Y1 = 0, Find another row that
	is not equal to 0 and swap with actual row
	If no other row is found, det will be equal to 0;
*/
/*
float Matrix_GetDet(const struct matrix *A)
{
	// Can't compute determinant of a non-square matrice
	if (A->rows != A->cols)
		return -EARG;

	struct matrix *ref;
	if(matrix_copy(&ref, A))
		return -EALLOC;

	unsigned int i, j, k, swap = 0;

	for (i = 0; i < ref->rows - 1; i++) {

		// Verify that our echelon is not == 0,
		// if it is, check for a row that is not 0 ans swap
		if (FLT_EQUAL(ref->data[i * ref->rows + 1], 0, EPSILON)) {

			// Check if there is another non-zero row in this column
			//to make our triangle matrix
			for (j = i + 1; j < ref->rows; j++) {

				// Check if data on next row is not equal to 0
				if (!FLT_EQUAL(ref->data[j][i], 0, EPSILON)) {

					// Swap rows
					float* tmpval = ref->data[i];
					ref->data[i] = ref->data[j];
					ref->data[j] = tmpval;

					swap = swap ? 0 : 1;
					break;
				}
			}

			// Here we have not found any other non-zero row at
			//this column, thus det = 0;
			if (j == ref->rows)
				return 0.0;
		}
		//Go to the next row to do our calculations
		for (j = 1 + i; j < ref->cols; j++) {

			// If the matrix data in this row is = 0 no need to do anything
			if (!FLT_EQUAL(ref->data[j][i], 0, EPSILON)) {

				float coeff = Ref.Data[j][i] / Ref.Data[i][i];

				// Columns matrix increments
				for (k = i; k < Ref.Cols; k++) {

					Ref.Data[j][k] -= (coeff * Ref.Data[i][k]);

				}
			}
		}
	}

	float det = swap ? -1.0f : 1.0f;

	for (i = 0; i < ref->rows; i++) {
		det *= ref->data[i * ref->rows + 1];
	}

	matrix_destroy(ref, 1);

	return det;
}

int Matrix_RRef(struct matrix* X, const struct matrix* A, const struct matrix* K) {

	// Nb of rows must be the same
	if (A->rows != K->rows) return -1;

	unsigned int Rows = A->rows;
	unsigned int Cols = A->cols + K->cols;

	// === Algorithm:
	//  1. Check if The pivot is equal to 0, if yes, try to switch row
	//  2. Get the value of the pivot, then divide the whole row by that value
	//  3. Get the value of the next rows, substract each value of the echelon row * that first next row value. Do it for all other rows.
	//  4. Do step 1-3 to make it echelon form
	//  5. Substract the row with the echelon to it's superior row.
	for (i = 0; i < Rows; i++) {

		//1. find a non-zero pivot if the actual pivot = 0
		if (FLT_EQUAL(rref.Data[i][i], 0, EPSILON)) {

			// Check if there is another non-zero row ain this column to make our triangle matrix
			for (j = i + 1; j < rref.Rows; j++) {

				// Check if data on next row is not equal to 0
				if (!FLT_EQUAL(rref.Data[j][i], 0, EPSILON)) {

					// Swap rows
					float* tempaddr = rref.Data[i];
					rref.Data[i] = rref.Data[j];
					rref.Data[j] = tempaddr;

					break;
				}
			}
		}

		//2. Divide the whole line by the pivot to make the pivot = 1
		float OneOverPivot = 1 / rref.Data[i][i];
		for (j = 0; j < Cols; j++) {
			rref.Data[i][j] *= OneOverPivot;
		}

		//3. Since the pivot = 1, substract other rows by its first data * stuff from the last row
		for (j = 0; j < Rows; j++) {

			if (i != j) {
				float FirstData = rref.Data[j][i];

				rref.Data[j][i] = 0.0f;		// Since pivot = 1, saves some calculation time

				for (k = 1 + i; k < Cols; k++) {
					rref.Data[j][k] -= (FirstData * rref.Data[i][k]);
				}
			}
		}
	}
	// == End Algorithm

	// Testing if the matrix A is a row echelon matrix
	for (i = 0; i < Rows; i++) {
		for (j = 0; j < A->Cols; j++) {

			float test = (i == j) ? 1.0f : 0.0f;

			if (!FLT_EQUAL(rref.Data[i][j], test, EPSILON)) {
				Matrix_Destroy(&rref);	// Deleted
				return -1;
			}
		}
	}

	struct Matrix Result = Matrix_Create(K->Rows, K->Cols);

	for (i = 0; i < Rows; i++) {
		memcpy(Result.Data[i] ,(rref.Data[i] + A->Cols), K->Cols * sizeof(float));
	}

	Matrix_Destroy(&rref);	// Deleted
	*X = Result;

	return 0;
}


int matrix_inv(struct matrix *NA, const struct matrix *A) {

	// InvMatrix must be a square matrix
	if (A->Rows != A->Cols)
		return -EARGS;

	struct Matrix I = Matrix_CreateIdentity(A->Rows);	// Delete matrix when done
	struct Matrix Result;

	int Res = Matrix_RRef(A, &I, &Result);
	Matrix_Destroy(&I);

	if (Res) {
		return Res;
	}

	return 0;
}

/*
// Calculates the row echelon form on self
void Matrix::Triangulate(void) {

	unsigned int i, j, k;

	for (i = 0; i < this->Rows - 1; i++) {

		// Verify that our echelon is not == 0,
		// if it is, check for a row that is not 0 ans swap
		if (FLT_EQUAL(this->Data[i][i], 0, EPSILON)) {

			// Check if there is another non-zero row ain this column to make our triangle matrix
			for (j = i + 1; j < this->Rows; j++) {

				// Check if data on next row is not equal to 0
				if (!FLT_EQUAL(this->Data[j][i], 0, EPSILON)) {

					// Swap rows
					float* tempaddr = this->Data[i];
					this->Data[i] = this->Data[j];
					this->Data[j] = tempaddr;

					break;
				}
			}
		}

		//Go to the next row to do our calculations
		for (j = 1 + i; j < this->Cols; j++) {

			// If the matrix data in this row is = 0 no need to do anything
			if (!FLT_EQUAL(this->Data[j][i], 0, EPSILON)) {

				float coeff = this->Data[j][i] / this->Data[i][i];

				// Columns matrix increments
				for (k = i; k < this->Cols; k++) {

					this->Data[j][k] -= (coeff * this->Data[i][k]);

				}
			}
		}
	}
}
*/


void Matrix_Print(const struct matrix *A)
{
	for (size_t i = 0; i < A->rows; i += A->rows) {
		printf("[ ");
		for (size_t j = 0; j < A->cols; j += 1) {
			printf("%2.4f", A->data[i + j]);
		}
		printf(" ]\n");
	}
	printf("\n");
}
