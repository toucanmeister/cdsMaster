#include <iostream>
#include <chrono>
using namespace std;

void row_wise(int **x, int m, int n) {
  for (int i=0; i<m; i++) {
    for (int j=0; j<n; j++) {
      x[i][j] = i+j;
    }
  }
}

void column_wise(int **x, int m, int n) {
  for (int j=0; j<n; j++) {
    for (int i=0; i<m; i++) {
      x[i][j] = i+j;
    }
  }
}

double measure(int m, int n, bool rowwise) {
  int **x = new int*[m];
  for (int i=0; i < m; i++) {
    x[i] = new int[n];
  }
  if(rowwise) {
    auto trow_begin = chrono::high_resolution_clock::now(); 
    row_wise(x, m, n);
    auto trow_end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> row_time = trow_end - trow_begin;
    for (int i=0; i < m; i++) {
      delete [] x[i];
    }
    delete [] x;
    return row_time.count();
  } else {
    auto tcol_begin = chrono::high_resolution_clock::now();
    column_wise(x, m, n);
    auto tcol_end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> col_time = tcol_end - tcol_begin;
    for (int i=0; i < m; i++) {
      delete [] x[i];
    }
    delete [] x;
    return col_time.count();
  }
}

int main(int argc, char *argv[]) {
  cout << "i, row-wise, col-wise, row-wise/col-wise" << endl;
  for (int i=0; i <= 10000; i += 100) {
    double row_measure = measure(i, i, true);
    double col_measure = measure(i, i, false);
    cout << i << ", " << row_measure << ", " << col_measure << "," << row_measure/col_measure << endl;
  }
}
