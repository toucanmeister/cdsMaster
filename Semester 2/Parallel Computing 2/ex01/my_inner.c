#include <stdio.h>
#include <stdlib.h>
int main()
{
    int* u = malloc(5 * sizeof(int));
    int* v = malloc(5 * sizeof(int));
    for (size_t i = 0; i < 5; ++i) {
        u[i]=1;
        v[i]=i;
    }
    int b=0;
    for (size_t j = 0; j < 5; ++j) {
        b += u[j]*v[j];
    }
    printf("%d ", b);
    free(u);
    free(v);
}