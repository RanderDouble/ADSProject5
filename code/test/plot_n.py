import pandas as pd
import matplotlib.pyplot as plt
import os

# 如果不存在，创建文档目录
output_dir = "../../documents"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# 读取 CSV 文件
try:
    df = pd.read_csv("results_n.csv")
except FileNotFoundError:
    print("错误: 未找到 results_n.csv。请先运行 test_n.sh。")
    exit(1)

# 设置 2x3 子图
fig, axes = plt.subplots(2, 3, figsize=(18, 12))
fig.suptitle('Performance Analysis: Backtracking (Pruned vs NoPrune) vs DP', fontsize=16)

# 绘图辅助函数
def plot_algo_comparison(ax, data, title, ylabel="Time (s)"):
    for algo in data['Algorithm'].unique():
        subset = data[data['Algorithm'] == algo]
        subset = subset.sort_values('N')
        # 过滤掉跳过的测试 (Time < 0)
        subset = subset[subset['AvgTime'] >= 0]
        if not subset.empty:
            ax.plot(subset['N'], subset['AvgTime'], marker='o', label=algo)
    
    ax.set_title(title)
    ax.set_xlabel('N')
    ax.set_ylabel(ylabel)
    ax.set_yscale('log')  # 使用对数刻度
    ax.legend()
    ax.grid(True) # 标准网格

# 1. 随机可整除情况
divisible_data = df[df['Type'] == 'RandomDivisible']
plot_algo_comparison(axes[0, 0], divisible_data, 'Random Divisible Case: Comparison')

# 2. 差一点情况 (Near Miss)
nearmiss_data = df[df['Type'] == 'NearMiss']
plot_algo_comparison(axes[0, 1], nearmiss_data, 'Near Miss Case: Comparison')

# 3. 肯定有解情况 (Yes Case)
yes_data = df[df['Type'] == 'Yes']
plot_algo_comparison(axes[0, 2], yes_data, 'Yes Case: Comparison')

# 4. 回溯: 所有情况
bt_data = df[df['Algorithm'] == 'Backtracking']
for case_type in bt_data['Type'].unique():
    if case_type == 'Random': continue
    subset = bt_data[bt_data['Type'] == case_type].sort_values('N')
    subset = subset[subset['AvgTime'] >= 0]
    if not subset.empty:
        axes[1, 0].plot(subset['N'], subset['AvgTime'], marker='o', label=f"{case_type} Case")
axes[1, 0].set_title('Backtracking: All Cases')
axes[1, 0].set_xlabel('N')
axes[1, 0].set_ylabel('Time (s)')
# axes[1, 0].set_yscale('log') # Removed log scale
axes[1, 0].legend()
axes[1, 0].grid(True)

# 5. 状态压缩 DP: 所有情况
dp_data = df[df['Algorithm'] == 'BitmaskDP']
for case_type in dp_data['Type'].unique():
    if case_type == 'Random': continue
    subset = dp_data[dp_data['Type'] == case_type].sort_values('N')
    subset = subset[subset['AvgTime'] >= 0]
    if not subset.empty:
        axes[1, 1].plot(subset['N'], subset['AvgTime'], marker='o', label=f"{case_type} Case")
axes[1, 1].set_title('Bitmask DP: All Cases')
axes[1, 1].set_xlabel('N')
axes[1, 1].set_ylabel('Time (s)')
# axes[1, 1].set_yscale('log') # Removed log scale
axes[1, 1].legend()
axes[1, 1].grid(True)

# 6. 维度 DP: 所有情况
dimen_data = df[df['Algorithm'] == 'DimenDP']
for case_type in dimen_data['Type'].unique():
    if case_type == 'Random': continue
    subset = dimen_data[dimen_data['Type'] == case_type].sort_values('N')
    subset = subset[subset['AvgTime'] >= 0]
    if not subset.empty:
        axes[1, 2].plot(subset['N'], subset['AvgTime'], marker='o', label=f"{case_type} Case")
axes[1, 2].set_title('Dimen DP: All Cases')
axes[1, 2].set_xlabel('N')
axes[1, 2].set_ylabel('Time (s)')
# axes[1, 2].set_yscale('log') # Removed log scale
axes[1, 2].legend()
axes[1, 2].grid(True)

# 调整布局并保存
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
output_path = os.path.join(output_dir, "performance_analysis.png")
plt.savefig(output_path)
print(f"图表已保存至 {output_path}")

# 保存单独的图表
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

print("单独的图表也已保存至 ../../documents/")
