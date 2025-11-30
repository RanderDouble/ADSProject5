#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <functional>

// Forward declarations
namespace Backtracking {
void solve(int n, int* nums);
} // namespace Backtracking
namespace BitmaskDP {
void solve(int n, int* nums);
} // namespace BitmaskDP
namespace DimenDP {
void solve(int n, int* nums);
} // namespace DimenDP

std::string verify(int n, int* original_nums, const std::string& output) {
    std::stringstream ss(output);
    std::string line;
    std::string result_type = "";

    while (std::getline(ss, line)) {
        // Trim whitespace
        size_t first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            continue;
        size_t last = line.find_last_not_of(" \t\r\n");
        line = line.substr(first, (last - first + 1));

        if (line.find("Searching...") != std::string::npos)
            continue;

        if (line == "no")
            return "no";
        if (line == "yes") {
            result_type = "yes";
            break;
        }
    }

    if (result_type == "")
        return "error: no result found";
    if (result_type == "no")
        return "no";

    long long total_sum = 0;
    for (int i = 0; i < n; ++i)
        total_sum += original_nums[i];
    if (total_sum % 3 != 0)
        return "error: input sum not divisible by 3 but output is yes";
    int target = total_sum / 3;

    std::map<int, int> counts;
    for (int i = 0; i < n; ++i)
        counts[original_nums[i]]++;

    for (int i = 0; i < 3; ++i) {
        if (!std::getline(ss, line))
            return "error: missing partition lines";
        std::stringstream pss(line);
        int val;
        long long current_sum = 0;
        while (pss >> val) {
            current_sum += val;
            if (counts[val] > 0) {
                counts[val]--;
            } else {
                return "error: used number not in input or used too many times: " + std::to_string(val);
            }
        }
        if (current_sum != target)
            return "error: partition sum mismatch";
    }

    for (auto const& [val, count] : counts) {
        if (count > 0)
            return "error: not all numbers used";
    }

    return "yes";
}

std::string run_test(const std::string& name, std::function<void(int, int*)> func, int n, int* nums, bool run) {
    std::cout << "--- " << name << " ---" << std::endl;

    if (!run) {
        std::cout << "Skipped" << std::endl;
        std::cout << "Time: -1.000000000 s" << std::endl;
        std::cout << "Result Check: skipped" << std::endl;
        std::cout << std::endl;
        return "skipped";
    }

    // Capture stdout
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    auto start = std::chrono::high_resolution_clock::now();
    func(n, nums);
    auto end = std::chrono::high_resolution_clock::now();

    // Restore stdout
    std::cout.rdbuf(old);

    std::string output = buffer.str();
    std::cout << output; // Print original output

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time: " << std::fixed << std::setprecision(9) << elapsed.count() << " s" << std::endl;

    std::string status = verify(n, nums, output);
    std::cout << "Result Check: " << status << std::endl;
    std::cout << std::endl;

    return status;
}

int main(int argc, char* argv[]) {
    bool run_bt = true;
    bool run_mask = true;
    bool run_dimen = true;

    if (argc >= 4) {
        run_bt = std::stoi(argv[1]);
        run_mask = std::stoi(argv[2]);
        run_dimen = std::stoi(argv[3]);
    }

    int n;
    if (!(std::cin >> n))
        return 0;

    int* nums = new int[n];
    for (int i = 0; i < n; ++i)
        std::cin >> nums[i];

    std::string r1 = run_test("Backtracking", Backtracking::solve, n, nums, run_bt);
    std::string r2 = run_test("Bitmask DP", BitmaskDP::solve, n, nums, run_mask);
    std::string r3 = run_test("Dimen DP", DimenDP::solve, n, nums, run_dimen);

    std::vector<std::pair<std::string, std::string>> results;
    if (run_bt) results.push_back({"BT", r1});
    if (run_mask) results.push_back({"DP", r2});
    if (run_dimen) results.push_back({"Dimen", r3});

    if (results.size() > 1) {
        bool consistent = true;
        std::string first_res = results[0].second;
        for(size_t i=1; i<results.size(); ++i) {
            if (results[i].second != first_res) {
                consistent = false;
                break;
            }
        }
        
        if (!consistent) {
            std::cerr << "!!! INCONSISTENT RESULTS !!!" << std::endl;
            for(auto& p : results) std::cerr << p.first << ": " << p.second << ", ";
            std::cerr << std::endl;
        }
    }

    delete[] nums;
    return 0;
}
