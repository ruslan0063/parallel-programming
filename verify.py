import numpy as np

def load_matrix(file):

    with open(file) as f:

        n = int(f.readline())

        matrix = []

        for _ in range(n):
            matrix.append(
                list(map(float, f.readline().split()))
            )

    return np.array(matrix)

A = load_matrix(input("Файл A: "))
B = load_matrix(input("Файл B: "))
C = load_matrix(input("Файл результата: "))

result = np.dot(A, B)

if np.allclose(C, result):

    print("Результат корректный")

else:

    print("Обнаружена ошибка")