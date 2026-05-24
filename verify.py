import numpy as np

def load_matrix(file):

    with open(file) as f:

        n = int(f.readline())

        matrix = []

        for _ in range(n):
            matrix.append(list(map(float, f.readline().split())))

    return np.array(matrix)

A = load_matrix(input("A: "))
B = load_matrix(input("B: "))
C = load_matrix(input("C (MPI): "))

result = np.dot(A, B)

if np.allclose(C, result):
    print("ВСЁ ПРАВИЛЬНО")
else:
    print("ОШИБКА")