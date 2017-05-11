#include <iostream>
#include <omp.h>
#include <stdio.h>

using namespace std;

int main(int argc, char ** argv){

	int numT = omp_get_num_threads();
	int numP = omp_get_num_procs();
	int maxT = omp_get_max_threads();

	printf(" No threads : %d \n", numT);
	printf(" No Max threads : %d \n", maxT);
	printf(" No Process : %d \n", numP);
	
	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
     	printf("Hello (%d)\n",ID);
	}
	return 0;
}