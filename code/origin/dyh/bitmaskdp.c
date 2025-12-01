#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 定义最大可能的 N。由于 O(N * 2^N) 的限制，N 通常不能超过 20-25。
// 这里为了安全使用动态内存分配，但若是静态数组通常设为 25。
int n;
int *numbers;
int target;
int *memo; // 记忆化数组: -1=未知, 0=不可行, 1=可行

// 检查第 i 位是否为 1
bool is_set(int mask, int i) { return (mask & (1 << i)) != 0; }

// 核心 DP 函数 (Top-Down with Memoization)
// mask: 当前使用的数字集合
// current_sum: 当前正在填装的桶的已有和
bool dp(int mask, int current_sum) {
    // 1. Base Case: 如果掩码全为1，说明所有数字都用完了，成功
    if (mask == (1 << n) - 1) {
        return true;
    }

    // 2. 查表 (Memoization)
    if (memo[mask] != -1) {
        return memo[mask];
    }

    // 3. 状态转移
    bool possible = false;
    for (int i = 0; i < n; i++) {
        // 如果数字 i 还没被使用
        if (!is_set(mask, i)) {
            // 如果加上这个数字没有超过目标和
            if (current_sum + numbers[i] <= target) {
                // 计算新的累加和。如果填满了当前桶 (== target)，则取模变为 0，意味着开始填下一个桶
                int next_sum = (current_sum + numbers[i]) % target;

                // 递归调用
                if (dp(mask | (1 << i), next_sum)) {
                    possible = true;
                    break; // 只要找到一条可行路径即可
                }
            }
        }
    }

    // 4. 记录结果并返回
    return memo[mask] = (possible ? 1 : 0);
}

// 辅助函数：根据 DP 结果打印路径
// 逻辑与 dp 函数几乎一致，只是不返回 bool 而是直接打印
void print_solution(int mask, int current_sum) {
    if (mask == (1 << n) - 1)
        return;

    for (int i = 0; i < n; i++) {
        if (!is_set(mask, i)) {
            if (current_sum + numbers[i] <= target) {
                int next_sum = (current_sum + numbers[i]) % target;

                // 关键点：我们只走 DP 表中标记为“可行 (1)”的路
                if (dp(mask | (1 << i), next_sum)) {
                    printf("%d ", numbers[i]);

                    // 如果当前桶填满了，换行
                    if (current_sum + numbers[i] == target) {
                        printf("\n");
                    }

                    // 递归打印剩余部分
                    print_solution(mask | (1 << i), next_sum);
                    return;
                }
            }
        }
    }
}

int main() {
    // 1. 输入
    if (scanf("%d", &n) != 1)
        return 0;

    numbers = (int *)malloc(n * sizeof(int));
    int total_sum = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &numbers[i]);
        total_sum += numbers[i];
    }

    // 2. 初步检查
    if (total_sum % 3 != 0) {
        printf("no\n");
        free(numbers);
        return 0;
    }
    target = total_sum / 3;

    // 3. 初始化 DP 表
    // 大小为 2^n，对于 n=20 大约是 1MB，对于 n=25 大约是 32MB
    // 使用 calloc 初始化为 0 是不行的，因为我们需要 -1 代表未访问
    // 这里使用 malloc + memset
    long long num_states = (1LL << n);
    memo = (int *)malloc(num_states * sizeof(int));
    if (memo == NULL) {
        printf("Memory limit exceeded for this N\n");
        free(numbers);
        return 0;
    }
    // 将 memo 的每个字节设为 -1 (在补码表示中 int 的 -1 所有位都是 1，这在 memset 中有效)
    // 注意：memset 按字节赋值，int 是4字节，但 -1 的字节表示是 0xFF，所以 memset 赋值 -1 是安全的
    memset(memo, -1, num_states * sizeof(int));

    // 4. 执行 DP
    if (dp(0, 0)) {
        printf("yes\n");
        print_solution(0, 0);
    } else {
        printf("no\n");
    }

    // 5. 释放内存
    free(numbers);
    free(memo);

    return 0;
}