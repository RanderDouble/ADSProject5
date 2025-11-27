#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace BitmaskDP {
    int n;
    int *numbers;
    int target;
    int *memo;

    bool is_set(int mask, int i) { return (mask & (1 << i)) != 0; }

    bool dp(int mask, int current_sum) {
        if (mask == (1 << n) - 1) {
            return true;
        }

        if (memo[mask] != -1) {
            return memo[mask];
        }

        bool possible = false;
        for (int i = 0; i < n; i++) {
            if (!is_set(mask, i)) {
                if (current_sum + numbers[i] <= target) {
                    int next_sum = (current_sum + numbers[i]) % target;

                    if (dp(mask | (1 << i), next_sum)) {
                        possible = true;
                        break;
                    }
                }
            }
        }

        return memo[mask] = (possible ? 1 : 0);
    }

    void print_solution(int mask, int current_sum) {
        if (mask == (1 << n) - 1)
            return;

        for (int i = 0; i < n; i++) {
            if (!is_set(mask, i)) {
                if (current_sum + numbers[i] <= target) {
                    int next_sum = (current_sum + numbers[i]) % target;

                    if (dp(mask | (1 << i), next_sum)) {
                        printf("%d ", numbers[i]);

                        if (current_sum + numbers[i] == target) {
                            printf("\n");
                        }

                        print_solution(mask | (1 << i), next_sum);
                        return;
                    }
                }
            }
        }
    }

    void solve(int input_n, int* input_numbers) {
        n = input_n;
        numbers = (int*)malloc(n * sizeof(int));
        memcpy(numbers, input_numbers, n * sizeof(int));
        
        long long total_sum = 0;
        for(int i=0; i<n; ++i) total_sum += numbers[i];
        
        if (total_sum % 3 != 0) {
            printf("no\n");
            free(numbers);
            return;
        }
        target = total_sum / 3;
        
        long long num_states = (1LL << n);
        memo = (int *)malloc(num_states * sizeof(int));
        if (memo == NULL) {
            printf("Memory limit exceeded for this N\n");
            free(numbers);
            return;
        }
        memset(memo, -1, num_states * sizeof(int));
        
        if (dp(0, 0)) {
            printf("yes\n");
            print_solution(0, 0);
        } else {
            printf("no\n");
        }
        
        free(numbers);
        free(memo);
    }
}
