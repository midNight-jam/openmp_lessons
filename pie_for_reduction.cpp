#include <iostream>
#include <omp.h>
using namespace std;

static long num_steps = 100000;
double step;
#define NUM_THREADS 2

int main(int argc, char ** argv){
	printf("Pie Parallel For with reduction\n");
	int i;
	double pi, sum  = 0.0;

	step = 1.0 / (double) num_steps;
	
	double Tstart = omp_get_wtime();

	omp_set_num_threads(NUM_THREADS);
	
	#pragma omp parallel 
	{
		double x;	// each threads local x
		// performing reduction with + on sum
		#pragma omp for reduction(+:sum), schedule(static, 4) 
		for(i = 0; i < num_steps; i++){
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x * x);
		}	
	}
		

	pi = step * sum;

	double Tend = omp_get_wtime();
	
	printf(" pi = %f \n", pi);
	
	printf(" Time = %f \n", Tend - Tstart);
	return 0;
}