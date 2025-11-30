#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int*** createDPTable(int target, int n) {
    int*** dp = (int***)malloc((target + 1) * sizeof(int**));
    for (int i = 0; i <= target; i++) {
        dp[i] = (int**)malloc((target + 1) * sizeof(int*));
        for (int j = 0; j <= target; j++) {
            dp[i][j] = (int*)malloc((n + 1) * sizeof(int));
            memset(dp[i][j], 0, (n + 1) * sizeof(int));
        }
    }
    return dp;
}
void freeDPTable(int*** dp, int target) {
    for (int i = 0; i <= target; i++) {
        for (int j = 0; j <= target; j++) {
            free(dp[i][j]);
        }
        free(dp[i]);
    }
    free(dp);
}
int threePartitionDP(int n, int* items, int*** result) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += items[i];
    }
    //前期剪枝
    if (total % 3 != 0 || n < 3) {
        return 0;
    }
    int target = total / 3;
    for (int i = 0; i < n; i++) {
        if (items[i] > target) {
            return 0;
        }
    }
    int*** dp = createDPTable(target, n);
    dp[0][0][0] = 1;
    // DP状态转移
    //每次选取一个数字，必然在三个子集中挑选一个
    //则就是dp[i-x][j][k-1] 或 dp[i][j-x][k-1] 或 dp[i][j][k-1] 三种情况之一
    for (int z = 1; z <= n; z++) {
        int item_value = items[z - 1];
        dp[0][0][z] = 1; // 初始化
        for (int x = 0; x <= target; x++) {
            int remain1 = x - item_value;
            for (int y = 0; y <= target; y++) {
                int remain2 = y - item_value;
                // Case 1: 数字给subset3
                 if (dp[x][y][z - 1]) {
                    dp[x][y][z] = 1;
                }
                // Case 2: 将当前数字给subset1
                else if (remain1 >= 0 && dp[remain1][y][z - 1]) {
                    dp[x][y][z] = 1;
                }
                // Case 3: 将当前数字给subset2
                else if (remain2 >= 0 && dp[x][remain2][z - 1]) {
                    dp[x][y][z] = 1;
                }
            }
        }
    }
    //不存在解
    if (!dp[target][target][n]) {
        freeDPTable(dp, target);
        return 0;
    }
    //回溯构造解
    int* part1 = (int*)malloc(n * sizeof(int));
    int* part2 = (int*)malloc(n * sizeof(int));
    int* part3 = (int*)malloc(n * sizeof(int));
    int count1 = 0, count2 = 0, count3 = 0;
    int x = target, y = target, z = n;
    while (z > 0) {
        //每次选取最后一个数字进行判断
        //若果dp[x][y][z-1]为真，则说明该数字未被使用，放入subset3
        //否则判断该数字是否放入subset1或subset2
        //若dp[x - item_value][y][z-1]==1,放入subset1，则x减少该数字值
        //若dp[x][y - item_value][z-1]==1,放入subset2，则y减少该数字值
        int item_value = items[z - 1];
        //结束条件
        if (x == 0 && y == 0) {
            break;
        }
        // Case 1: 数字给subset3
        if (dp[x][y][z - 1]) {
            part3[count3++] = item_value;
            z--;
        }
        // Case 2: 数字给subset1
        else if (item_value <= x && dp[x - item_value][y][z - 1]) {
            part1[count1++] = item_value;
            x = x - item_value;
            z--;
        }
        // Case 3: 数字给subset2
        else if (item_value <= y && dp[x][y - item_value][z - 1]) {
            part2[count2++] = item_value;
            y = y - item_value;
            z--;
        }
        else {
            // 这种情况不存在
            printf("Error in backtracking\n");
            part3[count3++] = item_value;
            z--;
        }
    }
    
    // 处理剩余物品
    while (z > 0) {
        part3[count3++] = items[z - 1];
        z--;
    }
    
    // 准备返回结果
    *result = (int**)malloc(3 * sizeof(int*));
    (*result)[0] = (int*)malloc((count1 + 1) * sizeof(int));
    (*result)[1] = (int*)malloc((count2 + 1) * sizeof(int));
    (*result)[2] = (int*)malloc((count3 + 1) * sizeof(int));
    
    (*result)[0][0] = count1;
    for (int i = 0; i < count1; i++) {
        (*result)[0][i + 1] = part1[i];
    }
    
    (*result)[1][0] = count2;
    for (int i = 0; i < count2; i++) {
        (*result)[1][i + 1] = part2[i];
    }
    
    (*result)[2][0] = count3;
    for (int i = 0; i < count3; i++) {
        (*result)[2][i + 1] = part3[i];
    }
    
    // 释放临时内存
    free(part1);
    free(part2);
    free(part3);
    freeDPTable(dp, target);
    
    return 1;
}

// 打印结果
void printResult(int** result) {
    for (int i = 0; i < 3; i++) {
        int count = result[i][0];
        for (int j = 1; j <= count; j++) {
            printf("%d", result[i][j]);
            if (j < count) printf(" ");
        }
        printf("\n");
    }
}

// 释放结果内存
void freeResult(int** result) {
    for (int i = 0; i < 3; i++) {
        free(result[i]);
    }
    free(result);
}

// 测试函数
void runTests() {
    printf("=== 测试三划分算法 ===\n\n");
    
    // 测试用例1: 可以划分 [1,2,3,4,5]
    {
        int test1[] = {1, 2, 3, 4, 5};
        int n1 = 5;
        int** result1;
        printf("测试1 [1,2,3,4,5]: ");
        if (threePartitionDP(n1, test1, &result1)) {
            printf("yes\n");
            printResult(result1);
            freeResult(result1);
        } else {
            printf("no\n");
        }
        printf("\n");
    }
    
    // 测试用例2: 不能划分 [7,8,9,10,11]
    {
        int test2[] = {7, 8, 9, 10, 11};
        int n2 = 5;
        int** result2;
        printf("测试2 [7,8,9,10,11]: ");
        if (threePartitionDP(n2, test2, &result2)) {
            printf("yes\n");
            printResult(result2);
            freeResult(result2);
        } else {
            printf("no\n");
        }
        printf("\n");
    }
    
    // 测试用例3: 边界情况 [3,3,3,3,3,3]
    {
        int test3[] = {3, 3, 3, 3, 3, 3};
        int n3 = 6;
        int** result3;
        printf("测试3 [3,3,3,3,3,3]: ");
        if (threePartitionDP(n3, test3, &result3)) {
            printf("yes\n");
            printResult(result3);
            freeResult(result3);
        } else {
            printf("no\n");
        }
        printf("\n");
    }
    
    // 测试用例4: 复杂情况 [4,4,5,2,3,3]
    {
        int test4[] = {4, 4, 5, 2, 3, 3};
        int n4 = 6;
        int** result4;
        printf("测试4 [4,4,5,2,3,3]: ");
        if (threePartitionDP(n4, test4, &result4)) {
            printf("yes\n");
            printResult(result4);
            freeResult(result4);
        } else {
            printf("no\n");
        }
        printf("\n");
    }
}

int main() {
    runTests();
    
    // 交互式测试
    printf("=== 交互式测试 ===\n");
    printf("输入元素个数: ");
    int n;
    scanf("%d", &n);
    
    int* nums = (int*)malloc(n * sizeof(int));
    printf("输入 %d 个元素: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }
    
    int** result;
    if (threePartitionDP(n, nums, &result)) {
        printf("yes\n");
        printResult(result);
        freeResult(result);
    } else {
        printf("no\n");
    }
    
    free(nums);
    return 0;
}