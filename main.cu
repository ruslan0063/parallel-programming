#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <cuda_runtime.h>
#include <iomanip>

using namespace std;

__global__ void multiplyKernel(
    double* A,
    double* B,
    double* C,
    int N
) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {

        double sum = 0.0;

        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }

        C[row * N + col] = sum;
    }
}

class Matrix {
private:

    vector<vector<double>> data;
    size_t size;

public:

    Matrix(size_t n = 0)
        : size(n), data(n, vector<double>(n, 0.0)) {}

    bool read(const string& filename) {

        ifstream file(filename);

        if (!file.is_open())
            return false;

        file >> size;

        data.assign(size, vector<double>(size, 0.0));

        for (size_t i = 0; i < size; ++i)
            for (size_t j = 0; j < size; ++j)
                file >> data[i][j];

        return true;
    }

    bool write(const string& filename) const {

        ofstream file(filename);

        if (!file.is_open())
            return false;

        file << size << "\n";

        for (size_t i = 0; i < size; ++i) {

            for (size_t j = 0; j < size; ++j) {

                file << fixed << setprecision(6)
                     << data[i][j];

                if (j < size - 1)
                    file << " ";
            }

            file << "\n";
        }

        return true;
    }

    Matrix multiplyCPU(const Matrix& other) const {

        Matrix result(size);

        for (size_t i = 0; i < size; ++i)
            for (size_t k = 0; k < size; ++k)
                for (size_t j = 0; j < size; ++j)
                    result.data[i][j] +=
                        data[i][k] * other.data[k][j];

        return result;
    }

    Matrix multiplyCUDA(
        const Matrix& other,
        int blockSize
    ) const {

        Matrix result(size);

        int N = (int)size;

        size_t bytes = N * N * sizeof(double);

        vector<double> flatA(N * N);
        vector<double> flatB(N * N);
        vector<double> flatC(N * N);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {

                flatA[i * N + j] = data[i][j];
                flatB[i * N + j] = other.data[i][j];
            }

        double* d_A;
        double* d_B;
        double* d_C;

        cudaMalloc(&d_A, bytes);
        cudaMalloc(&d_B, bytes);
        cudaMalloc(&d_C, bytes);

        cudaMemcpy(
            d_A,
            flatA.data(),
            bytes,
            cudaMemcpyHostToDevice
        );

        cudaMemcpy(
            d_B,
            flatB.data(),
            bytes,
            cudaMemcpyHostToDevice
        );

        dim3 threads(blockSize, blockSize);

        dim3 blocks(
            (N + blockSize - 1) / blockSize,
            (N + blockSize - 1) / blockSize
        );

        multiplyKernel<<<blocks, threads>>>(
            d_A,
            d_B,
            d_C,
            N
        );

        cudaDeviceSynchronize();

        cudaMemcpy(
            flatC.data(),
            d_C,
            bytes,
            cudaMemcpyDeviceToHost
        );

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                result.data[i][j] = flatC[i * N + j];

        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);

        return result;
    }

    bool compare(const Matrix& other) const {

        if (size != other.size)
            return false;

        for (size_t i = 0; i < size; ++i)
            for (size_t j = 0; j < size; ++j)
                if (abs(data[i][j] - other.data[i][j]) > 1e-6)
                    return false;

        return true;
    }

    size_t getSize() const {
        return size;
    }
};

int main(int argc, char** argv) {

    string fileA = "A200.txt";
    string fileB = "B200.txt";

    if (argc >= 3) {

        fileA = argv[1];
        fileB = argv[2];
    }

    Matrix A;
    Matrix B;

    if (!A.read(fileA) || !B.read(fileB)) {

        cout << "Ошибка чтения файлов\n";
        return 1;
    }

    if (A.getSize() != B.getSize()) {

        cout << "Размеры матриц не совпадают\n";
        return 1;
    }

    int N = (int)A.getSize();

    cout << "Размер матрицы: "
         << N
         << "x"
         << N
         << "\n";

    auto cpuStart =
        chrono::high_resolution_clock::now();

    Matrix cpuResult = A.multiplyCPU(B);

    auto cpuEnd =
        chrono::high_resolution_clock::now();

    double cpuTime =
        chrono::duration<double, milli>(
            cpuEnd - cpuStart
        ).count();

    cout << "CPU: "
         << cpuTime
         << " ms\n";

    int blocks[] = {4, 8, 16, 32};

    ofstream csv("results.csv", ios::app);

    for (int bs : blocks) {

        auto gpuStart =
            chrono::high_resolution_clock::now();

        Matrix gpuResult =
            A.multiplyCUDA(B, bs);

        auto gpuEnd =
            chrono::high_resolution_clock::now();

        double gpuTime =
            chrono::duration<double, milli>(
                gpuEnd - gpuStart
            ).count();

        cout << "Block "
             << bs
             << "x"
             << bs
             << ": "
             << gpuTime
             << " ms";

        if (gpuResult.compare(cpuResult))
            cout << " OK";

        cout << "\n";

        csv << N
            << ","
            << bs << "x" << bs
            << ","
            << cpuTime
            << ","
            << gpuTime
            << "\n";
    }

    csv.close();

    cpuResult.write("result.txt");

    return 0;
}