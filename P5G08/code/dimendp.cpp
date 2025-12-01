#include <iostream>
#include <vector>
#include <numeric>
#include <cstring>
#include <algorithm>

namespace DimenDP {

void solve(int n, int* input_numbers, int k) {
    //prune before dp
    if (k != 3) {
        std::cout << "no" << std::endl; // DimenDP only supports K=3
        return;
    }
    //the sum must be divided by 3
    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += input_numbers[i];
    if (total_sum % 3 != 0) {
        std::cout << "no" << std::endl;
        return;
    }
    //subset: target
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
    // We just need to check dp[target][target][n]
    char*** dp = new char**[target + 1];
    for (int i = 0; i <= target; ++i) {
        dp[i] = new char*[target + 1];
        for (int j = 0; j <= target; ++j) {
            dp[i][j] = new char[n + 1];
            std::memset(dp[i][j], 0, (n + 1) * sizeof(char));
        }
    }
    //initialize the state
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
        int partitions[3][100];
        int part_1=0,part_2=0,part_3=0;
        int i = target, j = target;
        //resverse the dynamic programming
        for (int k = n; k >= 1; --k) {
            //every time choose the last element
            int val = input_numbers[k - 1];
            
            // if dp[i-val][j][k-1]==1 then the number can be in subset 1
            if (i >= val && dp[i - val][j][k - 1]) {
                partitions[0][part_1++]=val;
                i -= val;
            }
            // if dp[i][j-val][k-1]==1 then the number can be in subset 2
            else if (j >= val && dp[i][j - val][k - 1]) {
                partitions[1][part_2++]=val;
                j -= val;
            }
            // otherwise,must have come from subset 3
            else {
                partitions[2][part_3++]=val;
            }
        }
        //print the result
        for(size_t idx=0;idx<part_1;idx++){
            std::cout << partitions[0][idx] << (idx == part_1 - 1 ? "" : " ");
        }
        std::cout<<std::endl;
        for(size_t idx=0;idx<part_2;idx++){
            std::cout << partitions[1][idx] << (idx == part_2 - 1 ? "" : " ");
        }
        std::cout<<std::endl;
        for(size_t idx=0;idx<part_3;idx++){
            std::cout << partitions[2][idx] << (idx == part_3 - 1 ? "" : " ");
        }
        std::cout<<std::endl;
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
