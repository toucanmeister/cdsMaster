#include<stdio.h>
int main() {
    char c = 'T'; // Char variable
    char *ptr = &c; // Pointer variable holding address
    printf("Size of pointer: %ld bytes", sizeof(ptr));
}