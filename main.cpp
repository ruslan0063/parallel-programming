#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <omp.h>

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
        if (!file.is_open()) return false;

        file >> dim;
        values.assign(dim, vector<double>(dim, 0.0));

        for (size_t i = 0; i < dim; ++i)
            for (size_t j = 0; j < dim; ++j)
                file >> values[i][j];

        return true;
    }

    SquareMatrix multiplyParallel(const SquareMatrix& other, int threads) const {
        SquareMatrix result(dim);

        omp_set_num_threads(threads);

        #pragma omp parallel for collapse(2)
        for (int i = 0; i < (int)dim; ++i) {
            for (int j = 0; j < (int)dim; ++j) {
                double sum = 0.0;
                for (int k = 0; k < (int)dim; ++k) {
                    sum += values[i][k] * other.values[k][j];
                }
                result.values[i][j] = sum;
            }
        }

        return result;
    }
};

int main() {
    using namespace std;

    int sizes[] = {200, 400, 800, 1200, 1600, 2000};
    int threads_list[] = {1, 2, 4, 8};

    ofstream out("results.csv");
    out << "size,threads,time_ms\n";

    for (int s = 0; s < 6; s++) {

        int N = sizes[s];

        string fileA = "A" + to_string(N) + ".txt";
        string fileB = "B" + to_string(N) + ".txt";

        SquareMatrix A, B;

        if (!A.readMatrix(fileA) || !B.readMatrix(fileB)) {
            cout << "Ошибка файлов: " << N << endl;
            continue;
        }

        if (A.getDim() != B.getDim()) continue;

        for (int t : threads_list) {

            auto start = chrono::high_resolution_clock::now();

            SquareMatrix C = A.multiplyParallel(B, t);

            auto end = chrono::high_resolution_clock::now();

            auto time = chrono::duration_cast<chrono::milliseconds>(end - start);

            cout << "N=" << N
                 << " T=" << t
                 << " time=" << time.count() << " ms\n";

            out << N << "," << t << "," << time.count() << "\n";
        }
    }

    cout << "Готово: results.csv\n";
    return 0;
}