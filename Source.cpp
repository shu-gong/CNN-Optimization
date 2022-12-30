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

void iterativeMatmulRegisterSum(
    float* const A,
    float* const B,
    float* const C,
    const int M,
    const int N,
    const int K);

void iterativeMatmulLoopReorder(
    float* const A,
    float* const B,
    float* const C,
    const int M,
    const int N,
    const int K);

void naiveIterativeMatmulTiled(
    float* const A,
    float* const B,
    float* const C);

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

    //naiveIterativeMatmul(A, B, C, M, N, K);
    //iterativeMatmulRegisterSum(A, B, C, M, N, K);
    //iterativeMatmulLoopReorder(A, B, C, M, N, K);
    naiveIterativeMatmulTiled(A, B, C);

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

void naiveIterativeMatmulTiled(
    float* const A,
    float* const B,
    float* const C)
{
    int Ni = 1024;
    int Nj = 1024;
    int Nk = 1024;

    int Ti = 10;
    int Tj = 10;
    int Tk = 10;
    for (int it = 0; it < Ni; it += Ti)
    {
        for (int jt = 0; jt < Nj; jt += Tj)
        {
            for (int kt = 0; kt < Nk; kt += Tk)
            {
                for (int i = 0; i < Ti; i++)
                {
                    for (int j = 0; j < Tj; j++)
                    {
                        for (int k = 0; k < Tk; k++)
                        {
                            C[i * Ni + j] += A[i * Ni + k] * B[k * Nk + j];
                        }
                    }
                }
            }
        }
    }
}

void iterativeMatmulRegisterSum(
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
            float sum = 0.0f;
            for (int k = 0; k < K; k++)
            {
                sum += A[m * M + k] * B[k * K + n];
            }
            C[m * M + n] = sum;
        }
    }
}

void iterativeMatmulLoopReorder(
    float* const A,
    float* const B,
    float* const C,
    const int M,
    const int N,
    const int K)
{
    for (int m = 0; m < M; m++)
    {
        for (int k = 0; k < K; k++)
        {
            for (int n = 0; n < N; n++)
            {
                C[m * M + n] += A[m * M + k] * B[k * K + n];
            }
        }
    }
}

