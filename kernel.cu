
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>

#include <mma.h>

using namespace nvcuda;

__global__ void wmma_ker(half* a, half* b, float* c) {
	// Declare the fragments
	wmma::fragment<wmma::matrix_a, 16, 16, 16, half, wmma::col_major> a_frag;
	wmma::fragment<wmma::matrix_b, 16, 16, 16, half, wmma::row_major> b_frag;
	wmma::fragment<wmma::accumulator, 16, 16, 16, float> c_frag;

	// Initialize the output to zero
	wmma::fill_fragment(c_frag, 0.0f);

	// Load the inputs
	wmma::load_matrix_sync(a_frag, a, 16);
	wmma::load_matrix_sync(b_frag, b, 16);

	// Perform the matrix multiplication
	wmma::mma_sync(c_frag, a_frag, b_frag, c_frag);

	// Store the output
	wmma::store_matrix_sync(c, c_frag, 16, wmma::mem_row_major);
}
// Create a cuDNN handle:
checkCudnnErr(cudnnCreate(&handle_));

// Create your tensor descriptors:
checkCudnnErr( cudnnCreateTensorDescriptor( &cudnnIdesc ));
checkCudnnErr( cudnnCreateFilterDescriptor( &cudnnFdesc ));
checkCudnnErr( cudnnCreateTensorDescriptor( &cudnnOdesc ));
checkCudnnErr( cudnnCreateConvolutionDescriptor( &cudnnConvDesc ));

// Set tensor dimensions as multiples of eight (only the input tensor is shown here):
int dimA[] = {1, 8, 32, 32};
int strideA[] = {8192, 1024, 32, 1};

checkCudnnErr( cudnnSetTensorNdDescriptor(cudnnIdesc, getDataType(), 
                                          convDim+2, dimA, strideA) );

// Allocate and initialize tensors (again, only the input tensor is shown):
checkCudaErr( cudaMalloc((void**)&(devPtrI), (insize) * sizeof(devPtrI[0]) ));
hostI = (T_ELEM*)calloc (insize, sizeof(hostI[0]) );

initImage(hostI, insize);

checkCudaErr( cudaMemcpy(devPtrI, hostI, sizeof(hostI[0]) * insize, cudaMemcpyHostToDevice));

// Set the compute data type (below as CUDNN_DATA_FLOAT):
checkCudnnErr( cudnnSetConvolutionNdDescriptor(cudnnConvDesc,
                                               convDim,
                                               padA,
                                               convstrideA,
                                               dilationA,
                                               CUDNN_CONVOLUTION,
                                               CUDNN_DATA_FLOAT) );

// Set the math type to allow cuDNN to use Tensor Cores:
checkCudnnErr( cudnnSetConvolutionMathType(cudnnConvDesc, CUDNN_TENSOR_OP_MATH) );

// Choose a supported algorithm:
cudnnConvolutionFwdAlgo_t algo = CUDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_PRECOMP_GEMM;

// Allocate your workspace:
checkCudnnErr( cudnnGetConvolutionForwardWorkspaceSize(handle_, cudnnIdesc, 
                                                       cudnnFdesc, cudnnConvDesc,
                                                       cudnnOdesc, algo, &workSpaceSize) );

if (workSpaceSize > 0) {
   cudaMalloc(&workSpace, workSpaceSize);
}

// Invoke the convolution:
checkCudnnErr( cudnnConvolutionForward(handle_, (void*)(&alpha), cudnnIdesc, devPtrI,
                                       cudnnFdesc, devPtrF, cudnnConvDesc, algo,
                                       workSpace, workSpaceSize, (void*)(&beta),
                                       cudnnOdesc, devPtrO) );
