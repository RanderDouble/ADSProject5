#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Backtracking {
    long long count = 0;
    int *numbers;
    int *bucket_id;
    int bucket_sum[3];
    int target_sum;
    int n;

    int compare(const void *a, const void *b) { return (*(int *)b - *(int *)a); }

    bool backtrack(int index) {
        count++;
        if (count % 10000000 == 0) {
            printf("Searching... Steps: %lld\n", count);
        }
        if (index == n) {
            return (bucket_sum[0] == target_sum && bucket_sum[1] == target_sum && bucket_sum[2] == target_sum);
        }

        for (int i = 0; i < 3; i++) {
            if (bucket_sum[i] + numbers[index] > target_sum) {
                continue;
            }

            if (i > 0 && bucket_sum[i] == bucket_sum[i - 1]) {
                continue;
            }

            bucket_sum[i] += numbers[index];
            bucket_id[index] = i;

            if (backtrack(index + 1)) {
                return true;
            }

            bucket_sum[i] -= numbers[index];
            bucket_id[index] = -1;
        }

        return false;
    }

    void solve(int input_n, int* input_numbers) {
        n = input_n;
        numbers = (int*)malloc(n * sizeof(int));
        memcpy(numbers, input_numbers, n * sizeof(int));
        
        bucket_id = (int*)malloc(n * sizeof(int));
        
        long long total_sum = 0;
        for(int i=0; i<n; ++i) total_sum += numbers[i];
        
        if (total_sum % 3 != 0) {
            printf("no\n");
            free(numbers);
            free(bucket_id);
            return;
        }
        
        target_sum = total_sum / 3;
        bucket_sum[0] = bucket_sum[1] = bucket_sum[2] = 0;
        count = 0;
        
        qsort(numbers, n, sizeof(int), compare);
        
        if (backtrack(0)) {
            printf("yes\n");
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
        
        free(numbers);
        free(bucket_id);
    }
}
