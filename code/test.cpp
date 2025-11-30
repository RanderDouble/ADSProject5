#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <functional>
#include <string>

// Forward declarations
namespace Backtracking {
void solve(int n, int* nums, int k);
} // namespace Backtracking
namespace BacktrackingNoPrune {
void solve(int n, int* nums, int k);
} // namespace BacktrackingNoPrune
namespace BitmaskDP {
void solve(int n, int* nums, int k);
} // namespace BitmaskDP
namespace DimenDP {
void solve(int n, int* nums, int k);
} // namespace DimenDP

void run_test(const std::string& name, std::function<void(int, int*, int)> func, int n, int* nums, int k, bool run) {
    std::cout << "--- " << name << " ---" << std::endl;

    if (!run) {
        std::cout << "Skipped" << std::endl;
        std::cout << "Time: -1.000000000 s" << std::endl;
        std::cout << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();
    func(n, nums, k);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time: " << std::fixed << std::setprecision(9) << elapsed.count() << " s" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    bool run_bt = true;
    bool run_mask = true;
    bool run_dimen = true;
    bool run_bt_noprune = true;
    int k = 3;

    if (argc >= 6) {
        run_bt = std::stoi(argv[1]);
        run_mask = std::stoi(argv[2]);
        run_dimen = std::stoi(argv[3]);
        run_bt_noprune = std::stoi(argv[4]);
        k = std::stoi(argv[5]);
    } else if (argc >= 5) {
        run_bt = std::stoi(argv[1]);
        run_mask = std::stoi(argv[2]);
        run_dimen = std::stoi(argv[3]);
        run_bt_noprune = std::stoi(argv[4]);
    } else if (argc >= 4) {
        run_bt = std::stoi(argv[1]);
        run_mask = std::stoi(argv[2]);
        run_dimen = std::stoi(argv[3]);
        run_bt_noprune = false;
    }

    int n;
    if (!(std::cin >> n))
        return 0;

    int* nums = new int[n];
    for (int i = 0; i < n; ++i)
        std::cin >> nums[i];

    run_test("Backtracking", Backtracking::solve, n, nums, k, run_bt);
    run_test("Bitmask DP", BitmaskDP::solve, n, nums, k, run_mask);
    run_test("Dimen DP", DimenDP::solve, n, nums, k, run_dimen);
    run_test("Backtracking (No Prune)", BacktrackingNoPrune::solve, n, nums, k, run_bt_noprune);

    delete[] nums;
    return 0;
}
