// Program to multiply two matrices using nested loops
// gcc sol.cpp m n p
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

void printArray(int **array, int m, int n){ // print 2D array
  for (int i = 0; i < m ; ++i){
    for (int j = 0; j < n; ++j){
      std::cout <<  std::setw (2) << array[i][j] << ' ';
	}
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void printArray(int *array, int m, int n){ // print 1D array
  for (int i = 0; i < m*n ; ++i){
    std::cout <<  std::setw (2) << array[i] << ' ';
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

#define BLOCKSIZE 500 // choose it based on your cache size

int* flatRowmajor(int* ptrStart, int n){
  int *arr = new int[BLOCKSIZE*BLOCKSIZE];
  for (int i=0; i < BLOCKSIZE; i++) {
	for (int j=0; j < BLOCKSIZE; j++) {
	  arr[i*BLOCKSIZE+j] = ptrStart[i*n+j];
	}
  }
  return arr;
}


int* flatColmajor(int* ptrStart, int n){
  int *arr = new int[BLOCKSIZE*BLOCKSIZE];
  for (int i=0; i < BLOCKSIZE; i++) {
	for (int j=0; j < BLOCKSIZE; j++) {
	  arr[j*BLOCKSIZE+i] = ptrStart[i*n+j];
	}
  }
  return arr; 
}


void writeIn2D(int *f_Z, int* ptrStart, int n){
  for (int i=0; i < BLOCKSIZE; i++) {
	for (int j=0; j < BLOCKSIZE; j++) {
	  ptrStart[i*n+j] = f_Z[i*BLOCKSIZE+j];
	}
  }
}

void do_block(int* flatX, int* flatY, int* flatZ, int n){ // all matrixes are n * n 1D
  for (int i=0; i < BLOCKSIZE; i++) {
    for (int j=0; j < BLOCKSIZE; j++) {
      for (int k=0; k < BLOCKSIZE; k++) {
        flatZ[i*BLOCKSIZE + j] = flatZ[i*BLOCKSIZE + j] + flatX[i*BLOCKSIZE + k] * flatY[j*BLOCKSIZE + k];
      }
    }
  }
}

void gemm(int** X, int** Y, int** Z, int n)
{
  for (int i=0; i < n; i+=BLOCKSIZE) {
    for (int j=0; j < n; j+=BLOCKSIZE) {
	  int* flatZ = flatRowmajor(&(Z[i][j]), n);
      for (int k=0; k < n; k+=BLOCKSIZE) {
        int* flatX = flatRowmajor(&(X[i][k]), n);
		int* flatY = flatColmajor(&(Y[k][j]), n);
		
		do_block(flatX, flatY, flatZ, n);

		delete [] flatX;
		delete [] flatY;
      }
	  writeIn2D(flatZ, &(Z[i][j]), n);
	  delete [] flatZ;
    }
  }
  // do matrix matrix multiplication on main matrix and call do_block for inner (blocked) matrix converted to 1D array 
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

int main(int argc, char **argv)
{
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
	using std::chrono::duration;
  using std::chrono::milliseconds;
  // generate matrixes 2D
  // all matrixes are n * n
  int m = atoi(argv[1]); // enter all inputs same  
  int n = atoi(argv[2]);
  int p = atoi(argv[3]);

  int** X = allocArray(m,n);
  int** Y = allocArray(n,p);
  int** Z = allocArray(m,p);

  randomMatrix(X,m,n);
  randomMatrix(Y,n,p);
  fillZero(Z,m,p);

  auto t_begin = high_resolution_clock::now();
  gemm(X, Y, Z, m);
  auto t_end = high_resolution_clock::now();
  
  //printArray(Z, m, m);
  
  duration<double, std::milli> real_time = t_end - t_begin;
  
  fillZero(Z,m,p);
  matMulrow(X, Y, Z, m, m, m);

  //printArray(Z, m, m);

  std::cout<<"--------------------------"<<std::endl;
	std::cout<<"Blocked Version:     " << real_time.count() << "ms\n";

  freeArray(X,m,n);
  freeArray(Y,n,p);
  freeArray(Z,m,p);

  return 0;
}
