#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int len = 2000000; // for large values we see the parallel loop taking less time 
// then the serial loop
// Serial loop is faster for small size of data

void parallel_loop(){
	int arr [len];
	
	//start
	high_resolution_clock::time_point start = high_resolution_clock::now();

	#pragma omp parallel for 
	// the array will be divided in to 4 chunks, one for each processor
	for(int i = 0; i < len; i++){
		arr[i] = 2 * i;
		int thread_id = omp_get_thread_num();
		//printf(" thread :%d  arr[%d] = %d \n",thread_id, i, arr[i]);
	}

	high_resolution_clock::time_point end = high_resolution_clock::now();

	auto exec_time = duration_cast<microseconds>(end - start).count();

	cout << endl << " Pararllel microseconds : " << exec_time << endl;

	return;
}


void serial_loop(){
	int arr [len];
	
	//start
	high_resolution_clock::time_point start = high_resolution_clock::now();

	for(int i = 0; i < len; i++){
		arr[i] = 2 * i;
		//printf(" arr[%d] = %d \n", i, arr[i]);
	}

	high_resolution_clock::time_point end = high_resolution_clock::now();
	auto exec_time = duration_cast<microseconds>(end - start).count();
	cout << endl << " Serail microseconds : " << exec_time << endl;
	return;
}



void single_loop(){
	
	//start
	high_resolution_clock::time_point start = high_resolution_clock::now();

	#pragma omp parallel
	{
		#pragma omp single
		{
			int num_T = omp_get_num_threads();
			int num_P = omp_get_num_procs();
			int max_T = omp_get_max_threads();

			cout << "num threads : " << num_T << endl;
			cout << "num Process : " << num_P << endl;
			cout << "max threads : " << max_T << endl;

			for(int i, I  = omp_get_max_threads(); i < I; i++){
				cout << " Hello ( " << i << " ) " << endl;  
			}	
		}
	}
	

	high_resolution_clock::time_point end = high_resolution_clock::now();

	auto exec_time = duration_cast<microseconds>(end - start).count();

	cout << endl << " Single  microseconds : " << exec_time << endl;

	return;

}


int main(int argc, char ** argv) {
  parallel_loop();
  serial_loop();
  single_loop();
  return 0;
}