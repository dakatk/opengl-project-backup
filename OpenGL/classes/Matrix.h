#ifndef MATRIX_H__
#define MATRIX_H__

#define MATRIX_SIZE 16
#define COL_SIZE 4

typedef float matrix[MATRIX_SIZE];

extern void dotProduct(matrix a, matrix b, matrix out);
extern void printMatrix(matrix m);

#endif
