Restatement

You are given a melody A of length N and must build a new melody B of the same length such that:
- Each adjacent pair preserves the relative order of A:
  - If A_i < A_{i+1}, then B_i < B_{i+1}.
  - If A_i = A_{i+1}, then B_i = B_{i+1}.
  - If A_i > A_{i+1}, then B_i > B_{i+1}.
- The jump size is bounded: |B_i − B_{i+1}| ≤ K.
- All notes stay within the singer’s range: L ≤ B_i ≤ R.

Among all valid B, output the lexicographically smallest one, or -1 if none exists.

Key ideas

1) Local constraints only depend on adjacent pairs.
   For each i, depending on the sign of A_{i+1} − A_i:
   - Increasing: B_{i+1} ∈ [B_i + 1, B_i + K]
   - Equal: B_{i+1} = B_i
   - Decreasing: B_{i+1} ∈ [B_i − K, B_i − 1]
   Additionally, each B_i must lie in [L, R].

2) Feasible values at each position form a contiguous interval.
   Define S_i as the set of values for B_i that can be extended to a full valid B[i..N]. One can prove by induction that S_i is always an interval [lo[i], hi[i]] (possibly empty). This is because S_i is obtained from S_{i+1} by a “shift-and-thicken” through the local constraint, then intersected with [L, R]. The image of an interval under these operations is still an interval.

3) Backward pass to compute feasibility (interval DP).
   Start from the end: S_N = [L, R].
   Move backward and compute S_i from S_{i+1}:
   - If A_i < A_{i+1} (must increase): B_{i+1} − B_i ∈ [1, K]
     => S_i = [lo[i+1] − K, hi[i+1] − 1] ∩ [L, R]
   - If A_i = A_{i+1}] (must equal): B_{i+1} − B_i = 0
     => S_i = [lo[i+1], hi[i+1]] ∩ [L, R]
   - If A_i > A_{i+1}] (must decrease): B_{i+1} − B_i ∈ [−K, −1]
     => S_i = [lo[i+1] + 1, hi[i+1] + K] ∩ [L, R]
   If at any i this interval becomes empty, no valid B exists.

4) Forward greedy to get the lexicographically smallest B.
   Once all S_i are known and S_1 is non-empty, choose:
   - B_1 = lo[1] (the smallest possible start that still allows a completion).
   - For each i from 1 to N−1, pick the smallest B_{i+1} that is both allowed by the local step from B_i and lies in S_{i+1}:
     - If A_i < A_{i+1}:
       allowed range = [max(lo[i+1], B_i + 1), min(hi[i+1], B_i + K)]
       choose B_{i+1} = left endpoint
     - If A_i = A_{i+1}:
       B_{i+1} = B_i  (and B_i ∈ S_{i+1} holds by construction)
     - If A_i > A_{i+1}:
       allowed range = [max(lo[i+1], B_i − K), min(hi[i+1], B_i − 1)]
       choose B_{i+1} = left endpoint
   The backward DP guarantees these intersections are non-empty if B_i ∈ S_i; picking the left endpoint ensures lexicographic minimality.

Why this works

- Correctness of feasibility DP: S_N = [L, R] is obvious. If S_{i+1} is an interval, then the possible B_i that can reach S_{i+1} via the local constraint form [lo[i+1] − K, hi[i+1] − 1] (increase), [lo[i+1], hi[i+1]] (equal), or [lo[i+1] + 1, hi[i+1] + K] (decrease). Intersect with [L, R] to enforce the global range. Thus S_i is an interval and the recurrence is correct.

- Existence: There exists a valid B iff S_1 is non-empty.

- Lexicographic minimality: Choosing B_1 = lo[1] minimizes the first coordinate among all valid sequences. Given B_i minimized so far, picking the smallest feasible B_{i+1} keeps the prefix lexicographically minimal. The DP ensures feasibility is preserved.

Pseudocode (high-level, no implementation details)

- Input N, L, R, K and array A[1..N].

- Backward feasibility:
  lo[N] = L
  hi[N] = R
  for i = N−1 down to 1:
    if A[i] < A[i+1]:
      lo[i] = max(L, lo[i+1] − K)
      hi[i] = min(R, hi[i+1] − 1)
    else if A[i] == A[i+1]:
      lo[i] = max(L, lo[i+1])
      hi[i] = min(R, hi[i+1])
    else:  // A[i] > A[i+1]
      lo[i] = max(L, lo[i+1] + 1)
      hi[i] = min(R, hi[i+1] + K)
    if lo[i] > hi[i]:
      print −1 and stop

- Forward greedy construction:
  B[1] = lo[1]
  for i = 1 to N−1:
    if A[i] < A[i+1]:
      Lcand = max(lo[i+1], B[i] + 1)
      Rcand = min(hi[i+1], B[i] + K)
      B[i+1] = Lcand
    else if A[i] == A[i+1]:
      B[i+1] = B[i]
    else:
      Lcand = max(lo[i+1], B[i] − K)
      Rcand = min(hi[i+1], B[i] − 1)
      B[i+1] = Lcand
  output B

Edge cases and notes

- N = 1: no adjacency constraints; the solution is B_1 = L (our DP yields lo[1] = L, hi[1] = R).
- Use 64-bit integers when computing lo[i+1] ± K and hi[i+1] ± K to avoid overflow (values are up to 1e9, sums up to 2e9).
- Time complexity: O(N).
- Memory: O(N) to store lo/hi and the result B (well within limits).

Why it handles all cases

- Strict inequality is enforced by the ±1 in the backward formulas and by the forward selection ranges [B_i+1, B_i+K] or [B_i−K, B_i−1].
- Equal segments of A force equal segments in B; the backward DP copies the interval, and the forward pass sets B_{i+1} = B_i.
- The K constraint is enforced both ways: backward (feasible intervals cannot differ by more than K in the right direction) and forward (we only pick within K of the previous).
- The global range [L, R] is enforced by intersecting every S_i with [L, R] and by choosing values within S_{i+1} when advancing.

This method is linear, simple to implement, and produces the lexicographically smallest valid melody if it exists, or correctly reports -1 otherwise.