#include <stdio.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

__global__ void exhausted_kernel(int Ni,  int Nj, int Nk,  int C, float* results)
{
    // calculate the indices for the current thread
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    int k = blockIdx.z * blockDim.z + threadIdx.z;

    // check if the current combination of Ti, Tj, and Tk is valid
    if (i < Ni && j < Nj && k < Nk && (i * k + j * k + i * j) <= C)
    {
        // calculate the result of the formula for the current combination of Ti, Tj, and Tk
        float result = (float)Nk * Nj * Nj / (1.0f / (float)j + 1.0f / (float)i + 1.0f / (float)k);

        // store the result in the results array
        results[i * Nj * Nk + j * Nk + k] = result;
    }
    else
    {
        // store a zero in the results array for invalid combinations
        results[i * Nj * Nk + j * Nk + k] = 0.0f;
    }
}

int main()
{
    int Ni = 16384;
    int Nj = 16384;
    int Nk = 16384;
    int C = 1000000;

    // define the results vector
    float* results = (float*)calloc(Ni * Nj * Nk, sizeof(float));
    for (int i = 0; i < Ni * Nj * Nk; i++)
    {
        results[i] = 0.0f;
    }

    // allocate memory on the GPU for the results array
    float* dev_results;
    cudaMalloc((void**)&dev_results, Ni * Nj * Nk * sizeof(float));

    int num_threads = 256;  // number of threads per block
    int num_blocks = (Ni * Nj * Nk + num_threads - 1) / num_threads;  // number of blocks needed

    dim3 grid(num_blocks, 1, 1);
    dim3 block(num_threads, 1, 1);

    // transfer the results from the GPU to the host (CPU)
    cudaMemcpy(dev_results, results, Ni * Nj * Nk * sizeof(float), cudaMemcpyHostToDevice);

    // launch the kernel function on the GPU
    exhausted_kernel << <grid, block >> > (Ni, Nj, Nk, C, dev_results);

    // transfer the results from the GPU to the host (CPU)
    cudaMemcpy(results, dev_results, Ni * Nj * Nk * sizeof(float), cudaMemcpyDeviceToHost);


    double objective_value = 0;
    double best_objective_value = DBL_MAX;
    double best_Ti = 0;
    double best_Tj = 0;
    double best_Tk = 0;

    // print the results
    for (int i = 1; i < Ni; i++)
    {
        for (int j = 1; j < Nj; j++)
        {
            for (int k = 1; k < Nk; k++)
            {
                objective_value = results[i * Nj * Nk + j * Nk + k];
                if (objective_value < best_objective_value)
                {
                    best_objective_value = objective_value;
                    best_Ti = i;
                    best_Tj = j;
                    best_Tk = k;
                }
                // std::cout << "Result for Ti = " << i << ", Tj = " << j << ", Tk = " << k << ": " << results[i * Nj * Nk + j * Nk + k];
            }
        }
    }

    cout << "Best values:" << endl;
    cout << "Ti = " << best_Ti << endl;
    cout << "Tj = " << best_Tj << endl;
    cout << "Tk = " << best_Tk << endl;

    free(results);
    // free the GPU memory
    cudaFree(dev_results);

    // free the host (CPU) memory
    return 0;
}