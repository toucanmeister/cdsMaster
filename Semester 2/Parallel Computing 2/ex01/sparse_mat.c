#include <stdio.h>
#include <stdlib.h>

typedef struct coo_mat{
    double* values;
    int* cols;
    int* rows;
} coo_mat;

coo_mat alloc_spmat(int nnz, int* col, int* row, double *A) {
    coo_mat mat;
    mat.values = malloc(sizeof(double) * nnz);
    mat.cols = malloc(sizeof(int) * nnz);
    mat.rows = malloc(sizeof(int) * (*row));
    return mat;
}

int main()
{
    return 0;
}