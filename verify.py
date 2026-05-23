import numpy as np

def loadMatrix(filename):
    with open(filename, 'r') as f:
        size = int(f.readline())

        matrix_data = []

        for _ in range(size):
            row = list(map(float, f.readline().split()))
            matrix_data.append(row)

    return np.array(matrix_data)

print("")
print("      ВЕРИФИКАЦИЯ РЕЗУЛЬТАТОВ      ")
print("\n")

print("Укажите файлы:")

fileA = input("  Матрица A: ")
fileB = input("  Матрица B: ")
fileC = input("  Результат C++: ")

matrixA = loadMatrix(fileA)
matrixB = loadMatrix(fileB)
matrixResult = loadMatrix(fileC)

expectedResult = np.dot(matrixA, matrixB)

print("\n")

if np.allclose(matrixResult, expectedResult, atol=1e-9):
    print("         ВСЁ ПРАВИЛЬНО!         ")

    max_err = np.max(np.abs(matrixResult - expectedResult))

    print(f"   Макс. ошибка: {max_err:.2e}         ")
else:
    print("         ОШИБКА!                ")

    max_err = np.max(np.abs(matrixResult - expectedResult))

    print(f"   Макс. ошибка: {max_err:.2e}         ")

print("")
