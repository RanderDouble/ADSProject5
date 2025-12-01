#include <iostream>
#include <algorithm>
#include <cstring>

namespace Backtracking {

// Global variables for recursion state
int *numbers;       // Array to store the input numbers
bool *used;         // Array to mark if a number has been used
int *bucket_id;     // Records which bucket a number belongs to (k = 3, 2, 1)
int target_sum;     // The target sum for every bucket
int n;              // Total count of numbers

// Comparator for sorting numbers in descending order
bool compare(int a, int b) { return a > b; }

/**
 * Recursive Backtracking Function
 * @param k Current bucket we are trying to fill (counts down from 3 to 1)
 * @param current_bucket_sum Current sum of the bucket k
 * @param start_index Index to start searching from to avoid duplicates
 * @return True if a valid partition is found, False otherwise.
 */
bool backtrack(int k, int current_bucket_sum, int start_index) {
    
    // [Modification 1]: If it is the last bucket (k==1), no need to recurse.
    // Since the first two buckets are filled and the total sum is divisible by 3,
    // the remaining numbers must inevitably sum up to the target.
    // We just need to mark all unused numbers as belonging to bucket 1.
    if (k == 1) {
        for (int i = 0; i < n; i++) {
            if (!used[i]) {
                bucket_id[i] = 1; // Assign remaining to bucket 1
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

        // Pruning: Exceeds target sum
        if (current_bucket_sum + numbers[i] > target_sum) continue;

        // Pruning: Handle duplicates to avoid symmetric searches
        // If current number is the same as the previous one and the previous one 
        // was not used (skipped), then using this one is redundant.
        if (i > start_index && numbers[i] == numbers[i - 1] && !used[i - 1]) continue;

        // --- Action ---
        used[i] = true;
        bucket_id[i] = k; // Record: this number belongs to bucket k

        // --- Recurse ---
        if (backtrack(k, current_bucket_sum + numbers[i], i + 1)) {
            return true;
        }

        // --- Backtrack (Undo selection) ---
        used[i] = false;
        bucket_id[i] = 0; // Clear record

        // Strong Pruning: If we fail to fill the bucket even when picking 
        // the first available number (which is the largest available due to sorting),
        // then no solution exists for this configuration.
        if (current_bucket_sum == 0) return false;
    }

    return false;
}

/**
 * Main Solver Interface
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

    // Validation: The total sum must be divisible by 3
    if (total_sum % 3 != 0) {
        std::cout << "no" << std::endl;
        delete[] numbers;
        delete[] used;
        delete[] bucket_id;
        return;
    }

    target_sum = total_sum / 3;

    // Optimization: Sort numbers in descending order.
    std::sort(numbers, numbers + n, compare);

    // Start recursion: Try to fill 3 buckets.
    // We start from k=3 down to 1.
    if (backtrack(3, 0, 0)) {
        std::cout << "yes" << std::endl;
        
        // --- Printing Result Logic ---
        // Iterate bucket_id to print.
        // Note: The algorithm fills k as 3, 2, 1.
        for (int k = 3; k >= 1; k--) {
            // Map 3->1, 2->2, 1->3 for display purposes (optional, keeps it "Bucket 1, 2, 3")
            // Or strictly follow bt2.c logic which implies printing in the order they were filled or ID order.
            // Here we print based on the ID stored: 3, 2, 1.
            std::cout << "Bucket " << (4 - k) << ": "; 
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