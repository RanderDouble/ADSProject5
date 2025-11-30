import pandas as pd
import matplotlib.pyplot as plt
import os

# Create documents directory if it doesn't exist
output_dir = "../documents"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Read the CSV file
try:
    df = pd.read_csv("results.csv")
except FileNotFoundError:
    print("Error: results.csv not found. Please run test.sh first.")
    exit(1)

# Set up the figure with 2x4 subplots (changed to accommodate new case)
fig, axes = plt.subplots(2, 4, figsize=(24, 12))
fig.suptitle('Performance Analysis: Backtracking vs Bitmask DP', fontsize=16)

# Helper function to plot data
def plot_algo_comparison(ax, data, title, ylabel="Time (s)"):
    for algo in data['Algorithm'].unique():
        subset = data[data['Algorithm'] == algo]
        subset = subset.sort_values('N')
        ax.plot(subset['N'], subset['AvgTime'], marker='o', label=algo)
    
    ax.set_title(title)
    ax.set_xlabel('N')
    ax.set_ylabel(ylabel)
    ax.legend()
    ax.grid(True)

# 1. Random Case: Backtracking vs Bitmask DP
random_data = df[df['Type'] == 'Random']
plot_algo_comparison(axes[0, 0], random_data, 'Random Case: Comparison')

# 2. Random Divisible Case: Backtracking vs Bitmask DP
divisible_data = df[df['Type'] == 'RandomDivisible']
plot_algo_comparison(axes[0, 1], divisible_data, 'Random Divisible Case: Comparison')

# 3. Near Miss Case: Backtracking vs Bitmask DP
nearmiss_data = df[df['Type'] == 'NearMiss']
plot_algo_comparison(axes[0, 2], nearmiss_data, 'Near Miss Case: Comparison')

# 4. Yes Case: Backtracking vs Bitmask DP
yes_data = df[df['Type'] == 'Yes']
plot_algo_comparison(axes[0, 3], yes_data, 'Yes Case: Comparison')

# 5. Backtracking: All Cases
bt_data = df[df['Algorithm'] == 'Backtracking']
for case_type in bt_data['Type'].unique():
    subset = bt_data[bt_data['Type'] == case_type].sort_values('N')
    axes[1, 0].plot(subset['N'], subset['AvgTime'], marker='o', label=f"{case_type} Case")
axes[1, 0].set_title('Backtracking: All Cases')
axes[1, 0].set_xlabel('N')
axes[1, 0].set_ylabel('Time (s)')
axes[1, 0].legend()
axes[1, 0].grid(True)

# 6. Bitmask DP: All Cases
dp_data = df[df['Algorithm'] == 'BitmaskDP']
for case_type in dp_data['Type'].unique():
    subset = dp_data[dp_data['Type'] == case_type].sort_values('N')
    axes[1, 1].plot(subset['N'], subset['AvgTime'], marker='o', label=f"{case_type} Case")
axes[1, 1].set_title('Bitmask DP: All Cases')
axes[1, 1].set_xlabel('N')
axes[1, 1].set_ylabel('Time (s)')
axes[1, 1].legend()
axes[1, 1].grid(True)

# 7. Empty plot or Summary
axes[1, 2].axis('off')
axes[1, 2].text(0.1, 0.5, "Comparison Summary:\n\nRandom: Often fails fast (sum%3!=0)\nRandomDivisible: Harder 'No' cases\nNearMiss: Perturbed Yes cases\nYes: Constructed valid cases", fontsize=12)

axes[1, 3].axis('off')

# Adjust layout and save
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
output_path = os.path.join(output_dir, "performance_analysis.png")
plt.savefig(output_path)
print(f"Plot saved to {output_path}")

# Save separate plots
plt.figure(figsize=(8, 6))
plot_algo_comparison(plt.gca(), random_data, 'Random Case: Comparison')
plt.savefig(os.path.join(output_dir, "random_comparison.png"))
plt.close()

plt.figure(figsize=(8, 6))
plot_algo_comparison(plt.gca(), divisible_data, 'Random Divisible Case: Comparison')
plt.savefig(os.path.join(output_dir, "random_divisible_comparison.png"))
plt.close()

plt.figure(figsize=(8, 6))
plot_algo_comparison(plt.gca(), nearmiss_data, 'Near Miss Case: Comparison')
plt.savefig(os.path.join(output_dir, "nearmiss_comparison.png"))
plt.close()

plt.figure(figsize=(8, 6))
plot_algo_comparison(plt.gca(), yes_data, 'Yes Case: Comparison')
plt.savefig(os.path.join(output_dir, "yes_comparison.png"))
plt.close()

plt.figure(figsize=(8, 6))
for case_type in bt_data['Type'].unique():
    subset = bt_data[bt_data['Type'] == case_type].sort_values('N')
    plt.plot(subset['N'], subset['AvgTime'], marker='o', label=f"{case_type} Case")
plt.title('Backtracking: All Cases')
plt.xlabel('N')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True)
plt.savefig(os.path.join(output_dir, "backtracking_analysis.png"))
plt.close()

plt.figure(figsize=(8, 6))
for case_type in dp_data['Type'].unique():
    subset = dp_data[dp_data['Type'] == case_type].sort_values('N')
    plt.plot(subset['N'], subset['AvgTime'], marker='o', label=f"{case_type} Case")
plt.title('Bitmask DP: All Cases')
plt.xlabel('N')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True)
plt.savefig(os.path.join(output_dir, "dp_analysis.png"))
plt.close()

print("Individual plots also saved to ../documents/")
