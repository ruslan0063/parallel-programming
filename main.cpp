#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <mpi.h>

using namespace std;

class SquareMatrix {
private:
    vector<vector<double>> values;
    size_t dim;

public:
    SquareMatrix(size_t size = 0)
        : dim(size), values(size, vector<double>(size, 0.0)) {}

    size_t getDim() const {
        return dim;
    }

    bool readMatrix(const string& path) {

        ifstream file(path);

        if (!file.is_open())
            return false;

        file >> dim;

        values.assign(dim, vector<double>(dim, 0.0));

        for (size_t i = 0; i < dim; ++i)
            for (size_t j = 0; j < dim; ++j)
                file >> values[i][j];

        return true;
    }

    bool writeMatrix(const string& path) const {

        ofstream file(path);

        if (!file.is_open())
            return false;

        file << dim << "\n";

        for (size_t i = 0; i < dim; ++i) {

            for (size_t j = 0; j < dim; ++j) {

                file << values[i][j];

                if (j < dim - 1)
                    file << " ";
            }

            file << "\n";
        }

        return true;
    }

    double get(size_t i, size_t j) const {
        return values[i][j];
    }

    void set(size_t i, size_t j, double value) {
        values[i][j] = value;
    }
};

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank;
    int processes;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    if (rank == 0) {

        cout << "Лабораторная работа №5\n";
        cout << "MPI на суперкомпьютере\n\n";
    }

    string fileA = "A200.txt";
    string fileB = "B200.txt";

    if (argc >= 3) {

        fileA = argv[1];
        fileB = argv[2];
    }

    SquareMatrix A;
    SquareMatrix B;

    if (rank == 0) {

        if (!A.readMatrix(fileA) || !B.readMatrix(fileB)) {

            cout << "Ошибка чтения файлов\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int N;

    if (rank == 0)
        N = (int)A.getDim();

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {

        A = SquareMatrix(N);
        B = SquareMatrix(N);
    }

    vector<double> flatA(N * N);
    vector<double> flatB(N * N);

    if (rank == 0) {

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {

                flatA[i * N + j] = A.get(i, j);
                flatB[i * N + j] = B.get(i, j);
            }
    }

    MPI_Bcast(flatA.data(), N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(flatB.data(), N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank != 0) {

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {

                A.set(i, j, flatA[i * N + j]);
                B.set(i, j, flatB[i * N + j]);
            }
    }

    SquareMatrix C(N);

    int rows = N / processes;

    int start = rank * rows;

    int end = (rank == processes - 1)
        ? N
        : start + rows;

    vector<double> local((end - start) * N, 0.0);

    MPI_Barrier(MPI_COMM_WORLD);

    double startTime = MPI_Wtime();

    for (int i = start; i < end; ++i) {

        for (int k = 0; k < N; ++k) {

            double value = A.get(i, k);

            for (int j = 0; j < N; ++j) {

                local[(i - start) * N + j] +=
                    value * B.get(k, j);
            }
        }
    }

    if (rank == 0) {

        for (int i = start; i < end; ++i)
            for (int j = 0; j < N; ++j)
                C.set(i, j, local[(i - start) * N + j]);

        for (int p = 1; p < processes; ++p) {

            int p_start = p * rows;

            int p_end = (p == processes - 1)
                ? N
                : p_start + rows;

            vector<double> temp((p_end - p_start) * N);

            MPI_Recv(
                temp.data(),
                (p_end - p_start) * N,
                MPI_DOUBLE,
                p,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            for (int i = p_start; i < p_end; ++i)
                for (int j = 0; j < N; ++j)
                    C.set(i, j, temp[(i - p_start) * N + j]);
        }

    } else {

        MPI_Send(
            local.data(),
            (end - start) * N,
            MPI_DOUBLE,
            0,
            0,
            MPI_COMM_WORLD
        );
    }

    double endTime = MPI_Wtime();

    if (rank == 0) {

        double time_ms =
            (endTime - startTime) * 1000.0;

        cout << "Количество процессов: "
             << processes
             << endl;

        cout << "Время выполнения: "
             << time_ms
             << " мс\n";

        C.writeMatrix("result.txt");

        ofstream out(
            "supercomputer_res.csv",
            ios::app
        );

        out << N
            << ","
            << processes
            << ","
            << time_ms
            << "\n";

        out.close();
    }

    MPI_Finalize();

    return 0;
}