import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("supercomputer_res.csv")

df["MatrixSize"] = pd.to_numeric(df["MatrixSize"])
df["Processes"] = pd.to_numeric(df["Processes"])
df["ParallelTime_ms"] = pd.to_numeric(df["ParallelTime_ms"])

plt.figure(figsize=(10, 6))

for proc in sorted(df["Processes"].unique()):

    subset = df[df["Processes"] == proc]

    plt.plot(
        subset["MatrixSize"],
        subset["ParallelTime_ms"],
        marker='o',
        linewidth=2,
        label=f"{proc} процессов"
    )

plt.xlabel("Размер матрицы")
plt.ylabel("Время выполнения (мс)")

plt.title(
    "MPI на суперкомпьютере: "
    "зависимость времени выполнения"
)

plt.legend()
plt.grid(True)

plt.tight_layout()

plt.savefig(
    "supercomputer_plot.png",
    dpi=150
)

plt.show()