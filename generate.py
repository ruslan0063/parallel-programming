import numpy as np

dimension = int(input("Введите размер матрицы (будет создано две): "))

matrixA = np.random.randint(0, 10, (dimension, dimension))
matrixB = np.random.randint(0, 10, (dimension, dimension))

with open(f"A{dimension}.txt", 'w') as file_a:
    file_a.write(f"{dimension}\n")

    for row in matrixA:
        file_a.write(' '.join(map(str, row)) + '\n')

with open(f"B{dimension}.txt", 'w') as file_b:
    file_b.write(f"{dimension}\n")

    for row in matrixB:
        file_b.write(' '.join(map(str, row)) + '\n')

print(f"Созданы файлы A{dimension}.txt и B{dimension}.txt")