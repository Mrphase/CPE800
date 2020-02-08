
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../CPE800_CUDA_APSP/apsp_misc.h"
#include "../CPE800_CUDA_APSP/apsp_parallel_1.h"
#include "../CPE800_CUDA_APSP/apsp_misc.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
using namespace std::chrono;
using namespace std;
#define NN  999
const int smp_executions = 8192;
const int threads_per_block = 128;
const int threads_per_smp = 2048;

// derived 
const int blocks_per_smp = threads_per_smp / threads_per_block;
const dim3 blocks(smp_executions, blocks_per_smp);
const dim3 threads(threads_per_block);

__global__ void apsp_parallel_1_kernel(float* dev_dist, int N, int k) {

	int tid = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;
	int i, j;
	float dist1, dist2, dist3;

	if (tid < N * N) {

		i = tid / N;
		j = tid - i * N;

		dist1 = dev_dist[tid];
		dist2 = dev_dist[i * N + k];
		dist3 = dev_dist[k * N + j];

		if (dist1 > dist2 + dist3)
			dev_dist[tid] = dist2 + dist3;
	}
}

// CUDA of Floyd Warshall algorithm
void apsp_parallel_1(float** graph, float** dist, int N) {


	float* dev_dist;
	cudaMalloc((void**)&dev_dist, N * N * sizeof(float));

	
	for (int i = 0; i < N; i++)
		cudaMemcpy(dev_dist + i * N, graph[i], N * sizeof(float),
			cudaMemcpyHostToDevice);

	
	for (int k = 0; k < N; k++) {

		// launch kernel
		apsp_parallel_1_kernel << <blocks, threads >> > (dev_dist, N, k);
		
	}

	// return results to dist matrix on host
	for (int i = 0; i < N; i++)
		cudaMemcpy(dist[i], dev_dist + i * N, N * sizeof(float),
			cudaMemcpyDeviceToHost);
	cudaFree((void**)&dev_dist);
}


int main()
{
	int row = 1024* 2, col = 1024*2; // col and row
	int INF = 9999;

	cout << "Normal 65!!! ";
	typedef struct
	{
		//结构体
		int row, col;
		//二维指针，目的是动态分配内存
		float** matrix;
	} Matrix;
	cout << "Normal 73!!! ";
	typedef struct
	{
		int row, col;
		string** matrix;
	} Matrix_string;

	Matrix m;         //store value of path
	Matrix_string m2; //store path
	std::cout << "Normal 91!!!put in to memory "
		<< "\n";

	float** enterMatrix;
	string** enterMatrix2;
	enterMatrix = (float**)malloc(row * sizeof(float*));   // value of path
	enterMatrix2 = (string**)malloc(row * sizeof(float*)); //store path
	std::cout << "Normal 103!!! "
		<< "\n";
	for (int i = 0; i < row; i++) //put in to memory  //change size to *10
	{
		enterMatrix[i] = (float*)malloc(col * 10 * sizeof(float));
		enterMatrix2[i] = (string*)malloc(col * 10 * sizeof(string)); // change sizeof(float) to string
	}
	std::cout << "Normal 109!!! set default value"
		<< "\n";
	int count_of_nuZero = 0;
	for (int i = 0; i < row; i++) //set default value
	{
		for (int j = 0; j < col; j++)
		{
			enterMatrix[i][j] = INF;  //For path value ,default is 99999
			//enterMatrix2[i][j] = " "; //for path, default is " "
			if (i == j )
			{
				enterMatrix[i][j] = 0;
			}
			if (rand() % (NN + 1) / (float)(NN + 1)>0.9)
			{
				enterMatrix[i][j] = 1;
				count_of_nuZero++;
			}

		}
		
	}
	std::cout << "Normal 128!!! GPU start"
		<< "\n";
	//print_array(arr, n);
	auto start = high_resolution_clock::now();	
	apsp_parallel_1(enterMatrix, enterMatrix, row);
	auto stop = high_resolution_clock::now();
	//print_array(arr, n);
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "apsp_parallel_1  1024     " << duration.count() << " ms   " << "\n";


	std::cout << "Normal 140!!! Please check your algrothm using below demo"
		<< "\n";

	float** M;
	M = (float**)malloc(100 * sizeof(float*));
	for (int i = 0; i < 100; i++) //put in to memory  //change size to *10
	{
		M[i] = (float*)malloc(col * 10 * sizeof(float));
	}
	count_of_nuZero = 0;
	for (int i = 0; i < 10; i++) //set default value
	{
		for (int j = 0; j < 10; j++)
		{
			M[i][j] = 99;  //For path value ,default is 99999
			//enterMatrix2[i][j] = " "; //for path, default is " "
			if (rand() % 100>70)
			{
				M[i][j] = 1;
				count_of_nuZero++;
			}
			if (i==j)
			{
				M[i][j] = 0;
			}
			std::cout << M[i][j] << " ";

		}
		std::cout 
		<< "\n";
	}
	std::cout << count_of_nuZero
		<< "\n";

	std::cout<< "M complete" << "\n";
	float** M2 = M;

	apsp_parallel_1(M, M2, 10);

	auto start2 = high_resolution_clock::now();
	apsp_parallel_1(M, M2, 10);
	auto stop2 = high_resolution_clock::now();
	auto duration2 = duration_cast<microseconds>(stop2 - start2);
	cout << "apsp_parallel_1  10     " << duration2.count() << " ms   "<<"\n";


	for (int i = 0; i < 10; i++) //set default value
	{
		for (int j = 0; j < 10; j++)
		{
			std::cout << M[i][j]<<" ";
		}
		std::cout
			<< "\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"
		<< "\n";
	std::cout << "Normal 200!!! start to test running time from 32 to 65536"
		<< "\n";
	ofstream fout("E:\\CPE800_CUDA_APSP\\mytest.txt");
	for (int ii = 32; ii < 32769; ii*=2)
	{

		int row = ii, col = ii; // col and row
		int INF = 9999;

		cout << "Normal 65!!! ";
		typedef struct
		{
			//结构体
			int row, col;
			//二维指针，目的是动态分配内存
			float** matrix;
		} Matrix;
		cout << "Normal 73!!! ";
		typedef struct
		{
			int row, col;
			string** matrix;
		} Matrix_string;

		Matrix m;         //store value of path
		Matrix_string m2; //store path
		std::cout << "Normal 91!!!put in to memory "
			<< "\n";

		float** enterMatrix;
		string** enterMatrix2;
		enterMatrix = (float**)malloc(row * sizeof(float*));   // value of path
		enterMatrix2 = (string**)malloc(row * sizeof(float*)); //store path
		std::cout << "Normal 103!!! "
			<< "\n";
		for (int i = 0; i < row; i++) //put in to memory  //change size to *10
		{
			enterMatrix[i] = (float*)malloc(col * 10 * sizeof(float));
			enterMatrix2[i] = (string*)malloc(col * 10 * sizeof(string)); // change sizeof(float) to string
		}
		std::cout << "Normal 109!!! set default value"
			<< "\n";
		int count_of_nuZero = 0;
		for (int i = 0; i < row; i++) //set default value
		{
			for (int j = 0; j < col; j++)
			{
				enterMatrix[i][j] = INF;  //For path value ,default is 99999
				//enterMatrix2[i][j] = " "; //for path, default is " "
				if (i == j)
				{
					enterMatrix[i][j] = 0;
				}
				if (rand() % (NN + 1) / (float)(NN + 1) > 0.95)
				{
					enterMatrix[i][j] = 1;
					count_of_nuZero++;
				}

			}

		}
		std::cout << "Normal 128!!! GPU start"
			<< "\n";
		//print_array(arr, n);
		auto start = high_resolution_clock::now();
		apsp_parallel_1(enterMatrix, enterMatrix, row);
		auto stop = high_resolution_clock::now();
		//print_array(arr, n);
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "apsp_parallel_1  edge：    "<< count_of_nuZero << "  node:  "<<ii<<"  " << duration.count() << " ms   " << "\n";
		
		fout << count_of_nuZero << "   "<< duration.count() << " ms   " << "\n";
		//fout.close();
	}

	return 0;
}
