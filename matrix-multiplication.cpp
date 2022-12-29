#include <iostream>
#include <string.h>
#include <chrono>


using namespace std;
void naiveIterativeMatmul(float* const A,
    float* const B,
    float* const C,
    const int M,
    const int N,
    const int K);

void main()
{
    int M = 1024;
    int N = 1024;
    int K = 1024;
    float* A = new float[M * K];
    float* B = new float[K * N];
    float* C = new float[M * N];
    memset(A, 0, M * K * sizeof(float));
    memset(B, 0, K * N * sizeof(float));
    memset(C, 0, M * N * sizeof(float));

    auto start = std::chrono::high_resolution_clock::now();


    naiveIterativeMatmul(A, B, C, M, N, K);

    auto stop = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;

}

void naiveIterativeMatmul(
    float* const A,
    float* const B,
    float* const C,
    const int M,
    const int N,
    const int K)
{

    for (int m = 0; m < M; m++)
    {
        for (int n = 0; n < N; n++)
        {
            for (int k = 0; k < K; k++)
            {
                C[m * M + n] += A[m * M + k] * B[k * K + n];
            }
        }
    }
}