/*----------------------------------------------------------------

*

* Programación Avanzada: Apuntadores y arreglos

* Fecha: 22-Ene-2019

* Autor: A01700820 Carlos Román

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

typedef struct {
	double *data;
	uint nrows, ncols;
} Matrix;

Matrix* alloc_matrix(uint nrows, uint ncols) {

 	if (nrows <= 0 || ncols <= 0) {
		printf("Rows and Cols must be greater than zero.\n");
		return NULL;
	}

	Matrix* M = (Matrix*) malloc(sizeof(Matrix));

	if (M == NULL) {
		printf("Couldn't allocate memory for the structure.\n");
		return NULL;
	}

	M->nrows = nrows;
	M->ncols = ncols;
	M->data = (double*) malloc(sizeof(double) * nrows * ncols);

	if (M->data == 0) {
		printf("Couldn't allocate memory for the matrix.\n");
		free(M);
		return NULL;
	}

	// Initialize Matrix to Zero
	for(int i = 0 ; i < (nrows * ncols) ; i++){
		M->data[i] = 0;
	}

	return M;

}

void printm(Matrix *M) {
	for(int i = 0 ; i < M->nrows ; i++){
	  for(int j = 0 ; j < M->ncols ; j++){
	    printf("%.2f ", M->data[(M->ncols * i) + j]);
	    printf("\t");
	  }
	  printf("\n");
	}
}

void set(Matrix *M, uint row, uint col, double val) {
	if(M != NULL){
		if(row < M->nrows && col < M->ncols) {
			M->data[M->ncols * row + col] = val;
		} else{
			printf("Index out of bounds.\n");
		}
	}else{
		printf("Matrix was null.\n");
	}
}

void matrix_mult(Matrix *A, Matrix *B, Matrix *C) {

	if(A == NULL || B == NULL){
		printf("The matrixes must be valid.\n");
		return;
	}

	if(A->ncols == B->nrows){
		for (int i = 0; i < A->nrows; i++){
			for (int j = 0; j < B->ncols ; j++){
				for (int k = 0; k < B->nrows; k++){
					C->data[i * (C->ncols) + j] += (A->data[i * (A->ncols) + k] * B->data[k * (B->ncols) + j]);
				}
			}
		}
	}else{
		printf("The matrixes must be valid.\n");
	}
}

void free_matrix(Matrix *M) {
	if (M != NULL) {
		free(M->data);
		free(M);
		printf(" matrix freed.\n");
	} else{
		printf(" matrix must be valid.\n");
	}
}


int main(int argc, char* argv[]) {
	printf("Creating the matrix A.\n");
	Matrix *A = alloc_matrix(3, 2);

	printf("Setting the matrix A.\n");
	set(A, 0, 0, 1.2);
	set(A, 0, 1, 2.3);
	set(A, 1, 0, 3.4);
	set(A, 1, 1, 4.5);
	set(A, 2, 0, 5.6);
	set(A, 2, 1, 6.7);
	printf("Printing the matrix A:\n");
	printm(A);
	printf("\n");

	printf("Creating the matrix B.\n");
	Matrix *B = alloc_matrix(2, 3);
	printf("Setting the matrix B.\n");
	set(B, 0, 0, 5.5);
	set(B, 0, 1, 6.6);
	set(B, 0, 2, 7.7);
	set(B, 1, 0, 1.2);
	set(B, 1, 1, 2.1);
	set(B, 1, 2, 3.3);
	printf("Printing the matrix B:\n");
	printm(B);
	printf("\n");

	printf("Creating the matrix C.\n");
	Matrix *C = alloc_matrix(3, 3);

	printf("A x B = C\n");
	matrix_mult(A, B, C);
	printf("Printing the matrix C:\n");
	printm(C);
	printf("\n");

	// Nueva matriz porque B X A debería dar una matriz 2 X 2
	Matrix *AUX = alloc_matrix(2, 2);

	printf("B x A = C:\n");
	matrix_mult(B, A, AUX);
	printf("Printing the matrix C:\n");
	printm(AUX);
	printf("\n");

	Matrix *D = NULL;
	printf("Setting a NULL matrix (D):\n");
	set(D, 0, 0, 10);
	printf("\n");

	printf("A x D(NULL) = C:\n");
	matrix_mult(A, D, C);
	printf("\n");

	printf("D(NULL) x A = C:\n");
	matrix_mult(D, A, C);
	printf("\n");

	printf("Allocating E with (0,0): \n");
	Matrix *E = alloc_matrix(0, 0);
	printf("\n");

	printf("Freeing A: ");
	free_matrix(A);
	printf("Freeing B: ");
	free_matrix(B);
	printf("Freeing C: ");
	free_matrix(C);
	printf("Freeing D(NULL): ");
	free_matrix(D);
	printf("Freeing E(NULL): ");
	free_matrix(E);

	// Free extra matrix.
	printf("Freeing AUX: ");
	free_matrix(AUX);

	return 0;
}
