#include <iostream>
#include <stdlib.h>
#include <chrono>

using namespace std;

#define NUM_ITERATIONS 50
#define MAX_SIZE 40000000

char read_data(char* data, long size) {
    char tmp;
    for (int i=0; i<size; i++) {
        tmp += data[i];
    }
    return tmp;
}

void fill_data(char* data, unsigned size) {
    for (unsigned i=0; i < size; i++) {
        data[i] = (char) rand();
    }
}

int main() {
    char trash;
    cout << "Finding out cache sizes" << endl;
    srand(0);
    for (long size=1024; size < MAX_SIZE; size*=2) {
        char *data = (char*) malloc(size);
        fill_data(data, size);
        read_data(data, size); // Data should now be cached
        auto start = chrono::high_resolution_clock::now();
        for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++) {
            trash += read_data(data, size);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end-start);
        long size_in_kib = size / 1024;
        cout << "Avg. time per KiB for " << size_in_kib << " KiB: " << duration.count()/(size_in_kib*NUM_ITERATIONS) << "ns" << endl;
        free(data);
    }
}
