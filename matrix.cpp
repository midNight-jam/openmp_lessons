//
// Created by jayam on 5/10/17.
//

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void createData(int const num_rows, int num_cols,
				float * const a, float * const b,
				float * const c, float * const ref);


void checkSolution(int const num_rows, int num_cols,
				float * const a, float * const b);

void add(int const num_rows, int num_cols,
				float * const a, float * const b,
				float * const c);


int main() {
  float *a, *b, *c, *sol;
  int const NUM_ROWS = 2048;
  int const NUM_COLS = 2048;

  size_t const N_BYTES = NUM_ROWS * NUM_COLS * sizeof(float);

  a = (float *) malloc(N_BYTES);
  b = (float *) malloc(N_BYTES);
  c = (float *) malloc(N_BYTES);
  sol = (float *) malloc(N_BYTES);

  high_resolution_clock::time_point start = high_resolution_clock::now();
  
  createData(NUM_ROWS, NUM_COLS, a, b, c, sol);
  
  add(NUM_ROWS, NUM_COLS, a, b, c);

  checkSolution(NUM_ROWS, NUM_COLS, c, sol);


  high_resolution_clock::time_point end = high_resolution_clock::now(); 

  auto exec_time = duration_cast<microseconds>(end - start).count();

  cout << endl << " Exec microseconds : " << exec_time << endl;
  
  free(a);
  free(b);
  free(c);
  free(sol);

  cout << "Main Done" << endl;
  return 0;
}

void createData(int const num_rows, int num_cols,
				float * const a, float * const b,
				float * const c, float * const ref) {

  // int row, col;
  srand((unsigned) time(NULL));

  // #pragma omp parallel
  {
  	// #pragma omp for
    for (int row = 0; row < num_rows; row++) {
      for (int col = 0; col < num_cols; col++) {
        int i = col + row * num_cols;
        a[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        b[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        c[i] = 0.0f;
        ref[i] = a[i] + b[i];
      }
    }
  }

  cout << "Created Data ... " << endl;
}


void checkSolution(int const num_rows, int num_cols,
				float * const a, float * const b) {

  int const N = num_rows * num_cols;
  int different = 0;

  // #pragma omp parallel
  {
	// #pragma omp for
    for (int i = 0; i < N; i++) {
	    different = (a[i] != b[i]);
	    if (different) break;
	  }
  }
  if (different)
    printf("Arrays Do not match FAILURE!!! \n");
  else
    printf("Arrays match.. Success !!! \n");
}

void add(int const num_rows, int num_cols,
				float * const a, float * const b,
				float * const c) {
  // #pragma omp parallel
  {		
	  // #pragma omp for
	  for (int col = 0; col < num_cols; col++) {
	    for (int row = 0; row < num_rows; row++) {
	      int i = col + row * num_cols;
	      c[i] = a[i] + b[i];
	    }
	  }
   }
  cout << "Addition Done ..." << endl;
}