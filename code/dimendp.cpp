#include <iostream>
#include <vector>
#include <numeric>
#include <cstring>
#include <algorithm>

namespace DimenDP {

void solve(int n, int* input_numbers, int k) {
    if (k != 3) {
        std::cout << "no" << std::endl; // DimenDP only supports K=3
        return;
    }
    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += input_numbers[i];

    if (total_sum % 3 != 0) {
        std::cout << "no" << std::endl;
        return;
    }

    int target = total_sum / 3;
    for (int i = 0; i < n; ++i) {
        if (input_numbers[i] > target) {
            std::cout << "no" << std::endl;
            return;
        }
    }

    // dp[i][j][k] represents if it's possible to fill partition 1 to sum 'i'
    // and partition 2 to sum 'j' using the first 'k' items.
    // Partition 3 is implicitly determined by the remaining items.
    char*** dp = new char**[target + 1];
    for (int i = 0; i <= target; ++i) {
        dp[i] = new char*[target + 1];
        for (int j = 0; j <= target; ++j) {
            dp[i][j] = new char[n + 1];
            std::memset(dp[i][j], 0, (n + 1) * sizeof(char));
        }
    }

    dp[0][0][0] = 1;

    for (int k = 1; k <= n; ++k) {
        int val = input_numbers[k - 1];
        for (int i = 0; i <= target; ++i) {
            for (int j = 0; j <= target; ++j) {
                // Option 1: Put current item in Partition 3 (skip for i and j)
                if (dp[i][j][k - 1]) {
                    dp[i][j][k] = 1;
                }
                // Option 2: Put current item in Partition 1
                else if (i >= val && dp[i - val][j][k - 1]) {
                    dp[i][j][k] = 1;
                }
                // Option 3: Put current item in Partition 2
                else if (j >= val && dp[i][j - val][k - 1]) {
                    dp[i][j][k] = 1;
                }
            }
        }
    }

    if (dp[target][target][n]) {
        std::cout << "yes" << std::endl;
        std::vector<int> partitions[3];

        int i = target, j = target;

        for (int k = n; k >= 1; --k) {
            int val = input_numbers[k - 1];

            // Check if we came from Option 2 (Partition 1)
            if (i >= val && dp[i - val][j][k - 1]) {
                partitions[0].push_back(val);
                i -= val;
            }
            // Check if we came from Option 3 (Partition 2)
            else if (j >= val && dp[i][j - val][k - 1]) {
                partitions[1].push_back(val);
                j -= val;
            }
            // Must have come from Option 1 (Partition 3)
            else {
                partitions[2].push_back(val);
            }
        }

        for (int p = 0; p < 3; ++p) {
            for (size_t idx = 0; idx < partitions[p].size(); ++idx) {
                std::cout << partitions[p][idx] << (idx == partitions[p].size() - 1 ? "" : " ");
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "no" << std::endl;
    }

    for (int i = 0; i <= target; ++i) {
        for (int j = 0; j <= target; ++j) {
            delete[] dp[i][j];
        }
        delete[] dp[i];
    }
    delete[] dp;
}
} // namespace DimenDP
