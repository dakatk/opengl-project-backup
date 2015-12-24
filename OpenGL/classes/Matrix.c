#include "Matrix.h"

#include <stdio.h>

void dotProduct(matrix a, matrix b, matrix out)
{
	int i, j, k;
	float sum = 0;
	for(i = 0; i < COL_SIZE; i ++)
		for(j = 0; j < COL_SIZE; j ++)
		{
			for(k = 0; k < COL_SIZE; k ++)
				sum += a[i + k * COL_SIZE] * b[k + j * COL_SIZE];

			out[i + j * COL_SIZE] = sum;
			sum = 0;
		}
}

void printMatrix(matrix m)
{
	printf("{");

	int i, j;
	for(i = 0; i < COL_SIZE; i ++)
	{
		for(j = 0; j < COL_SIZE; j ++)
			printf("%f%s", m[i + j * COL_SIZE], (j == COL_SIZE-1 ? "" : ", "));
		printf("\n");
	}
	printf("}\n");
}
