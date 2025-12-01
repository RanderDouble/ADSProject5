import pandas as pd
import matplotlib.pyplot as plt
import os

# 如果不存在，创建文档目录
output_dir = "../../documents"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# 读取 CSV 文件
try:
    df = pd.read_csv("results_k.csv")
except FileNotFoundError:
    print("错误: 未找到 results_k.csv。请先运行 test_k.sh。")
    exit(1)

# 设置 1x3 子图: RandomDivisible, NearMiss, Yes
fig, axes = plt.subplots(1, 3, figsize=(18, 5))
fig.suptitle('Performance Analysis: Varying K (Fixed N=20)', fontsize=16)

# 绘图辅助函数
def plot_algo_comparison(ax, data, title, ylabel="Time (s)"):
    for algo in data['Algorithm'].unique():
        subset = data[data['Algorithm'] == algo]
        subset = subset.sort_values('K')
        # 过滤掉跳过的测试 (Time < 0)
        subset = subset[subset['AvgTime'] >= 0]
        if not subset.empty:
            ax.plot(subset['K'], subset['AvgTime'], marker='o', label=algo)
    
    ax.set_title(title)
    ax.set_xlabel('K')
    ax.set_ylabel(ylabel)
    ax.set_yscale('log')  # 使用对数刻度
    ax.legend()
    ax.grid(True) # 标准网格

# 1. 随机可整除情况
divisible_data = df[df['Type'] == 'RandomDivisible']
plot_algo_comparison(axes[0], divisible_data, 'Random Divisible Case')

# 2. 差一点情况 (Near Miss)
nearmiss_data = df[df['Type'] == 'NearMiss']
plot_algo_comparison(axes[1], nearmiss_data, 'Near Miss Case')

# 3. 肯定有解情况 (Yes Case)
yes_data = df[df['Type'] == 'Yes']
plot_algo_comparison(axes[2], yes_data, 'Yes Case')

# 调整布局并保存
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
output_path = os.path.join(output_dir, "performance_analysis_k.png")
plt.savefig(output_path)
print(f"图表已保存至 {output_path}")

# 保存单独的图表

plt.figure(figsize=(8, 6))
plot_algo_comparison(plt.gca(), divisible_data, 'Random Divisible Case (Varying K)')
plt.savefig(os.path.join(output_dir, "random_divisible_comparison_k.png"))
plt.close()

plt.figure(figsize=(8, 6))
plot_algo_comparison(plt.gca(), nearmiss_data, 'Near Miss Case (Varying K)')
plt.savefig(os.path.join(output_dir, "nearmiss_comparison_k.png"))
plt.close()

plt.figure(figsize=(8, 6))
plot_algo_comparison(plt.gca(), yes_data, 'Yes Case (Varying K)')
plt.savefig(os.path.join(output_dir, "yes_comparison_k.png"))
plt.close()

print("单独的图表也已保存至 ../../documents/")
