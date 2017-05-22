#include <iostream>
#include <omp.h>
#include <chrono>
#include <random>
#include <list>
#include <iterator>
#include <limits>
#include <math.h>
#include <omp.h>

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
        printf("  %d,%d - %f  ", row, i, a[i]);
     }
     cout << endl;
    }
}


bool clustersMatch(list <int> lol_1 [], list <int> lol_2 [], int size){
	// cout << " size 1 : " << sizeof(lol_1);
	// cout << " size 2 : " << sizeof(lol_2);
	bool match = true;
	cout <<"===========================================" << endl;		

	for(int i = 0; i < size; i++){
		list <int> :: iterator it1 = lol_1[i].begin();
		list <int> :: iterator it2 = lol_2[i].begin();

		for(; it1 != lol_1[i].end() && it2 != lol_2[i].end(); ++it1, ++it2){
				cout << " cluster old : " << *it1  << " cluster : " << *it2 << " K : " << i << endl;
				if(*it1 != *it2){
					match = false;
				}
			}
		if( it1 == lol_1[i].end() && it2 == lol_2[i].end() && match == true){
			cout << "Size & content matches for cluster : " << i << "continuing " <<  endl;  
			continue;
		}		
		else{
			cout << " Doesnt match , cluster match returning false" << endl;
			return false;
		}
	}

	cout <<"===========================================" << endl;		
	return match;
}

float getPairwiseEuclideanDist(float * a, float * b, int size){
	float dist = 0.0;
	for(int i = 0; i < size; i++){
		float sub = abs(a[i] - b[i]);
		dist += pow(sub, 2); 
	}

	dist = sqrt(dist);
	return dist;
}

void printCluster(list <int> lol_1 [], int size){
for(int i = 0; i < size; i++){
		for(list <int> :: iterator it1 = lol_1[i].begin(); it1 != lol_1[i].end(); ++it1){
				cout << "cluster : "<< i <<" val  : " << *it1  << endl;
			}
			cout <<"--------------------" << endl;		
	}
}


void K_Means_Pairwise(float * mat, int num_rows, int num_cols){
	int e = 0;
	list <int>  old_cluster[5];
	bool completeMatch = false;

	int center_indexes [5]; 
	random_device rd;
	mt19937 eng(rd());
	uniform_int_distribution<> distr(0,num_rows);
	// selecting the random vectors as index
	for(int n = 0; n < 5; n++){
		center_indexes[n] = distr(eng); 
	}
	// sleecting 5 indexes as centroid , 5 cols are hard coded will remove
	float centeroids[5][5];
	// fetching the centroids data
	for(int i = 0; i < 5; i++){
		int row_num = center_indexes[i];
		for(int j = 0; j < 5; j++){
			centeroids[i][j] = mat[row_num*5 + j];
		}
	} 

	while(!completeMatch){ // epoch loop
	// while(e < 3){ // epoch loop
		list <int> new_cluster[5];


		// for(int i =0 ; i< 5; i++){
		// 	cout << " cent : ";
		// 	for(int j = 0; j < 5; j++){
		// 		// cout  << " " << centeroids[i][j];
		// 	}
		// 	cout << endl;
		// }
		 
		 
		// for each point, calculate its distance with the centroids & keep track of closest centroid
		float data[5]; // as 5 cols, taking the values out in a seperate array for dist calculation purpose
		float minDist;
		int cluster_no = 0;
		for(int row = 0; row < num_rows; row++){
			 minDist = numeric_limits<float>::max();
			for(int col = 0; col < 5; col++){
				int index  = col + row * 5;
				data [col] = mat[index];
			}
			// got the data, now calculate distance 
			// cout << "\n-------------row : " << row << "-------------" <<endl;
			for(int i = 0; i < 5; i++){
				float dist = getPairwiseEuclideanDist(data, centeroids[i], 5);
				if(dist < minDist){
					minDist = dist;
					cluster_no = i;
					// cout << "\n new min dist : " << dist << " cluster : "<< cluster_no << endl;
				}
			}
			new_cluster[cluster_no].push_back(row);
			// cout << " \n row " << row  << " goes to cluster " << cluster_no << " with min dist " << minDist;
		}
		printCluster(new_cluster, 5);
		
		//readjust the centroids as per the members of the cluster
		cout << "Readjusting ZZZZ.... "  << endl;
		for(int i = 0; i < 5; i++){
			float new_centroid[5] = {};
			int cluster_size = 0;
			for(list <int> :: iterator it1 = new_cluster[i].begin(); it1 != new_cluster[i].end(); ++it1){
				// cout << "cluster : "<< i <<" val  : " << *it1  << endl;
				++cluster_size;
			      
			      for (int col = 0; col < num_cols; col++) {
			        int i = col + *it1 * num_cols;
			        // printf("  %d,%d - %f  ", *it1, i, mat[i]);
			        new_centroid[col] += mat[i]; // adding all the features for all the vector in the cluster 
			      }
			     // cout << endl;
				}
			// cout << "Cluster size : " << cluster_size << endl;
			// cout << "Beofre avging" << endl;
			// for(int i = 0; i<num_cols; i++){
				// cout << " " << new_centroid[i];
			// }
			// cout << endl;
			// cout << "after  avging" << endl;
			// avergaing each feture for the new centroid
			if(cluster_size > 0){
				for(int i = 0; i<num_cols; i++){
					new_centroid[i] = new_centroid[i] / cluster_size;
					// cout << " " << new_centroid[i];
				}
			}


			// cout << "\nB4 Readjusting " << endl;
			// for(int j = 0; j < num_cols; j++){
			// 	cout << " " << centeroids[i][j];
			// }
			// cout << endl;


			// cout << "After  Readjusting " << endl;
			for(int j = 0; j < num_cols; j++){
				centeroids[i][j] = new_centroid[j];
				// cout << " " << centeroids[i][j];
			}
			
			// cout <<"\n--------------------" << endl;		
		}

		e++;
		
		// printCluster(old_cluster, 5);
		if(e != 0){
			completeMatch = clustersMatch(old_cluster, new_cluster, 5);	
		}
		
		cout << "completeMatch : " << completeMatch << endl;
		
		// old_cluster = new_cluster;
		for(int i = 0; i < 5; i++){
			old_cluster[i] = new_cluster[i];
		}
		// printCluster(old_cluster, 5);

		cout << "Epoch " << e << " ends "  << endl;
	}

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
	
	for(int i=0; i<4; i++){
		listOfList2[0].push_back(i);
		listOfList2[0].push_front(i * 3);
	}
	list <int> :: iterator it;



	for(it = listOfList[0].begin(); it != listOfList[0].end(); ++it){
		cout << " val : " << *it << endl;
	}

	bool res = clustersMatch(listOfList, listOfList2, 1);

	cout << "Amtch results : " <<res << endl; 

	bool dinal  = listOfList == listOfList2;
	cout << "Fianl results : " <<dinal << endl; 

}


int main(int argc, char ** argv) {
  cout<<"Ready... for KNN Serial"<<endl;
  
  float *a;
  int const NUM_ROWS = 100;
  int const NUM_COLS = 5;
  int k = 5;
  size_t const N_BYTES = NUM_ROWS * NUM_COLS * sizeof(float);
  a = (float *) malloc(N_BYTES);
  
  double Tstart = omp_get_wtime();
  
  createData(NUM_ROWS, NUM_COLS, a);
  printMatrix(NUM_ROWS, NUM_COLS, a);
  K_Means_Pairwise(a, NUM_ROWS, NUM_COLS);
  
  double Tend = omp_get_wtime();

  free(a);


  printf(" Time = %f secs \n", Tend - Tstart);			
  return 0;
}