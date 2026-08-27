import pandas as pd

data = pd.read_csv("data/Depth Data.csv")

# Convert depth values from text to numbers
data["Depth (m)"] = pd.to_numeric(data["Depth (m)"], errors="coerce")

print("First 5 readings:")
print(data.head())

print("\nDataset information:")
data.info()

print("\nDataset dimensions:")
print(data.shape)

print("\nInvalid readings:")
print(data[data["Depth (m)"].isna()])

print("\nDepth statistics:")
print("Minimum depth:", data["Depth (m)"].min(), "m")
print("Maximum depth:", data["Depth (m)"].max(), "m")
print("Average depth:", data["Depth (m)"].mean(), "m")
print("Valid readings:", data["Depth (m)"].count())
print("Invalid readings:", data["Depth (m)"].isna().sum())