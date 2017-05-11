#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char  ** argv) {
	cout << "Hello U " << endl;
	int a = 99;
	printf("Hello (%d)\n",a);

	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		printf("Thread Num : (%d)\n",ID);
	}
	return 0;
}

