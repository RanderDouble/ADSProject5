#include <cstdio>
#include <cstdlib>
#include <ctime>

// Forward declarations
namespace Backtracking {
    void solve(int n, int* nums);
}
namespace BitmaskDP {
    void solve(int n, int* nums);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    
    int* nums = (int*)malloc(n * sizeof(int));
    for(int i=0; i<n; ++i) scanf("%d", &nums[i]);
    
    printf("--- Backtracking ---\n");
    clock_t start = clock();
    Backtracking::solve(n, nums);
    clock_t end = clock();
    double time_bt = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.4f s\n", time_bt);
    
    printf("\n--- Bitmask DP ---\n");
    start = clock();
    BitmaskDP::solve(n, nums);
    end = clock();
    double time_dp = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.4f s\n", time_dp);
    
    free(nums);
    return 0;
}
