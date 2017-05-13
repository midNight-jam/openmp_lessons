#include <iostream>
#include <omp.h>
using namespace std;


static long num_steps = 100000;
double step;

int main(){
	int i;
	double x, pi, sum  = 0.0;

	step = 1.0 / (double) num_steps;
	
	double Tstart = omp_get_wtime();

	for(i = 0; i < num_steps; i++){
		x = (i + 0.5) * step;
		sum = sum + 4.0 / (1.0 + x * x);
	}	

	pi = step * sum;

	double Tend = omp_get_wtime();
	
	printf(" pi = %f \n", pi);
	
	printf(" Time = %f \n", Tend - Tstart);
	return 0;
}