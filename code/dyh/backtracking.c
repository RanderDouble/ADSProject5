#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

long long count = 0;

// 全局变量用于存储数据
int *numbers;      // 存储输入的数字
int *bucket_id;    // 记录每个数字属于哪个桶 (0, 1, 或 2)
int bucket_sum[3]; // 当前3个桶的累加和
int target_sum;    // 每个桶的目标和
int n;             // 数字的总个数

// 用于qsort的比较函数，降序排列
int compare(const void *a, const void *b) { return (*(int *)b - *(int *)a); }

// 回溯核心函数
// index: 当前正在处理第几个数字
bool backtrack(int index) {
    count++;
    if (count % 10000000 == 0) { // 每跑一千万次打印一下
        printf("Searching... Steps: %lld\n", count);
    }
    // 基准情况：如果所有数字都处理完了
    if (index == n) {
        // 检查三个桶的和是否都等于目标和
        return (bucket_sum[0] == target_sum && bucket_sum[1] == target_sum && bucket_sum[2] == target_sum);
    }

    // 尝试将当前数字 numbers[index] 放入 3 个桶中的某一个
    for (int i = 0; i < 3; i++) {
        // 剪枝 1: 如果放入该桶后超过目标和，则跳过
        if (bucket_sum[i] + numbers[index] > target_sum) {
            continue;
        }

        // 剪枝 2 (重要): 如果当前桶的和与前一个桶的和相同，
        // 且前一个桶已经尝试过失败了，那么放入当前桶也是等价的，直接跳过。
        // 这消除了搜索树中的对称性。
        if (i > 0 && bucket_sum[i] == bucket_sum[i - 1]) {
            continue;
        }

        // 做选择
        bucket_sum[i] += numbers[index];
        bucket_id[index] = i;

        // 递归
        if (backtrack(index + 1)) {
            return true;
        }

        // 撤销选择 (回溯)
        bucket_sum[i] -= numbers[index];
        bucket_id[index] = -1; // 可选，主要用于调试
    }

    return false;
}

int main() {
    // 1. 输入处理
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input\n");
        return 0;
    }

    numbers = (int *)malloc(n * sizeof(int));
    bucket_id = (int *)malloc(n * sizeof(int));
    int total_sum = 0;

    for (int i = 0; i < n; i++) {
        scanf("%d", &numbers[i]);
        total_sum += numbers[i];
    }

    // 2. 初步检查
    // 如果总和不能被3整除，直接失败
    if (total_sum % 3 != 0) {
        printf("no\n");
        free(numbers);
        free(bucket_id);
        return 0;
    }

    target_sum = total_sum / 3;
    bucket_sum[0] = 0;
    bucket_sum[1] = 0;
    bucket_sum[2] = 0;

    // 3. 排序优化 (降序)
    // 从大到小尝试可以更快地填满桶，或者更快地发现不可能填满（剪枝）
    qsort(numbers, n, sizeof(int), compare);

    // 4. 执行回溯
    if (backtrack(0)) {
        printf("yes\n");
        // 输出结果：遍历3个桶，打印属于该桶的数字
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < n; j++) {
                if (bucket_id[j] == i) {
                    printf("%d ", numbers[j]);
                }
            }
            printf("\n");
        }
    } else {
        printf("no\n");
    }

    // 释放内存
    free(numbers);
    free(bucket_id);

    return 0;
}