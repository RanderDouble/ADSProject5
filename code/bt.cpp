#include <iostream>
#include <algorithm>
#include <cstring>

// Define the number of partitions (buckets)
#ifndef K
#define K 3
#endif

namespace Backtracking {

// Global variables for recursion state
int *numbers;     // Array to store the input numbers
int *bucket_id;   // Array to track which bucket a number belongs to (for printing)
int *bucket_sum;  // Current sum of each of the K buckets
int target_sum;   // The target sum for every bucket
int n;            // Total count of numbers
int k_partitions; // Number of partitions

// Comparator for sorting numbers in descending order (Heuristic optimization)
bool compare(int a, int b) { return a > b; }

/**
 * Recursive Backtracking Function
 * @param index The index of the current number we are trying to place.
 * @return True if a valid partition is found, False otherwise.
 */
bool backtrack(int index) {
    // Base Case: All numbers have been successfully placed.
    // Since we ensure no bucket exceeds target_sum during placement,
    // and total_sum % K == 0, if we reach here, all buckets must equal target_sum.
    if (index == n) {
        return true;
    }

    // Try placing the current number (numbers[index]) into each of the K buckets
    for (int i = 0; i < k_partitions; i++) {
        // Pruning 1: Capacity Check
        // If adding this number exceeds the target sum, this bucket is invalid.
        if (bucket_sum[i] + numbers[index] > target_sum) {
            continue;
        }

        // Pruning 2: Symmetry Breaking (Crucial optimization)
        // If the current bucket has the same sum as the previous bucket,
        // placing the number here is mathematically equivalent to placing it
        // in the previous bucket (which was already tried or skipped).
        // This avoids searching permutations of identical buckets.
        if (i > 0 && bucket_sum[i] == bucket_sum[i - 1]) {
            continue;
        }

        // Action: Place number in bucket i
        bucket_sum[i] += numbers[index];
        bucket_id[index] = i;

        // Recurse: Try to place the next number
        if (backtrack(index + 1)) {
            return true;
        }

        // Backtrack: Remove number from bucket i and try the next bucket
        bucket_sum[i] -= numbers[index];
        bucket_id[index] = -1;
    }

    return false;
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

    // Copy input to internal array
    numbers = new int[n];
    std::copy(input_numbers, input_numbers + n, numbers);

    bucket_id = new int[n];
    bucket_sum = new int[k_partitions];

    // Calculate total sum
    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += numbers[i];

    // Validation: The total sum must be divisible by K
    if (total_sum % k_partitions != 0) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        delete[] bucket_id;
        delete[] bucket_sum;
        return;
    }

    target_sum = total_sum / k_partitions;

    // Reset bucket sums
    for (int i = 0; i < k_partitions; ++i) {
        bucket_sum[i] = 0;
    }

    // Optimization: Sort numbers in descending order.
    // Placing larger numbers first reduces the branching factor early in the tree.
    std::sort(numbers, numbers + n, compare);

    // Immediate failure check: If the largest number is greater than the target, impossible.
    if (n > 0 && numbers[0] > target_sum) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        delete[] bucket_id;
        delete[] bucket_sum;
        return;
    }

    // Start recursion
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

    // Clean up dynamic memory
    delete[] numbers;
    delete[] bucket_id;
    delete[] bucket_sum;
}
} // namespace Backtracking