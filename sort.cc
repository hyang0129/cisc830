#include<stdio.h>
#include<string.h>
#include<algorithm>
#define THREAD_MAX 4
#define MAX 10000000
#include <time.h>
#include <pthread.h>
#include <iostream>
using namespace std;


int X[MAX];



int part = 0;
 
// merge function for merging two parts
void merge(int low, int mid, int high)
{
    int* left = new int[mid - low + 1];
    int* right = new int[high - mid];
 
    // n1 is size of left part and n2 is size
    // of right part
    int n1 = mid - low + 1, n2 = high - mid, i, j;
 
    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = X[i + low];
 
    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = X[i + mid + 1];
 
    int k = low;
    i = j = 0;
 
    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            X[k++] = left[i++];
        else
            X[k++] = right[j++];
    }
 
    // insert remaining values from left
    while (i < n1) {
        X[k++] = left[i++];
    }
 
    // insert remaining values from right
    while (j < n2) {
        X[k++] = right[j++];
    }
}
 
// merge sort function
void merge_sort(int low, int high)
{
    // calculating mid point of array
    int mid = low + (high - low) / 2;
    if (low < high) {
 
        // calling first half
        merge_sort(low, mid);
 
        // calling second half
        merge_sort(mid + 1, high);
 
        // merging the two halves
        merge(low, mid, high);
    }
}
 
// thread function for multi-threading
void* merge_sort(void* arg)
{
    // which part out of 4 parts
    int thread_part = part++;
 
    // calculating low and high
    int low = thread_part * (MAX / 4);
    int high = (thread_part + 1) * (MAX / 4) - 1;
 
    // evaluating mid point
    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }
}




int main(int argc,char** argv){
	int N,K,A,B,C,M;
	clock_t t1, t2;
	t1 = clock();
	
	FILE* fin = fopen(argv[1],"r");
	fscanf(fin,"%d%d%d%d%d%d",&N,&K,&A,&B,&C,&M);
	for(int i = 0;i < K;++i)
		fscanf(fin,"%d",&X[i]);
	fclose(fin);

	for(int i = K;i < N;++i)
		X[i] = ((long long)A * X[i - 1] + (long long)B * X[i - 2] + C) % M;

	
	// sorting 
	
	pthread_t threads[THREAD_MAX];
	
    	for (int i = 0; i < THREAD_MAX; i++)
		pthread_create(&threads[i], NULL, merge_sort,
		                                (void*)NULL);


        for (int i = 0; i < 4; i++)
		pthread_join(threads[i], NULL);	
	

	merge(0, (MAX / 2 - 1) / 2, MAX / 2 - 1);
	merge(MAX / 2, MAX/2 + (MAX-1-MAX/2)/2, MAX - 1);
	merge(0, (MAX - 1)/2, MAX - 1);

	// std::sort(X,X + N);
	
    
	FILE* fout = fopen(argv[2],"w");
	
	
	for(int i = MAX - N;i < MAX;++i)
		fprintf(fout,"%d\n",X[i]);
	fclose(fout);
	
	t2 = clock();
	
        cout << "Time taken: " << (t2 - t1) /
              (double)CLOCKS_PER_SEC << endl;
              
	return 0;
}
