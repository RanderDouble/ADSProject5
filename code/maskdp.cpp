#include <iostream>
#include <cstring>
#include <algorithm>
#include <new>

// Define the number of partitions (buckets)
#ifndef K
#define K 3
#endif

namespace BitmaskDP {
int n;
int *numbers;
int target;
int *memo; // Memoization table to store results of states
int k_partitions;

// Helper function to check if the i-th bit is set in the mask
bool is_set(int mask, int i) { return (mask & (1 << i)) != 0; }

/**
 * Recursive Function with Memoization (Top-Down DP)
 * 
 * @param mask Integer representing the set of numbers already used (1 = used, 0 = unused).
 * @param current_sum The sum of the bucket currently being filled.
 * @return True if the remaining numbers can form valid partitions.
 */
bool dp(int mask, int current_sum) {
    // Base Case: If the mask is all 1s, all numbers have been used successfully.
    // Because of the logic below, this implies K buckets of 'target' sum were formed.
    if (mask == (1 << n) - 1) {
        return true;
    }

    // Check Memoization Table
    if (memo[mask] != -1) {
        return memo[mask];
    }

    bool possible = false;

    // Iterate through all numbers to find the next candidate
    for (int i = 0; i < n; i++) {
        // If number i is not yet used
        if (!is_set(mask, i)) {
            // Check if adding this number exceeds the current bucket's target
            if (current_sum + numbers[i] <= target) {
                // Calculate the new sum state.
                // Modulo Arithmetic Trick:
                // If (current_sum + number) equals 'target', the result is 0.
                // This implicitly means the current bucket is finished, and we start
                // filling a new bucket from 0 sum with the remaining numbers.
                int next_sum = (current_sum + numbers[i]) % target;

                // Recursive call with updated mask and sum
                if (dp(mask | (1 << i), next_sum)) {
                    possible = true;
                    break; // Found a valid path, no need to check other numbers for this state
                }
            }
        }
    }

    // Store result in memo table (1 for true, 0 for false) and return
    return memo[mask] = (possible ? 1 : 0);
}

/**
 * Helper function to reconstruct and print the solution
 * Re-runs the logic of the DP to find the path taken.
 */
void print_solution(int mask, int current_sum) {
    if (mask == (1 << n) - 1)
        return;

    for (int i = 0; i < n; i++) {
        if (!is_set(mask, i)) {
            if (current_sum + numbers[i] <= target) {
                int next_sum = (current_sum + numbers[i]) % target;

                // Check if this path leads to a valid solution
                if (dp(mask | (1 << i), next_sum)) {
                    std::cout << numbers[i] << (next_sum == 0 ? "" : " ");
                    if (next_sum == 0)
                        std::cout << std::endl;

                    // Continue printing the rest
                    print_solution(mask | (1 << i), next_sum);
                    return;
                }
            }
        }
    }
}

/**
 * Main Solver Interface
 * @param input_n The number of elements.
 * @param input_numbers Pointer to the array of numbers.
 * @param k The number of partitions.
 */
void solve(int input_n, int *input_numbers, int k) {
    n = input_n;
    k_partitions = k;
    numbers = new int[n];
    std::copy(input_numbers, input_numbers + n, numbers);

    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += numbers[i];

    // Validation: Total sum must be divisible by K
    if (total_sum % k_partitions != 0) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        return;
    }

    // Determine the target sum for each partition
    target = total_sum / k_partitions;

    // Optimization: If any single number exceeds the target, solution is impossible
    for (int i = 0; i < n; i++) {
        if (numbers[i] > target) {
            std::cout << "no" << std::endl;
            delete[] numbers;
            return;
        }
    }

    // Allocate memory for the DP table
    // Size is 2^n states
    long long num_states = (1LL << n);
    memo = new (std::nothrow) int[num_states];

    // Check for memory allocation failure
    if (memo == nullptr) {
        std::cerr << "Memory limit exceeded" << std::endl;
        delete[] numbers;
        return;
    }

    // Initialize memo table with -1 (representing unvisited states)
    std::memset(memo, -1, num_states * sizeof(int));

    // Start DP from empty mask (0) and 0 current sum
    if (dp(0, 0)) {
        std::cout << "yes" << std::endl;
        print_solution(0, 0);
    } else {
        std::cout << "no" << std::endl;
    }

    // Cleanup
    delete[] numbers;
    delete[] memo;
}
} // namespace BitmaskDP