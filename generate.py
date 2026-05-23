import numpy as np

sizes = [200, 400, 800, 1200, 1600, 2000]

for n in sizes:
    A = np.random.randint(0, 10, (n, n))
    B = np.random.randint(0, 10, (n, n))

    with open(f"A{n}.txt", "w") as f:
        f.write(str(n) + "\n")
        for row in A:
            f.write(" ".join(map(str, row)) + "\n")

    with open(f"B{n}.txt", "w") as f:
        f.write(str(n) + "\n")
        for row in B:
            f.write(" ".join(map(str, row)) + "\n")

print("Матрицы готовы")