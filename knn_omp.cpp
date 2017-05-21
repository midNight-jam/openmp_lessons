#include <iostream>
#include <omp.h>
#include <chrono>
#include <random>
#include <list>
#include <iterator>

using namespace std;
using namespace std::chrono;

// taking 5 clusters

// int * getKRandomCentroidsIndex(int low, int high, int k){
// 	int centroids [5];  
// 	random_device rd;
// 	mt19937 eng(rd());
// 	uniform_int_distribution<> distr(low,high);
// 	for(int n = 0; n < 5; n++){
// 		cout << " no : " << distr(eng) << endl;
// 		centroids[n] = distr(eng); 
// 	}
// 	cout << " clusters : " << k << endl;
// 	return centroids;
// }

void createData(int const num_rows, int num_cols,
				float * const a) {

  srand((unsigned) time(NULL));

  // #pragma omp parallel
  {
  	// #pragma omp for
    for (int row = 0; row < num_rows; row++) {
      for (int col = 0; col < num_cols; col++) {
        int i = col + row * num_cols;
        a[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      }
    }
  }

  cout << "Created Data ... " << endl;
}

void printMatrix(int const num_rows, int num_cols,
				float * const a){
	for (int row = 0; row < num_rows; row++) {
      for (int col = 0; col < num_cols; col++) {
        int i = col + row * num_cols;
        printf("%d,%d - %f\n", row, i, a[i]);
     }
    }
}


void K_Means_Pairwise(float * mat, int num_rows, int num_cols){
	int e = 0;
	while(e < 3){ // epoch loop
		int centroids [5]; 
		random_device rd;
		mt19937 eng(rd());
		uniform_int_distribution<> distr(0,num_rows);
		// selecting the random vectors as index
		for(int n = 0; n < 5; n++){
			centroids[n] = distr(eng); 
			cout << " no : " << centroids[n] << endl;
		}



		e++;
		cout << "Epoch ends" << endl;
	}

}


bool clustersMatch(list <int> lol_1 [], list <int> lol_2 [], int size){
	cout << " size 1 : " << sizeof(lol_1);
	cout << " size 2 : " << sizeof(lol_2);

	for(int i = 0; i < size; i++){
		for(list <int> :: iterator it1 = lol_1[i].begin(), it2 = lol_2[i].begin(); it1 != lol_1[i].end() || it2 != lol_2[i].end(); ++it1, ++it2){
				cout << " val 1 : " << *it1  << " val 2 : " << *it2 << endl;
			}		
	}
	return true;
}

void try_list(){
	list <int> listOfList [1] ;
	
	list <int> mylist;
	for(int i=0; i<5; i++){
		mylist.push_back(i);
		mylist.push_front(i * 3);

		listOfList[0].push_back(i);
		listOfList[0].push_front(i * 3);
	}


	list <int> listOfList2 [1] ;
	
	for(int i=0; i<5; i++){
		listOfList2[0].push_back(i);
		listOfList2[0].push_front(i * 3);
	}
	list <int> :: iterator it;



	for(it = listOfList[0].begin(); it != listOfList[0].end(); ++it){
		cout << " val : " << *it << endl;
	}

	bool res = clustersMatch(listOfList, listOfList2, 1);

	cout << "Amtch results : " <<res << endl; 
}




int main(int argc, char ** argv) {
  cout<<"Ready... for KNN OMP"<<endl;
  
  float *a;
  int const NUM_ROWS = 20;
  int const NUM_COLS = 20;
  int k = 5;
  size_t const N_BYTES = NUM_ROWS * NUM_COLS * sizeof(float);
  a = (float *) malloc(N_BYTES);
  // createData(NUM_ROWS, NUM_COLS, a);
  // printMatrix(NUM_ROWS, NUM_COLS, a);
  // K_Means_Pairwise(a, NUM_ROWS, NUM_COLS);

  try_list();

  free(a);
  return 0;
}