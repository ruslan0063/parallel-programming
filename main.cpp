#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
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

        if (!file.is_open()) {
            cerr << "[ERROR] Не удалось открыть файл: " << path << endl;
            return false;
        }

        file >> dim;

        if (dim == 0) {
            cerr << "[ERROR] Неверный размер матрицы" << endl;
            return false;
        }

        values.assign(dim, vector<double>(dim, 0.0));

        for (size_t i = 0; i < dim; ++i) {
            for (size_t j = 0; j < dim; ++j) {
                if (!(file >> values[i][j])) {
                    cerr << "[ERROR] Недостаточно данных в файле" << endl;
                    return false;
                }
            }
        }

        return true;
    }

    bool writeMatrix(const string& path) const {
        ofstream file(path);

        if (!file.is_open()) {
            cerr << "[ERROR] Не удалось создать файл: " << path << endl;
            return false;
        }

        file << dim << "\n";

        for (size_t i = 0; i < dim; ++i) {
            for (size_t j = 0; j < dim; ++j) {
                file << fixed << setprecision(6) << values[i][j];

                if (j < dim - 1)
                    file << " ";
            }

            file << "\n";
        }

        return true;
    }

    SquareMatrix operator*(const SquareMatrix& other) const {
        if (dim != other.dim) {
            cerr << "[ERROR] Размеры матриц не совпадают" << endl;
            return SquareMatrix(0);
        }

        SquareMatrix result(dim);

        #pragma omp parallel for
        for (int i = 0; i < static_cast<int>(dim); ++i) {
            for (size_t k = 0; k < dim; ++k) {
                double aik = values[i][k];

                if (aik != 0.0) {
                    for (size_t j = 0; j < dim; ++j) {
                        result.values[i][j] += aik * other.values[k][j];
                    }
                }
            }
        }

        return result;
    }
};

int main() {
    string pathA;
    string pathB;
    string pathOut;

    cout << "" << endl;
    cout << "Параллельное умножение матриц" << endl;
    cout << "" << endl;

    cout << "\nФайл матрицы A: ";
    cin >> pathA;

    cout << "Файл матрицы B: ";
    cin >> pathB;

    cout << "Файл результата: ";
    cin >> pathOut;

    SquareMatrix A;
    SquareMatrix B;

    auto readStart = chrono::high_resolution_clock::now();

    if (!A.readMatrix(pathA))
        return 1;

    if (!B.readMatrix(pathB))
        return 1;

    auto readEnd = chrono::high_resolution_clock::now();

    if (A.getDim() != B.getDim()) {
        cerr << "[ERROR] Размеры матриц различаются" << endl;
        return 1;
    }

    size_t N = A.getDim();

    size_t inputMemory = 2 * N * N * sizeof(double);
    size_t outputMemory = N * N * sizeof(double);
    size_t totalMemory = inputMemory + outputMemory;

    cout << "\n ХАРАКТЕРИСТИКИ " << endl;
    cout << "Размер: " << N << "x" << N << endl;
    cout << "Входные данные: " << inputMemory / 1024.0 << " KB" << endl;
    cout << "Выходные данные: " << outputMemory / 1024.0 << " KB" << endl;
    cout << "Общий объем: " << totalMemory / 1024.0 << " KB" << endl;
    cout << "Потоков OpenMP: " << omp_get_max_threads() << endl;

    auto multStart = chrono::high_resolution_clock::now();

    SquareMatrix C = A * B;

    auto multEnd = chrono::high_resolution_clock::now();

    auto writeStart = chrono::high_resolution_clock::now();

    if (!C.writeMatrix(pathOut))
        return 1;

    auto writeEnd = chrono::high_resolution_clock::now();

    auto readTime = chrono::duration_cast<chrono::milliseconds>(readEnd - readStart);
    auto multTime = chrono::duration_cast<chrono::milliseconds>(multEnd - multStart);
    auto writeTime = chrono::duration_cast<chrono::milliseconds>(writeEnd - writeStart);

    auto totalTime = readTime + multTime + writeTime;

    cout << "\n РЕЗУЛЬТАТЫ " << endl;
    cout << "Чтение: " << readTime.count() << " мс" << endl;
    cout << "Умножение: " << multTime.count() << " мс" << endl;
    cout << "Запись: " << writeTime.count() << " мс" << endl;
    cout << "Всего: " << totalTime.count() << " мс" << endl;

    ofstream results("results.txt", ios::app);

    if (results.is_open()) {
        results
            << N << " "
            << N * N << " "
            << N * N * N << " "
            << readTime.count() << " "
            << multTime.count() << " "
            << writeTime.count() << " "
            << totalTime.count() << "\n";
    }

    return 0;
}