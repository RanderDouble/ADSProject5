#include <iostream>
#include <algorithm>
#include <cstring>

namespace BacktrackingNoPrune {
long long count = 0;
int *numbers;
int *bucket_id;
int *bucket_sum;
int target_sum;
int n;
int k_partitions;

bool compare(int a, int b) { return a > b; }

bool backtrack(int index) {
    count++;
    // Removed progress print to avoid cluttering stdout/result
    if (index == n) {
        // Check if all buckets are full
        for (int i = 0; i < k_partitions; ++i) {
            if (bucket_sum[i] != target_sum)
                return false;
        }
        return true;
    }

    for (int i = 0; i < k_partitions; i++) {
        if (bucket_sum[i] + numbers[index] > target_sum) {
            continue;
        }

        // Pruning removed here
        // if (i > 0 && bucket_sum[i] == bucket_sum[i - 1]) {
        //     continue;
        // }

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

void solve(int input_n, int *input_numbers, int k) {
    n = input_n;
    k_partitions = k;
    numbers = new int[n];
    std::copy(input_numbers, input_numbers + n, numbers);

    bucket_id = new int[n];
    bucket_sum = new int[k_partitions];

    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += numbers[i];

    if (total_sum % k_partitions != 0) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        delete[] bucket_id;
        delete[] bucket_sum;
        return;
    }

    target_sum = total_sum / k_partitions;
    for (int i = 0; i < k_partitions; ++i)
        bucket_sum[i] = 0;
    count = 0;

    std::sort(numbers, numbers + n, compare);

    if (backtrack(0)) {
        std::cout << "yes" << std::endl;
        for (int i = 0; i < k_partitions; i++) {
            bool first = true;
            for (int j = 0; j < n; j++) {
                if (bucket_id[j] == i) {
                    if (!first)
                        std::cout << " ";
                    std::cout << numbers[j];
                    first = false;
                }
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "no" << std::endl;
    }

    delete[] numbers;
    delete[] bucket_id;
    delete[] bucket_sum;
}
} // namespace BacktrackingNoPrune
