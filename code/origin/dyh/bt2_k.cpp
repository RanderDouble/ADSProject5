#include <iostream>
#include <algorithm>
#include <cstring>

// Define the number of partitions (buckets)
#ifndef K
#define K 3
#endif

namespace Backtracking {

// Global variables for recursion state
int *numbers;       // Array to store the input numbers
bool *used;         // Array to mark if a number has been used
int *bucket_id;     // Records which bucket a number belongs to (k = K, K-1, ..., 1)
int target_sum;     // The target sum for every bucket
int n;              // Total count of numbers

// Comparator for sorting numbers in descending order (Heuristic optimization)
bool compare(int a, int b) { return a > b; }

/**
 * Recursive Backtracking Function
 * Logic Source: bt2.cpp (Sequential filling)
 * 
 * @param k Current bucket we are trying to fill (counts down from K to 1)
 * @param current_bucket_sum Current sum of the bucket k
 * @param start_index Index to start searching from to avoid duplicates/permutations
 * @return True if a valid partition is found, False otherwise.
 */
bool backtrack(int k, int current_bucket_sum, int start_index) {
    
    // [Logic from bt2.cpp]: If it is the last bucket (k==1), no need to recurse.
    // Since the previous (K-1) buckets are filled and the total sum is divisible by K,
    // the remaining numbers must inevitably sum up to the target.
    // We just need to mark all unused numbers as belonging to bucket 1.
    if (k == 1) {
        for (int i = 0; i < n; i++) {
            if (!used[i]) {
                bucket_id[i] = 1; // Assign remaining to the last bucket (logical bucket 1)
            }
        }
        return true;
    }

    // If the current bucket is full, start filling the next bucket (k-1)
    // Reset current_sum to 0 and start_index to 0.
    if (current_bucket_sum == target_sum) {
        return backtrack(k - 1, 0, 0);
    }

    for (int i = start_index; i < n; i++) {
        if (used[i]) continue; // Already used by another bucket

        // Pruning 1: Exceeds target sum
        if (current_bucket_sum + numbers[i] > target_sum) continue;

        // Pruning 2: Handle duplicates to avoid symmetric searches (Symmetry Breaking)
        // If current number is the same as the previous one and the previous one 
        // was not used (skipped), then using this one is redundant.
        if (i > start_index && numbers[i] == numbers[i - 1] && !used[i - 1]) continue;

        // --- Action ---
        used[i] = true;
        bucket_id[i] = k; // Record: this number belongs to logical bucket k

        // --- Recurse ---
        if (backtrack(k, current_bucket_sum + numbers[i], i + 1)) {
            return true;
        }

        // --- Backtrack (Undo selection) ---
        used[i] = false;
        bucket_id[i] = 0; // Clear record

        // Pruning 3 (Strong Pruning from bt2.cpp): 
        // If we fail to fill the bucket even when picking the first available number 
        // (which is the largest available due to sorting), then no solution exists 
        // for this configuration. This is because this largest number MUST belong 
        // to some bucket, and if it can't fit in the current empty bucket, 
        // it won't fit anywhere else equivalent.
        if (current_bucket_sum == 0) return false;
    }

    return false;
}

/**
 * Main Solver Interface
 * Framework: btk.cpp
 * @param input_n The number of elements.
 * @param input_numbers Pointer to the array of numbers.
 */
void solve(int input_n, int *input_numbers) {
    n = input_n;

    // Allocate memory
    numbers = new int[n];
    std::copy(input_numbers, input_numbers + n, numbers);

    used = new bool[n];
    std::fill(used, used + n, false);

    bucket_id = new int[n];
    std::fill(bucket_id, bucket_id + n, 0);

    // Calculate total sum
    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += numbers[i];

    // Validation: The total sum must be divisible by K
    if (total_sum % K != 0) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        delete[] used;
        delete[] bucket_id;
        return;
    }

    target_sum = total_sum / K;

    // Optimization: Sort numbers in descending order.
    std::sort(numbers, numbers + n, compare);

    // Immediate failure check: If the largest number is greater than the target, impossible.
    if (n > 0 && numbers[0] > target_sum) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        delete[] used;
        delete[] bucket_id;
        return;
    }

    // Start recursion: Try to fill K buckets sequentially.
    // We start from k = K down to 1.
    if (backtrack(K, 0, 0)) {
        std::cout << "yes" << std::endl;
        
        // --- Printing Result ---
        // Iterate to print buckets.
        // The algorithm fills logical buckets K, K-1, ..., 1.
        // To match btk.cpp output style (Bucket 0, Bucket 1...), we map:
        // Logical bucket K -> Display Bucket 0
        // Logical bucket K-1 -> Display Bucket 1
        for (int k = K; k >= 1; k--) {
            std::cout << "Bucket " << (K - k) << ": "; 
            for (int i = 0; i < n; i++) {
                if (bucket_id[i] == k) {
                    std::cout << numbers[i] << " ";
                }
            }
            std::cout << std::endl;
        }

    } else {
        std::cout << "no" << std::endl;
    }

    // Clean up dynamic memory
    delete[] numbers;
    delete[] used;
    delete[] bucket_id;
}
} // namespace Backtracking