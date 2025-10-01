Restatement

You are given N axis-aligned rectangular lands with sizes Li × Wi. You must build exactly two identical rectangular buildings of size A × B (with real A, B allowed), and each building must be placed axis-parallel on a land.

Two placement options:
- Place both buildings on the same land with the same orientation (side-by-side along one axis).
- Place one building on each of two lands (each building may be rotated independently to fit its land).

Goal: maximize the area A × B. Output the maximum possible area with exactly one decimal place.

Key observations

1) Normalize each land
For every land, sort its sides:
- Pi = max(Li, Wi)  (the longer side)
- Qi = min(Li, Wi)  (the shorter side)

This is convenient because any rectangle A × B (let a = max(A,B), b = min(A,B)) can be placed on a land iff a ≤ Pi and b ≤ Qi.

2) Two buildings on the same land
The constraints say you can put two A × B with the same orientation if either:
- 2A ≤ Pi and B ≤ Qi, or
- A ≤ Pi and 2B ≤ Qi (or swapped L/W, which is the same after sorting).

To maximize area, you make the “doubled” dimension tight and the other as large as possible:
- Using 2A ≤ Pi and B ≤ Qi gives area (Pi/2) × Qi = (Pi × Qi)/2.
- Using A ≤ Pi and 2B ≤ Qi gives area Pi × (Qi/2) = (Pi × Qi)/2.

Therefore, on a single land i, the best you can do is exactly half of its area: (Li × Wi)/2. So across all lands, the best “single-land” value is:
S1 = max over i of (Li × Wi)/2.

3) One building per land (two lands)
Let a = max(A,B), b = min(A,B). For a land with (P, Q), the fit condition is a ≤ P and b ≤ Q. For two lands i and j, the rectangle must satisfy:
- a ≤ min(Pi, Pj)
- b ≤ min(Qi, Qj)
Since Qi ≤ Pi and Qj ≤ Pj, it follows that min(Qi, Qj) ≤ min(Pi, Pj). Thus we can set:
- a = min(Pi, Pj)
- b = min(Qi, Qj)
This respects a ≥ b and saturates both limits, giving area:
S(i, j) = min(Pi, Pj) × min(Qi, Qj).

So the best “two-lands” value is:
S2 = max over i < j of min(Pi, Pj) × min(Qi, Qj).

Efficient computation of S2 (no brute force)

Directly checking all pairs is O(N^2), too slow for N up to 100,000. Use a sort-and-scan trick:

- Sort lands by Pi in non-increasing order: P1 ≥ P2 ≥ ... ≥ PN. Keep their corresponding Qi alongside.
- When considering land t as the one with the smaller P in a pair, any partner i must have i < t (hence Pi ≥ Pt). For such a pair:
  S(i, t) = min(Pi, Pt) × min(Qi, Qt) = Pt × min(Qi, Qt).
  For fixed t, to maximize min(Qi, Qt) over i < t, simply pick the largest Qi among the first t−1 lands. Let M be the running maximum of Qi seen so far. Then the best partner yields:
  candidate(t) = Pt × min(Qt, M).
- Update M = max(M, Qt) and continue.

The maximum over t of candidate(t) is S2.

Why this works:
- For fixed t, any partner i < t contributes Pt × min(Qi, Qt). The min is maximized by choosing the largest Qi among the prefix. Also, since Qi ≤ Pi, we always have min(Qi, Qt) ≤ Pt, so the a ≥ b constraint is never violated.

Putting it together

The final answer is the better of:
- S1 = max_i (Li × Wi)/2 (two buildings on the same land), and
- S2 = max over pairs computed by the scan above (one building per land).

Implementation outline (pseudocode)

- Read N.
- For i = 1..N:
  - Read Li, Wi.
  - Pi = max(Li, Wi), Qi = min(Li, Wi).
  - Keep track of best_single = max(best_single, Li × Wi).  // This equals 2 × (Li × Wi/2)
- Sort pairs (Pi, Qi) by Pi descending.
- M = -infinity
- best_pair = 0
- For t from 1 to N:
  - If t > 1:
    - candidate = Pt × min(Qt, M)
    - best_pair = max(best_pair, candidate)
  - M = max(M, Qt)
- The maximum area is max( best_single / 2, best_pair ).

Precision and overflow
- Li, Wi ≤ 1e9, so Li × Wi ≤ 1e18 fits in 64-bit signed.
- best_pair is also a product of two ints ≤ 1e9, so ≤ 1e18.
- To avoid floating-point, keep and compare doubled areas:
  - double_ans = max( best_single, 2 × best_pair )
  - Print double_ans / 2 with exactly one decimal:
    - If double_ans is even: print (double_ans/2).0
    - Else: print (double_ans/2).5

Correctness proof (sketch)

- Any optimal arrangement must be of the two allowed types.
- For the same-land case, the constraints force one dimension to be doubled. Maximizing area under 2A ≤ P, B ≤ Q (or the symmetric case) gives exactly (P × Q)/2, and orientation choice guarantees this is achievable.
- For the two-lands case, sorting each land as (P ≥ Q) and the rectangle as (a ≥ b) gives the tight necessary and sufficient conditions a ≤ min(Pi, Pj), b ≤ min(Qi, Qj). Because min(Qi, Qj) ≤ min(Pi, Pj), setting a and b to these bounds yields a feasible rectangle with maximal area for that pair.
- The scan computes max over all pairs efficiently: when Pt is the smaller long side in a pair, the area equals Pt × min(Qi, Qt), which is maximized by using the maximum Qi seen so far (since i must have Pi ≥ Pt). Taking the maximum over all t covers all unordered pairs.
- Therefore, the algorithm returns the maximum possible area over all allowed placements.

Complexity

- Sorting N pairs: O(N log N).
- Single pass scan: O(N).
- Memory: O(N) to store pairs, O(1) extra.

Examples

- Sample 1: lands (5,5), (3,4).
  - S1 = max(25/2, 12/2) = 12.5.
  - Sorted by P: (5,5), (4,3). Scan gives best_pair = 4 × min(3,5) = 12.
  - Answer = max(12.5, 12) = 12.5.

- Sample 2: lands (5,2), (4,3).
  - S1 = max(10/2, 12/2) = 6.
  - Sorted: (5,2), (4,3). best_pair = 4 × min(3,2) = 8.
  - Answer = max(6, 8) = 8.0.

- Sample 3: lands (10,1), (9,8), (7,6).
  - S1 = max(10/2, 72/2, 42/2) = 36.
  - Sorted: (10,1), (9,8), (7,6). Scan candidates: 9×1=9, then 7×min(6,8)=42. best_pair=42.
  - Answer = max(36, 42) = 42.0.

This approach runs comfortably within the time and memory limits and handles all edge cases, including N = 1 (only the same-land option is possible).