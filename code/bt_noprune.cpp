#include <iostream>
#include <algorithm>
#include <cstring>

namespace BacktrackingNoPrune {
long long count = 0;
int *numbers;
int *bucket_id;
int bucket_sum[3];
int target_sum;
int n;

bool compare(int a, int b) { return a > b; }

bool backtrack(int index) {
    count++;
    if (count % 10000000 == 0) {
        std::cout << "Searching... Steps: " << count << std::endl;
    }
    if (index == n) {
        return (bucket_sum[0] == target_sum && bucket_sum[1] == target_sum && bucket_sum[2] == target_sum);
    }

    for (int i = 0; i < 3; i++) {
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

void solve(int input_n, int *input_numbers) {
    n = input_n;
    numbers = new int[n];
    std::copy(input_numbers, input_numbers + n, numbers);

    bucket_id = new int[n];

    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += numbers[i];

    if (total_sum % 3 != 0) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        delete[] bucket_id;
        return;
    }

    target_sum = total_sum / 3;
    bucket_sum[0] = bucket_sum[1] = bucket_sum[2] = 0;
    count = 0;

    std::sort(numbers, numbers + n, compare);

    if (backtrack(0)) {
        std::cout << "yes" << std::endl;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < n; j++) {
                if (bucket_id[j] == i) {
                    std::cout << numbers[j] << " ";
                }
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "no" << std::endl;
    }

    delete[] numbers;
    delete[] bucket_id;
}
} // namespace BacktrackingNoPrune
