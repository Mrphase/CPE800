
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <algorithm>
//#include "graph.h"
#include <string>
#include <iostream>
#include <chrono>
#include <string>
#include <list>

#include <stdio.h>
#include <device_launch_parameters.h> //.显示blockDim等变量出现了未定义的错误：
#include <cuda.h>
#include <assert.h>
#include <./inc/helper_cuda.h> //C:\ProgramData\NVIDIA Corporation\CUDA Samples\v10.1\common\inc\helper_cuda.h
//#include<C:\ProgramData\NVIDIA Corporation\CUDA Samples\v10.1\common\inc\helper_string.h>

#include <cusparse.h>

//#include "../common/common.h"

#include <stdio.h>

#include <stdlib.h>

#include <cusparse_v2.h>

#include <cuda.h>


#define blockMatrixSize	
// #include <ext/hash_map> //gcc ?
// #include <hash_map>
#include <unordered_map> 

using namespace std::chrono;
using namespace std;
//using namespace __gnu_cxx;
#include <stdio.h>

cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);

__global__ void addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

void initialize(float* cooValHostPtr, int* cooColIndexHostPtr, float* yHostPtr, int* csrRowPtr)
{
	cooValHostPtr[0] = 1.0;
	cooValHostPtr[1] = 2.0;
	cooValHostPtr[2] = 3.0;
	cooValHostPtr[3] = 4.0;
	cooValHostPtr[4] = 5.0;
	cooValHostPtr[5] = 6.0;
	cooValHostPtr[6] = 7.0;
	cooValHostPtr[7] = 8.0;
	cooValHostPtr[8] = 9.0;

	cooValHostPtr[9] = 10.0;

	cooColIndexHostPtr[0] = 0;
	cooColIndexHostPtr[1] = 2;
	cooColIndexHostPtr[2] = 3;
	cooColIndexHostPtr[3] = 1;
	cooColIndexHostPtr[4] = 0;
	cooColIndexHostPtr[5] = 2;
	cooColIndexHostPtr[6] = 3;
	cooColIndexHostPtr[7] = 1;
	cooColIndexHostPtr[8] = 3;

	cooColIndexHostPtr[9] = 0;

	yHostPtr[0] = 10.0;
	yHostPtr[1] = 20.0;
	yHostPtr[2] = 30.0;
	yHostPtr[3] = 40.0;
	yHostPtr[4] = 50.0;
	yHostPtr[5] = 60.0;
	yHostPtr[6] = 70.0;
	yHostPtr[7] = 80.0;

	csrRowPtr[0] = 0;
	csrRowPtr[1] = 3;
	csrRowPtr[2] = 4;
	csrRowPtr[3] = 7;
	csrRowPtr[4] = 9;

	csrRowPtr[5] = 10;

}

void cuda_sparse()
{
	int m = 5, n = 4, nnz = 10;
	float* cooValHostPtr = new float[nnz];
	float* zHostPtr = new float[2 * (m)];

	int* cooColIndexHostPtr = new int[nnz];
	int* csrRowPtr = new int[m + 1];

	int* crsRow, * cooCol;

	float alpha = 1;
	float beta = 0;
	float* yHostPtr = new float[2 * n];
	float* y, * cooVal, * z;
	initialize(cooValHostPtr, cooColIndexHostPtr, yHostPtr, csrRowPtr);


	cusparseHandle_t handle;
	cusparseMatDescr_t descr;
	(cusparseCreateMatDescr(&descr));
	cusparseSetMatType(descr, CUSPARSE_MATRIX_TYPE_GENERAL);
	cusparseSetMatIndexBase(descr, CUSPARSE_INDEX_BASE_ZERO);

	(cusparseCreate(&handle));

	(cudaMalloc((void**)&cooVal, nnz * sizeof(float)));
	(cudaMalloc((void**)&y, 2 * n * sizeof(float)));
	(cudaMalloc((void**)&z, 2 * (m) * sizeof(float)));
	(cudaMalloc((void**)&crsRow, (m + 1) * sizeof(int)));
	(cudaMalloc((void**)&cooCol, nnz * sizeof(int)));

	cudaMemcpy(cooVal, cooValHostPtr, nnz * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(y, yHostPtr, 2 * n * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(crsRow, csrRowPtr, (m + 1) * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cooCol, cooColIndexHostPtr, nnz * sizeof(int), cudaMemcpyHostToDevice);

	(cudaMemset((void*)z, 0, 2 * (m) * sizeof(float)));

	(cusparseScsrmm(handle, CUSPARSE_OPERATION_NON_TRANSPOSE, m, 2, n, nnz, &alpha, descr, cooVal, crsRow, cooCol, y, n, &beta, z, m));

	(cudaMemcpy(zHostPtr, z, 2 * (m) * sizeof(float), cudaMemcpyDeviceToHost));

	//for (int i = 0; i < m; i++)
	//{
	//  //if(i%(2)==0&&i!=0)
	//  //  cout<<endl;
	//  cout<<zHostPtr[i]<<" "<<zHostPtr[i+m]<<endl;
	//}
	for (int i = 0; i < m * 2; i++)
	{
		cout << zHostPtr[i] << " ";
	}
}

int main()
{
	cuda_sparse();
	return 0;
}
//
//
//
//int main()
//{
//	cout << "hds";
//    return 0;
//}
