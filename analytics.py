import matplotlib.pyplot as plt

sizes = []
times = []

with open('results.txt', 'r') as f:
    lines = f.readlines()

    for line in lines:
        if line.strip():
            parts = line.split()

            if len(parts) >= 7:
                sizes.append(int(parts[0]))
                times.append(int(parts[6]))

plt.figure(figsize=(10, 6))

plt.plot(sizes, times, marker='o', linewidth=2)

plt.xlabel('Размер матрицы')
plt.ylabel('Общее время (мс)')
plt.title('Зависимость времени выполнения от размера матрицы')

plt.grid(True)

plt.tight_layout()

plt.savefig('performance_graph.png', dpi=300)

plt.show()

print("График сохранён в performance_graph.png")