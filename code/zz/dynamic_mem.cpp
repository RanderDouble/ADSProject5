#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x, y, z;  // 前驱状态
    int group;     // 当前物品分配给哪个组 (0,1,2)
} StateInfo;

int threePartitionWithPath(int n, int* items, int*** result) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += items[i];
    }
    
    if (total % 3 != 0 || n < 3) {
        return 0;
    }
    
    int target = total / 3;
    
    // 检查是否有元素大于目标值
    for (int i = 0; i < n; i++) {
        if (items[i] > target) {
            return 0;
        }
    }
    
    // 创建DP表和路径记录表
    int*** dp = (int***)malloc((target + 1) * sizeof(int**));
    StateInfo**** path = (StateInfo****)malloc((target + 1) * sizeof(StateInfo***));
    
    for (int i = 0; i <= target; i++) {
        dp[i] = (int**)malloc((target + 1) * sizeof(int*));
        path[i] = (StateInfo***)malloc((target + 1) * sizeof(StateInfo**));
        for (int j = 0; j <= target; j++) {
            dp[i][j] = (int*)malloc((n + 1) * sizeof(int));
            path[i][j] = (StateInfo**)malloc((n + 1) * sizeof(StateInfo*));
            for (int k = 0; k <= n; k++) {
                dp[i][j][k] = 0;
                path[i][j][k] = NULL;
            }
        }
    }
    
    // 初始化
    dp[0][0][0] = 1;
    path[0][0][0] = (StateInfo*)malloc(sizeof(StateInfo));
    path[0][0][0]->x = -1;
    path[0][0][0]->y = -1;
    path[0][0][0]->z = -1;
    path[0][0][0]->group = -1;
    
    // DP过程，同时记录路径
    for (int z = 1; z <= n; z++) {
        int item_value = items[z - 1];
        
        for (int x = 0; x <= target; x++) {
            for (int y = 0; y <= target; y++) {
                // Case 1: 将当前物品分配给组1
                if (item_value <= x && dp[x - item_value][y][z - 1]) {
                    dp[x][y][z] = 1;
                    path[x][y][z] = (StateInfo*)malloc(sizeof(StateInfo));
                    path[x][y][z]->x = x - item_value;
                    path[x][y][z]->y = y;
                    path[x][y][z]->z = z - 1;
                    path[x][y][z]->group = 0;  // 组1
                    continue;
                }
                
                // Case 2: 将当前物品分配给组2
                if (item_value <= y && dp[x][y - item_value][z - 1]) {
                    dp[x][y][z] = 1;
                    path[x][y][z] = (StateInfo*)malloc(sizeof(StateInfo));
                    path[x][y][z]->x = x;
                    path[x][y][z]->y = y - item_value;
                    path[x][y][z]->z = z - 1;
                    path[x][y][z]->group = 1;  // 组2
                    continue;
                }
                
                // Case 3: 将当前物品分配给组3 (不需要更新x,y)
                if (dp[x][y][z - 1]) {
                    dp[x][y][z] = 1;
                    path[x][y][z] = (StateInfo*)malloc(sizeof(StateInfo));
                    path[x][y][z]->x = x;
                    path[x][y][z]->y = y;
                    path[x][y][z]->z = z - 1;
                    path[x][y][z]->group = 2;  // 组3
                }
            }
        }
    }
    
    // 检查是否存在解
    if (!dp[target][target][n]) {
        // 释放内存
        for (int i = 0; i <= target; i++) {
            for (int j = 0; j <= target; j++) {
                for (int k = 0; k <= n; k++) {
                    if (path[i][j][k]) free(path[i][j][k]);
                }
                free(dp[i][j]);
                free(path[i][j]);
            }
            free(dp[i]);
            free(path[i]);
        }
        free(dp);
        free(path);
        return 0;
    }
    
    // 直接从路径记录构造解，不需要回溯
    int* assignment = (int*)malloc(n * sizeof(int));  // 记录每个物品的分配
    memset(assignment, -1, n * sizeof(int));
    
    // 从最终状态向前追踪
    int x = target, y = target, z = n;
    while (z > 0 && path[x][y][z] != NULL) {
        StateInfo* current = path[x][y][z];
        int item_index = z - 1;
        
        // 记录当前物品的分配
        assignment[item_index] = current->group;
        
        // 移动到前驱状态
        x = current->x;
        y = current->y;
        z = current->z;
    }
    
    // 根据分配结果构造三个组
    int* part1 = (int*)malloc(n * sizeof(int));
    int* part2 = (int*)malloc(n * sizeof(int));
    int* part3 = (int*)malloc(n * sizeof(int));
    int count1 = 0, count2 = 0, count3 = 0;
    
    for (int i = 0; i < n; i++) {
        if (assignment[i] == 0) {
            part1[count1++] = items[i];
        } else if (assignment[i] == 1) {
            part2[count2++] = items[i];
        } else if (assignment[i] == 2) {
            part3[count3++] = items[i];
        }
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
    
    // 释放内存
    free(part1);
    free(part2);
    free(part3);
    free(assignment);
    
    for (int i = 0; i <= target; i++) {
        for (int j = 0; j <= target; j++) {
            for (int k = 0; k <= n; k++) {
                if (path[i][j][k]) free(path[i][j][k]);
            }
            free(dp[i][j]);
            free(path[i][j]);
        }
        free(dp[i]);
        free(path[i]);
    }
    free(dp);
    free(path);
    
    return 1;
}

// 优化的版本：使用更紧凑的路径记录
int threePartitionCompact(int n, int* items, int*** result) {
    int total = 0;
    for (int i = 0; i < n; i++) total += items[i];
    if (total % 3 != 0 || n < 3) return 0;
    
    int target = total / 3;
    
    // 检查是否有元素大于目标值
    for (int i = 0; i < n; i++) {
        if (items[i] > target) return 0;
    }
    
    // 使用单个数组记录路径：prev[x][y][z] = (prev_x << 20) | (prev_y << 10) | (prev_z << 2) | group
    int*** dp = (int***)malloc((target + 1) * sizeof(int**));
    int*** prev = (int***)malloc((target + 1) * sizeof(int**));
    
    for (int i = 0; i <= target; i++) {
        dp[i] = (int**)malloc((target + 1) * sizeof(int*));
        prev[i] = (int**)malloc((target + 1) * sizeof(int*));
        for (int j = 0; j <= target; j++) {
            dp[i][j] = (int*)malloc((n + 1) * sizeof(int));
            prev[i][j] = (int*)malloc((n + 1) * sizeof(int));
            memset(dp[i][j], 0, (n + 1) * sizeof(int));
            memset(prev[i][j], -1, (n + 1) * sizeof(int));
        }
    }
    
    dp[0][0][0] = 1;
    
    for (int z = 1; z <= n; z++) {
        int item = items[z - 1];
        
        for (int x = 0; x <= target; x++) {
            for (int y = 0; y <= target; y++) {
                // 分配给组1
                if (item <= x && dp[x - item][y][z - 1]) {
                    dp[x][y][z] = 1;
                    prev[x][y][z] = ((x - item) << 20) | (y << 10) | ((z - 1) << 2) | 0;
                    continue;
                }
                
                // 分配给组2
                if (item <= y && dp[x][y - item][z - 1]) {
                    dp[x][y][z] = 1;
                    prev[x][y][z] = (x << 20) | ((y - item) << 10) | ((z - 1) << 2) | 1;
                    continue;
                }
                
                // 分配给组3
                if (dp[x][y][z - 1]) {
                    dp[x][y][z] = 1;
                    prev[x][y][z] = (x << 20) | (y << 10) | ((z - 1) << 2) | 2;
                }
            }
        }
    }
    
    if (!dp[target][target][n]) {
        // 释放内存
        for (int i = 0; i <= target; i++) {
            for (int j = 0; j <= target; j++) {
                free(dp[i][j]);
                free(prev[i][j]);
            }
            free(dp[i]);
            free(prev[i]);
        }
        free(dp);
        free(prev);
        return 0;
    }
    
    // 从路径记录构造解
    int* assignment = (int*)calloc(n, sizeof(int));
    int x = target, y = target, z = n;
    
    while (z > 0 && prev[x][y][z] != -1) {
        int packed = prev[x][y][z];
        int group = packed & 0x3;
        int prev_z = (packed >> 2) & 0xFF;
        int prev_y = (packed >> 10) & 0x3FF;
        int prev_x = (packed >> 20) & 0x3FF;
        
        assignment[z - 1] = group;
        x = prev_x;
        y = prev_y;
        z = prev_z;
    }
    
    // 构造结果
    int* part1 = (int*)malloc(n * sizeof(int));
    int* part2 = (int*)malloc(n * sizeof(int));
    int* part3 = (int*)malloc(n * sizeof(int));
    int count1 = 0, count2 = 0, count3 = 0;
    
    for (int i = 0; i < n; i++) {
        if (assignment[i] == 0) part1[count1++] = items[i];
        else if (assignment[i] == 1) part2[count2++] = items[i];
        else if (assignment[i] == 2) part3[count3++] = items[i];
    }
    
    *result = (int**)malloc(3 * sizeof(int*));
    (*result)[0] = (int*)malloc((count1 + 1) * sizeof(int));
    (*result)[1] = (int*)malloc((count2 + 1) * sizeof(int));
    (*result)[2] = (int*)malloc((count3 + 1) * sizeof(int));
    
    (*result)[0][0] = count1;
    for (int i = 0; i < count1; i++) (*result)[0][i + 1] = part1[i];
    (*result)[1][0] = count2;
    for (int i = 0; i < count2; i++) (*result)[1][i + 1] = part2[i];
    (*result)[2][0] = count3;
    for (int i = 0; i < count3; i++) (*result)[2][i + 1] = part3[i];
    
    // 释放内存
    free(part1);
    free(part2);
    free(part3);
    free(assignment);
    
    for (int i = 0; i <= target; i++) {
        for (int j = 0; j <= target; j++) {
            free(dp[i][j]);
            free(prev[i][j]);
        }
        free(dp[i]);
        free(prev[i]);
    }
    free(dp);
    free(prev);
    
    return 1;
}

// 打印结果函数
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

void freeResult(int** result) {
    for (int i = 0; i < 3; i++) {
        free(result[i]);
    }
    free(result);
}

// 测试函数
int main() {
    int test1[] = {1, 2, 3, 4, 5};
    int n1 = 5;
    int** result1;
    
    printf("测试 [1,2,3,4,5]:\n");
    if (threePartitionCompact(n1, test1, &result1)) {
        printf("yes\n");
        printResult(result1);
        freeResult(result1);
    } else {
        printf("no\n");
    }
    
    printf("\n测试 [7,8,9,10,11]:\n");
    int test2[] = {7, 8, 9, 10, 11};
    int n2 = 5;
    int** result2;
    if (threePartitionCompact(n2, test2, &result2)) {
        printf("yes\n");
        printResult(result2);
        freeResult(result2);
    } else {
        printf("no\n");
    }
    
    return 0;
}