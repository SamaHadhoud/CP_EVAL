Restatement

We work in base 10. Call an integer “sorted” if its decimal digits are non-decreasing from left to right. For example, 1223 is sorted, while 132 and 120 are not. For each query X (up to 10^18), we need to count how many integers n in [1, X] are such that both n and n^2 are sorted in base 10.

High-level plan

- There are many n with non-decreasing digits, but among them very few have n^2 non-decreasing as well. We will precompute all such n ≤ 10^18 once, then answer each query by binary searching how many precomputed values are ≤ X.
- The precomputation exploits a crucial “suffix invariance” property of squaring modulo powers of 10 to prune the search drastically.

Key observations

1) What “sorted” means numerically
- A number a1 a2 … ak (digits from most significant to least) is sorted iff a1 ≤ a2 ≤ … ≤ ak.
- Equivalently, if you read the digits of the number from least significant to most significant, they must be non-increasing.

2) No zeros in a sorted positive integer
- Since the first digit must be ≥ 1 and digits are non-decreasing left to right, every digit is ≥ the first digit. Hence all digits are in {1, 2, …, 9}. Zeros never appear.

3) Suffix invariance for squares
- For any L ≥ 1, (n^2) mod 10^L depends only on n mod 10^L.
- Therefore, once we have fixed the last L digits of n, the last L digits of n^2 are fully determined and will not change if we later add more digits to the left.

This is the pruning lever: if the last L digits of n^2 (read from least significant to most) are not non-increasing, then no extension with more digits on the left can fix it. We can stop exploring that branch.

How to generate candidates

We must generate all n with non-decreasing digits and check n^2. A direct enumeration of all such n up to 10^18 is about 6.9 million numbers (sum_{k=1..19} C(8+k, 8) = C(28, 9) − 1 = 6,906,899), which is borderline but feasible in optimized languages. However, using the suffix pruning above slashes the search.

The right-to-left construction is the correct direction:
- Build the number from the least significant digit upwards (i.e., from right to left).
- Because the digits are non-decreasing left to right, when you build right to left, the digits must be non-increasing: each newly added more-significant digit must be ≤ the last placed digit.
- Let the current suffix of length L (the L least significant digits) be S. Its numeric value is sVal. Then S^2 modulo 10^L is the actual last L digits of n^2 for any completion to the left.

At every step we can:
- Check that the last L digits of S^2 (read from LSD to MSD) are non-increasing.
- If not, prune the branch (no completion will help).
- If yes, S itself is a complete number with non-decreasing digits. We then check the entire square S^2 (not just the last L digits). If that passes, we record S as a “good” number.
- Try to extend to the left by adding a new digit d in [1 .. lastDigit], where lastDigit is the least significant digit of the current suffix (ensures non-increasing when building right-to-left, i.e., non-decreasing left-to-right).

This way the suffix check is always relevant (and safe) because the last L digits of the final n are exactly the suffix we have already built.

Digit checks for the square

To test whether a number Q (either S^2 modulo 10^L or the full S^2) has non-decreasing digits left-to-right:
- Equivalently, scan its digits from least significant to most significant and ensure they are non-increasing.
- Do it arithmetically (no strings): keep prev = 10; while digits remain: cur = Q % 10; if cur > prev → not sorted; prev = cur; Q /= 10.

Putting it together (pseudocode)

- Precomputation (done once):
  - good = empty list
  - pow10[0] = 1; for i = 1..18: pow10[i] = pow10[i-1] * 10
  - DFS(lastDigit, sVal, L):
    - Tail check (necessary condition on the last L digits of square):
      - Let T = sVal * sVal (use 128-bit to avoid overflow).
      - Repeat L times:
        - cur = T % 10; if cur > prev return (prune); prev = cur; T /= 10
    - Full square check for the current complete number sVal:
      - Continue the same loop with the remaining T:
        - while T > 0:
          - cur = T % 10; if cur > prev: fail; prev = cur; T /= 10
      - If not failed, push sVal into good.
    - Try to extend to the left (if within bounds):
      - If L == 19 stop (10^19 would exceed the numeric limit).
      - For d in [1 .. lastDigit]:
        - newVal = d * pow10[L] + sVal
        - If newVal ≤ 10^18: DFS(d, newVal, L+1)
  - Start DFS with each possible unit digit:
    - For d in 1..9: DFS(d, d, 1)
  - Sort(good) (DFS order is not strictly numeric order).

- Answering queries:
  - For each X: output count of values in good that are ≤ X (upper_bound).

Why this is correct

- Generation covers exactly the sorted numbers:
  - Building from right to left with each new digit ≤ the previous ensures digits are non-increasing right-to-left, i.e., non-decreasing left-to-right, and we disallow 0 so the first digit ≥ 1. This enumerates all and only sorted positive integers.
- Pruning is safe:
  - The last L digits of n^2 only depend on the last L digits of n (suffix invariance). If those L digits already violate the non-increasing condition (LSD to MSD), no extension on the left can fix it.
- Acceptance is correct:
  - We insert sVal into good only after verifying the entire square sVal^2 has non-increasing digits right-to-left (equivalently, non-decreasing left-to-right).
- Queries are correct:
  - good contains exactly the n ≤ 10^18 we want. upper_bound counts how many are ≤ X.

Complexity and feasibility

- Count of sorted numbers up to 19 digits (no zeros) is sum_{k=1..19} C(8 + k, 8) = C(28, 9) − 1 = 6,906,899. This is an absolute upper bound on DFS nodes; in practice, the suffix check prunes very early for most branches, so far fewer nodes are explored deeply.
- Per node we do:
  - One 128-bit multiplication for sVal^2.
  - A small number of digit operations (up to L for the tail check, and for those that pass we continue a few more steps; average constant).
- Memory:
  - The list good is tiny compared to RAM (on the order of at most hundreds of thousands of 64-bit integers; in practice typically much less).
- Answering T ≤ 1e5 queries:
  - Each query is answered in O(log |good|) via binary search; with |good| small, this is extremely fast.

Practical tips

- Avoid strings; use arithmetic digit extraction.
- Use 128-bit integer for squaring (n ≤ 10^18 ⇒ n^2 ≤ 10^36).
- It’s safe and fast to keep a precomputed array of powers of 10 up to 10^18.
- Optional micro-optimization: you can pre-filter at length L=2 via a tiny 100-entry table that says whether (x^2 mod 100) has tens ≤ ones; that catches many bad endings early. But the general suffix check already subsumes this.

Sample sanity check

- For X = 5:
  - 1..5 are all sorted; their squares are 1, 4, 9, 16, 25 — all sorted → answer 5.
- For X = 8:
  - 6 → 36 (sorted), 7 → 49 (sorted), 8 → 64 (not sorted) → total 7.
- For X = 13:
  - 12 → 144 (sorted), 13 → 169 (sorted) → total 9.

Why the time limit is met

- The pruning by suffix-invariance is very effective: as soon as the last few digits of the square are “out of order,” the whole branch is cut. Most branches die at small L. The total number of nodes that survive to large L is tiny, so the overall precomputation time is well within 1 second in an optimized implementation. Query processing is trivial.

Summary

- Precompute, once, all n ≤ 10^18 whose digits are non-decreasing and whose square’s digits are non-decreasing by a right-to-left DFS with suffix-based pruning on n^2.
- Store them in a sorted array. For each test X, binary search to count how many precomputed values are ≤ X.
- Correctness follows from the equivalence “sorted ⇔ digits non-increasing from LSD to MSD” and the invariance of the last L digits of n^2 with respect to the last L digits of n, which justifies the pruning. The limits are comfortably satisfied.