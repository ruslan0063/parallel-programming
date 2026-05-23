import csv
import matplotlib.pyplot as plt

data = {}

with open("results.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        size = int(row["size"])
        threads = int(row["threads"])
        time = int(row["time_ms"])

        if threads not in data:
            data[threads] = {"x": [], "y": []}

        data[threads]["x"].append(size)
        data[threads]["y"].append(time)

plt.figure(figsize=(10,6))

for t in sorted(data.keys()):
    plt.plot(data[t]["x"], data[t]["y"], marker="o", label=f"{t} threads")

plt.xlabel("Matrix size")
plt.ylabel("Time (ms)")
plt.title("OpenMP performance: size vs threads")
plt.grid()
plt.legend()

plt.savefig("lab2.png", dpi=200)
plt.show()