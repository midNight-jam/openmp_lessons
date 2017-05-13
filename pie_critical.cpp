#include <iostream>
#include <omp.h>
using namespace std;


static long num_steps = 100000;
double step;
#define NUM_THREADS 2
// running parallel with single thread is slower than running serial 

int main(){
	int i, nthreads;
	double  pi;

	step = 1.0 / (double) num_steps;
	printf("Num of requested threads = %d \n", NUM_THREADS);	
	double Tstart = omp_get_wtime();

	omp_set_num_threads(NUM_THREADS);
	
	#pragma omp parallel
	{
		int i, id, nthrds;
		double x, sum = 0.0;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if(id == 0){
			nthreads = nthrds; // as onlymain thread is goig to get the no of allocated threads
			printf("Num of allocated threads = %d \n", nthreads);	
		}
		


		for(i = id; i < num_steps; i = i + nthreads){
			x = (i + 0.5) * step;
			sum += 4.0/(1.0 + x*x); // update each threads local sum
		}

		#pragma omp critical
		pi += sum; // using the threads sum, beacuse once we move outside parallel threads local var are gone 
	}

	pi = pi * step;

	double Tend = omp_get_wtime();
	double time = Tend - Tstart;
	
	printf(" pi = %f \n", pi);
	printf(" time = %f secs \n", time);

	return 0;
}
