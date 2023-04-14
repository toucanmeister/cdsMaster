#include <stdio.h>
#include <stdlib.h>

double** alloc_mat(int* col, int* row) {
    double** mat = malloc(sizeof(double*) * (*row));
    for (int i=0; i < *row; i++) {
        mat[i] = malloc(sizeof(double) * (*col));
    }
    return mat;
}

void free_mat(double** A, int* row) {
    for (int i=0; i < *row; i++) {
        free(A[i]);
    }
    free(A);
}

int main()
{
    int num_rows = 3;
    int num_cols = 2;
    double** A = alloc_mat(&num_cols, &num_rows);
    for (int i=0; i < num_rows; i++) {
        for (int j=0; j < num_cols; j++) {
            A[i][j] = i+j;
            printf("%f ", A[i][j]);
        }
    }
    printf("\n");
    free_mat(A, &num_rows);
}