#include <stdio.h>
#include <stdlib.h>


void outer_product(int n, int m, double *u, double *v, double **A) {
    for (int i=0; i < n; i++) {
        for (int j=0; j < m; j++) {
            A[i][j] = u[i] * v[j];
        }
    }
}

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
    int n = 3;
    int m = 4;
    double* u = malloc(sizeof(double) * n);
    double* v = malloc(sizeof(double) * m);
    for (int i=0; i < n; i++) {
        u[i] = i;
    }
    for (int j=0; j < m; j++) {
        v[j] = j;
    }
    double** A = alloc_mat(&m, &n);
    outer_product(n, m, u, v, A);
    for (int i=0; i < n; i++) {
        for (int j=0; j < m; j++) {
            printf("%f ", A[i][j]);
        }
    }
    free_mat(A, &n);

    return 0;
}