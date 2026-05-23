import numpy as np

def load_matrix(file):
    with open(file) as f:
        n = int(f.readline())
        data = [list(map(float, f.readline().split())) for _ in range(n)]
    return np.array(data)

A = load_matrix(input("A: "))
B = load_matrix(input("B: "))
C = load_matrix(input("C (C++): "))

expected = np.dot(A, B)

if np.allclose(C, expected, atol=1e-9):
    print("OK — результат верный")
else:
    print("ERROR — есть расхождения")