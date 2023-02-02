// g++ sol2_3.cpp m n p
#include <iostream>
#include <chrono>
#include <iomanip>

int** allocArray(int m, int n){
	int **array = new int*[m];
	int *a = new int[m*n];
	for(int i=0;i<m;i++)
		array[i] =&a[n*i];
	return array;
}

void freeArray(int **array, int m, int n) {
    free(array[0]);
    free(array);
}

void printArray(int **array, int m, int n){
  for (int i = 0; i < m ; ++i){
    for (int j = 0; j < n; ++j){
      std::cout <<  std::setw (2)  << array[i][j] << ' ';
	}
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void randomMatrix(int** array, int m, int n) {     
  int i, j;
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      array[i][j]=rand() % 4;
    }
  }
}

void fillZero(int** array, int m, int n) {     
  int i, j;
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      array[i][j]=0;
    }
  }
}

void copyMat(int** arr1, int** arr2, int m, int n) {     
  int i, j;
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      arr2[i][j]=arr1[i][j];
    }
  }
}

void matMulrow(int** X, int** Y, int** Z, int m, int n, int p){
  for (int i=0; i < m; i++) {
    for (int j=0; j < p; j++) {
      for (int k=0; k < n; k++) {
        Z[i][j] = Z[i][j] + X[i][k]*Y[k][j];
      }
    }
  }
}

void matMulcol(int** X, int** Y, int** Z, int m, int n, int p){
  for (int j=0; j < p; j++) {
    for (int i=0; i < m; i++) {
      for (int k=0; k < n; k++) {
        Z[i][j] = Z[i][j] + X[i][k]*Y[k][j];
      }
    }
  }
}

int main(int argc, char **argv)
{
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
	using std::chrono::duration;
  using std::chrono::milliseconds;

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int p = atoi(argv[3]);

  int** X = allocArray(m,n);
  int** Y = allocArray(n,p);
  int** Z = allocArray(m,p);
  int** temp = allocArray(m,p);

  randomMatrix(X,m,n);
  randomMatrix(Y,n,p);
  randomMatrix(Z,m,p);
  copyMat(Z, temp, m,p);

  auto t1_begin = high_resolution_clock::now();
  matMulrow(X, Y, temp, m, n, p);
  auto t1_end = high_resolution_clock::now();

  copyMat(Z, temp, m,p);

  auto t2_begin = high_resolution_clock::now();
  matMulrow(X, Y, temp, m, n, p);
  auto t2_end = high_resolution_clock::now();

  duration<double, std::milli> real_time1 = t1_end - t1_begin;
  duration<double, std::milli> real_time2 = t2_end - t2_begin;

  std::cout<<"--------------------------"<<std::endl;
	std::cout <<"Row Version:     " << real_time1.count() << "ms\n";
	std::cout <<"Col Version:     " << real_time2.count() << "ms\n";

  freeArray(X,m,n);
  freeArray(Y,n,p);
  freeArray(Z,m,p);
  freeArray(temp,m,p);

  return 0;
}
