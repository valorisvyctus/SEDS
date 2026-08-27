# Athena - Task 1: Finding the Sea Floor
# Name: Chiranthan Radhakrishna
# ID: 2026A7PS0163H

import pandas as pd
from pathlib import Path
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# ---------------------------------------------------------
# 1. Load the depth sensor data
# ---------------------------------------------------------

data_path = Path(__file__).parent / "Depth Data.csv"
data = pd.read_csv(data_path)

# Convert depth values to numbers.
# Invalid/corrupted values are converted to NaN.
data["Depth (m)"] = pd.to_numeric(data["Depth (m)"], errors="coerce")

# The sensor was recorded once every second.
data["Time (s)"] = data["Point"] - 1

# ---------------------------------------------------------
# 2. Handle corrupted readings
# ---------------------------------------------------------

invalid_count = data["Depth (m)"].isna().sum()

print("Total readings:", len(data))
print("Invalid readings:", invalid_count)

# Interpolate missing values using neighbouring readings.
# This prevents the corrupted reading from breaking the graph.
data["Depth Clean (m)"] = data["Depth (m)"].interpolate(
    method="linear",
    limit_direction="both"
)

# ---------------------------------------------------------
# 3. Reduce random sensor noise
# ---------------------------------------------------------

# Median filtering removes isolated spikes while preserving
# the overall shape of the depth signal.
data["Depth Filtered (m)"] = (
    data["Depth Clean (m)"]
    .rolling(window=5, center=True, min_periods=1)
    .median()
)
# ---------------------------------------------------------
# 4. Create the graph
# ---------------------------------------------------------

fig, ax = plt.subplots(figsize=(11, 6))

ax.set_title("Ship Depth Sensor Data", fontsize=16)
ax.set_xlabel("Time (seconds)", fontsize=12)
ax.set_ylabel("Depth (m)", fontsize=12)

ax.grid(True, alpha=0.3)

# Set graph limits using the complete dataset.
ax.set_xlim(data["Time (s)"].min(), data["Time (s)"].max())

depth_min = data["Depth (m)"].min()
depth_max = data["Depth (m)"].max()

ax.set_ylim(depth_min - 50, depth_max + 50)

# Raw sensor readings
raw_line, = ax.plot(
    [],
    [],
    linestyle=":",
    linewidth=1,
    label="Raw sensor data"
)

# Filtered data
filtered_line, = ax.plot(
    [],
    [],
    linewidth=2,
    label="Filtered depth"
)

ax.legend()

# ---------------------------------------------------------
# 5. Animate the graph
# ---------------------------------------------------------

def update(frame):
    """
    Display one additional sensor reading per animation frame.
    Since the sensor records once per second, each frame
    represents one second of the ship's journey.
    """

    current_time = data["Time (s)"].iloc[:frame + 1]

    raw_depth = data["Depth (m)"].iloc[:frame + 1]

    filtered_depth = data["Depth Filtered (m)"].iloc[:frame + 1]

    raw_line.set_data(current_time, raw_depth)
    filtered_line.set_data(current_time, filtered_depth)

    return raw_line, filtered_line


animation = FuncAnimation(
    fig,
    update,
    frames=len(data),
    interval=1000,
    blit=True,
    repeat=False
)

plt.tight_layout()

plt.show()