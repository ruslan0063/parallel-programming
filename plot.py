import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv(
    "results.csv",
    header=None,
    names=["MatrixSize", "Processes", "ParallelTime_ms"]
)

plt.figure(figsize=(10, 6))

for processes in sorted(df["Processes"].unique()):
    subset = df[df["Processes"] == processes]

    plt.plot(
        subset["MatrixSize"],
        subset["ParallelTime_ms"],
        marker="o",
        label=f"{processes} processes"
    )

plt.xlabel("Размер матрицы")
plt.ylabel("Время выполнения (мс)")
plt.title("MPI: зависимость времени выполнения от размера матрицы")

plt.legend()
plt.grid(True)

plt.savefig("mpi_time_vs_size.png", dpi=150)

plt.show()