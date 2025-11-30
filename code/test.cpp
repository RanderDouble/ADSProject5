#include <iostream>
#include <iomanip>
#include <chrono>

// Forward declarations
namespace Backtracking {
void solve(int n, int* nums);
} // namespace Backtracking
namespace BitmaskDP {
void solve(int n, int* nums);
} // namespace BitmaskDP

int main() {
    int n;
    if (!(std::cin >> n))
        return 0;

    int* nums = new int[n];
    for (int i = 0; i < n; ++i)
        std::cin >> nums[i];

    std::cout << "--- Backtracking ---" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    Backtracking::solve(n, nums);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_bt = end - start;
    std::cout << "Time: " << std::fixed << std::setprecision(9) << elapsed_bt.count() << " s" << std::endl;

    std::cout << "\n--- Bitmask DP ---" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    BitmaskDP::solve(n, nums);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_dp = end - start;
    std::cout << "Time: " << std::fixed << std::setprecision(9) << elapsed_dp.count() << " s" << std::endl;

    delete[] nums;
    return 0;
}
