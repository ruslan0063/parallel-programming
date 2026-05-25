import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv(
    "results.csv",
    names=[
        "MatrixSize",
        "BlockConfiguration",
        "SequentialTime_ms",
        "ParallelTime_ms"
    ]
)

plt.figure(figsize=(10, 6))

for block in df["BlockConfiguration"].unique():

    subset = df[
        df["BlockConfiguration"] == block
    ]

    plt.plot(
        subset["MatrixSize"],
        subset["ParallelTime_ms"],
        marker="o",
        label=f"Блок {block}"
    )

cpu = df[
    ["MatrixSize", "SequentialTime_ms"]
].drop_duplicates()

plt.plot(
    cpu["MatrixSize"],
    cpu["SequentialTime_ms"],
    marker="s",
    linestyle="--",
    color="black",
    label="CPU"
)

plt.xlabel("Размер матрицы")
plt.ylabel("Время выполнения (мс)")
plt.title("CUDA: CPU vs GPU")
plt.legend()
plt.grid(True)

plt.yscale("log")

plt.savefig("graph.png", dpi=150)

plt.show()