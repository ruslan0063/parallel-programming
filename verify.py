import numpy as np

def read_matrix(filename):

    with open(filename, "r") as f:

        n = int(f.readline())

        matrix = []

        for _ in range(n):

            row = list(
                map(float, f.readline().split())
            )

            matrix.append(row)

    return np.array(matrix)

fileA = input("Файл A: ")
fileB = input("Файл B: ")
fileC = input("Файл результата: ")

A = read_matrix(fileA)
B = read_matrix(fileB)
C = read_matrix(fileC)

expected = np.dot(A, B)

if np.allclose(C, expected):

    print("Результат правильный")

else:

    print("Ошибка")