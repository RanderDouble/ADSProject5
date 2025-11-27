#include <iostream>
#include <cstring>
#include <algorithm>
#include <new>

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
                    std::cout << numbers[i] << " ";

                    if (current_sum + numbers[i] == target) {
                        std::cout << std::endl;
                    }

                    print_solution(mask | (1 << i), next_sum);
                    return;
                }
            }
        }
    }
}

void solve(int input_n, int *input_numbers) {
    n = input_n;
    numbers = new int[n];
    std::copy(input_numbers, input_numbers + n, numbers);

    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += numbers[i];

    if (total_sum % 3 != 0) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        return;
    }
    target = total_sum / 3;

    long long num_states = (1LL << n);
    memo = new (std::nothrow) int[num_states];
    if (memo == nullptr) {
        std::cout << "Memory limit exceeded for this N" << std::endl;
        delete[] numbers;
        return;
    }
    std::memset(memo, -1, num_states * sizeof(int));

    if (dp(0, 0)) {
        std::cout << "yes" << std::endl;
        print_solution(0, 0);
    } else {
        std::cout << "no" << std::endl;
    }

    delete[] numbers;
    delete[] memo;
}
} // namespace BitmaskDP
