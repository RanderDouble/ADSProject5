#import "@preview/diatypst:0.7.1": *

// 使用 example.typ 中的 "slides.with" 结构进行初始化
#show: slides.with(
  // 基础内容配置 (来自 report.typ 和 example.typ)
  title: "Projects 3: Recover the Design",
  subtitle: "Advanced Data Structure and Algorithm",
  date: "2025.11.03",
  authors: "", // "author" 字段在 report.typ 中为空

  // 风格化选项 (来自 example.typ)
  title-color: blue.darken(50%), // 选用模板示例颜色
  ratio: 16 / 9, // 使用更现代的 16:9 比例
  layout: "medium",
  toc: true, // 自动生成目录
  count: "dot", // 页面角标使用点
  footer: true,
)

// ===================================
// 幻灯片内容开始
// ===================================



= Chapter 1: Algorithm Design - Backtracking

== Core Idea: Sequential Bucket Filling

*Naive Approach vs. Our Strategy*:
Instead of trying to decide which bucket an item belongs to (which leads to $K^N$ states), we invert the decision process.

*Sequential Filling*:
We attempt to completely fill one bucket to the `target_sum` before moving on to the next.

#quote[
  The algorithm focuses on filling the $k$-th bucket using available numbers. Only when `current_bucket_sum == target_sum` do we proceed to bucket $k-1$.
]

*Heuristic Optimization*:
- The input array is sorted in descending order.
- *Rationale*: We prioritize placing larger numbers. They are harder to fit, allowing the algorithm to "fail fast" and prune invalid branches early in the recursion tree.

== Pruning Strategies (Constraint & Symmetry)

To transform the exponential search into an efficient solver, we apply three strict pruning rules:

+ *Capacity Constraint*:
  If `current_sum + numbers[i] > target_sum`, skip the number immediately.
+ *Local Symmetry (Duplicate Pruning)*:
  If `numbers[i] == numbers[i-1]` and the previous number was skipped (`!used[i-1]`), using the current one would produce an identical state. Skip to avoid redundancy.
+ *Strong Pruning (Empty Bucket Failure)*:
  If we fail to fill a *newly opened empty bucket* starting with the largest available number, no solution exists.
  - *Logic*: The largest remaining number *must* go somewhere. If it cannot start the current bucket, it cannot start any subsequent equivalent bucket.

== Complexity Analysis: Backtracking

#columns(2)[
  *Time Complexity*

  - *Worst-case*: $O(K \cdot 2^N)$.
  - *Practical Performance*:
    Due to "Sequential Filling" and aggressive pruning (especially the *Empty Bucket* check), the effective branching factor is drastically reduced.
  - *Scalability*: Runs instantaneously for $N \le 30$.

  #colbreak()

  *Space Complexity*

  - *Auxiliary Space*: $O(N)$.
  - *Components*:
    - `used` array and `bucket_id` array take linear space.
    - Recursive stack depth is bounded by $N$.
  - *Advantage*: Very memory efficient compared to DP.
]

= Chapter 2: Algorithm Design - Bitmask DP

== Core Idea: State Compression

*Problem Modeling*:
We use Top-Down Dynamic Programming with Memoization. Since the order within a bucket doesn't matter, the state is uniquely defined by the subset of used numbers.

*Bitmask Representation*:
A `mask` (integer) represents the set of used numbers:
- If the $i$-th bit is `1`, the number is used.
- If the $i$-th bit is `0`, the number is available.

*State Transition*:
We define `DP(mask)`: Is it possible to partition the *remaining* numbers into valid buckets?
We cache results in a `memo` table to avoid re-calculating the same subproblems.

== Key Optimizations: Implicit State

We reduced the state space from standard $N \cdot 2^N$ to just $2^N$ using mathematical properties:

1. *State Reduction*:
  We do *not* store `current_sum` in the DP state.
  - *Reasoning*: For any given `mask`, the total sum of used numbers is fixed. The current bucket's fill level is deterministically calculated.

2. *Implicit Bucket Switching*:
  We use modulo arithmetic to handle bucket transitions automatically:
  $ "next_sum" = ("current_sum" + "value") mod "target" $
  - If the sum reaches `target`, the modulo operation resets it to `0`, signaling the start of a new bucket.

== Complexity Analysis: Bitmask DP

#columns(2)[
  *Time Complexity*

  - *Analysis*: $O(N \cdot 2^N)$.
  - *Independence from K*:
    Unlike backtracking, the complexity depends purely on $N$. Increasing the number of partitions ($K$) does *not* increase the search depth significantly.

  #colbreak()

  *Space Complexity (The Bottleneck)*

  - *Memory Usage*: $O(2^N)$.
  - *Constraint*:
    - The `memo` array grows exponentially.
    - $N=20$: ~1MB (Fast).
    - $N=30$: Requires Gigabytes (MLE).
  - *Conclusion*:
    DP is superior for small $N$ with complex constraints, but fails for large $N$ due to memory limits.
]

= Chapter 5: Test

== Random sample

#columns(2)[
  *Almost Failed Cases*
  + Pure Random
    - Generates $N$ random integers uniformly distributed in $[1, "max_val"]$. These cases almost *never* have a valid solution.

  + Sum-Divisible Random
    - We tried to generates random numbers and adjusts one number to ensure the total sum is divisible by $K$ to avoid initial pruning.

    #colbreak()
    *Nearly Successed Cases*
  + Guaranteed "Yes" Case
    - Constructs a valid solution by generating $K$ buckets individually.
    - Choose a target sum and then randomly fills each bucket with numbers summing to that target.
    - Shuffled to hide the structure.

  + Mode 3: Near-Miss Case
    - Starts with the case below
    - Increase one number and decrease the other.
    - Tried to cause a worse case
]

== N value
#grid(columns: (1fr, 1.6fr), gutter: 1em)[
  + Bitmask DP:
    - $O(N dot 2^N)$ derived in Chapter 3.

  + Three-dimensional DP:
    - $O(N dot "target"^2)=O(N^3)$ in Chapter 4.
    - fixed `max_val=100`, but as $N$ increases, the `target` grows linearly with $N$.
  + Backtracking:
    - *worst-case* $O(K^N)$ (exponential)
    - Pruning strategies and ordered filling drastically reduce the effective search space in practice.
][
  #align(center + horizon)[
    #figure(image("./full3/performance_analysis.png", width: 100%), caption: "Time vs N, k = 3, max_val = 20")
  ]
]

== Target sum
#grid(columns: (1fr, 1.6fr), gutter: 1em)[
  + Three-dimensional DP:
    - $O(N dot "target"^2)$ complexity. Since `target` is proportional to `max_val`, the time complexity is effectively $O("max_val"^2)$.

  + Bitmask DP and Backtracking:
    Theoretically, they have nothing to do with the size of the target, but as the number grows, so does the calculation time

][
  #align(center + horizon)[
    #figure(image("./maxval/performance_analysis_maxval.png"), caption: "Time vs Target Sum, k = 3, N = 20")
  ]]

== K value

#figure(image("./k/performance_analysis_k.png", height: 55%), caption: "Time vs K, N=15, max_val=100")

We only test Bitmask DP and Backtracking, but because of the limit of the long runtime of Bitmask DP, N must to be small. It turn to be a limit for the K.

For Bitmask DP, the time complexity is $O(N dot 2^N)$, which is independent of $K$. But due to the limit of N, if the K is too large relative to N, obviously there is fewer numbers in each partition, making it is easier to prove that no valid partition exists, as the graph shows above.

#figure(image("./k3/performance_analysis_k.png"), caption: "Time vs K, N = 120, max_val = 100")

For Backtracking, the time complexity is theoretically $O(K^N)$, which means for k, this is an algorithm of polynomial time. However, as K increases, the runtime does not explode as strictly as $K^N$ would suggest. This aligns with our analysis in Chapter 2.

= Thank You!

