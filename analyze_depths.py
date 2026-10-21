import sys
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Usage: python3 <this_script>.py [csv_path]
# Default: depth_results.csv in the same folder as this script. Outputs go there too.
here = Path(__file__).resolve().parent
csv_path = Path(sys.argv[1]) if len(sys.argv) > 1 else here / "depth_results.csv"
df = pd.read_csv(csv_path)

# Sanity checks: expected columns, no blanks, and 1 Sorted + 10 Random rows per tree and size
expected = ["Tree", "N", "Order", "Trial", "Seed", "Height", "AvgDepth"]
assert list(df.columns) == expected and not df.isna().any().any(), \
    f"Unexpected columns or blank values in CSV: {list(df.columns)}"
counts = df.groupby(["Tree", "N", "Order"]).size().unstack("Order")
assert (counts["Sorted"] == 1).all() and (counts["Random"] == 10).all(), \
    f"CSV looks truncated, row counts per group:\n{counts}"

# Summary table, saved for the report (readable headers)
table = df.groupby(["N", "Tree", "Order"]).agg(
    H_mean=("Height", "mean"), H_min=("Height", "min"), H_max=("Height", "max"),
    D_mean=("AvgDepth", "mean"), D_min=("AvgDepth", "min"), D_max=("AvgDepth", "max"),
).round(3).reset_index()
readable = table.rename(columns={
    "H_mean": "Height mean", "H_min": "Height min", "H_max": "Height max",
    "D_mean": "AvgDepth mean", "D_min": "AvgDepth min", "D_max": "AvgDepth max",
})
readable.to_csv(here / "summary.csv", index=False)
print(readable.to_string(index=False))

# Plot: height (left), average depth (right)
# Bounds match the C++ checks: 1.44 lg(N+2) and 2 lg(N+1)
n = np.geomspace(df["N"].min(), df["N"].max(), 100)
fig, axes = plt.subplots(1, 2, figsize=(12, 5))
for ax, p, title in zip(axes, ["H", "D"], ["Height vs N", "Average depth minus lg N (dotted line: lg N)"]):
    shift = np.log2 if p == "D" else (lambda x: 0)  # right panel: average depth minus lg N
    ax.plot(n, np.log2(n) - shift(n), "k:", label="lg N")
    if p == "H":
        ax.plot(n, 1.44 * np.log2(n + 2), "b:", label="1.44 lg(N+2) (AVL bound)")
        ax.plot(n, 2 * np.log2(n + 1), "r:", label="2 lg(N+1) (RB bound)")
    for tree, c in [("AVL", "b"), ("RB", "r")]:
        s = table[(table.Tree == tree) & (table.Order == "Sorted")]
        r = table[(table.Tree == tree) & (table.Order == "Random")]
        ax.plot(s.N, s[p + "_mean"] - shift(s.N), c + "--o", label=f"{tree} Sorted")
        ax.plot(r.N, r[p + "_mean"] - shift(r.N), c + "-s", label=f"{tree} Random (mean, band = min-max)")
        ax.fill_between(r.N, r[p + "_min"] - shift(r.N), r[p + "_max"] - shift(r.N), color=c, alpha=0.15)
    ax.set_xscale("log", base=2)
    ax.set_title(title)
    ax.set_xlabel("N")
    ax.set_ylabel("Height" if p == "H" else "Average depth - lg N")
    ax.grid(alpha=0.3)
axes[0].legend(fontsize=8, loc="upper left")
fig.tight_layout()
fig.savefig(here / "depth_plot.png", dpi=300)