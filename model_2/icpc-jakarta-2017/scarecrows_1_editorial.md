Restatement

You have an R×C grid (R ≤ 14, C ≤ 1000). A scarecrow can be placed only on cells marked '.', not on 'v'. An arrangement is sacred if:
- Every row has at least one scarecrow somewhere in the row.
- For every pair of consecutive columns j and j+1, at least one of those columns contains a scarecrow (i.e., two adjacent empty columns are forbidden).

Count the number of sacred arrangements modulo 1,000,000,007.

Key observations

1) Column-wise choices are sets of rows:
- For each column j, let A[j] be the set of rows where the cell is '.' (represent it as a bitmask over R rows).
- An arrangement is a sequence of subsets S[1], S[2], …, S[C], where S[j] ⊆ A[j]. S[j] tells which rows in column j receive a scarecrow.

2) The “no two consecutive empty columns” condition only depends on whether S[j] is empty or not, not on which rows inside S[j] were chosen.

3) The “every row has at least one scarecrow” condition couples all columns and all choices; it’s a global coverage constraint across rows.

Main idea

Use inclusion–exclusion over rows to enforce the “every row is covered” constraint, and inside that, use a simple 2-state DP across columns to enforce “no two consecutive empty columns.”

Inclusion–exclusion over rows

- For a subset T of rows, consider arrangements where no scarecrow is placed in any row from T (i.e., these rows are “forbidden” everywhere).
- If we denote by g(T) the number of sequences S[1..C] with:
  - S[j] ⊆ A[j] \ T for all j, and
  - No two consecutive columns are both empty,
  then the number of sacred arrangements is:
  Answer = sum over all T ⊆ {1..R} of (-1)^{|T|} × g(T)  (mod M)
This is the standard inclusion–exclusion over the events “row r is never used.”

Computing g(T) fast

Fix T. Define B[j] = A[j] \ T, the allowed rows in column j after forbidding T. Let k_j = |B[j]| be the number of allowed rows in column j.

- In column j, the number of possible non-empty choices is W[j] = 2^{k_j} − 1 (choose any non-empty subset of allowed rows).
- Choosing the empty set has exactly 1 way.

The only coupling between columns is “no two consecutive empty columns.” This yields a 2-state DP over columns:

- dp0[j]: number of ways for the first j columns where column j is empty
- dp1[j]: number of ways for the first j columns where column j is non-empty

Transitions:
- Base (j = 1):
  - dp0[1] = 1  (first column empty)
  - dp1[1] = W[1]  (first column non-empty)
- For j ≥ 2:
  - dp0[j] = dp1[j−1]           (empty now only if previous was non-empty)
  - dp1[j] = (dp0[j−1] + dp1[j−1]) × W[j]  (non-empty now after any previous)

Finally g(T) = dp0[C] + dp1[C] (modulo M).

Efficient implementation details

- Represent each column j by a bitmask A[j] over R bits: bit r is 1 if grid[r][j] = '.'.
- Precompute pow2[t] = 2^t mod M for t = 0..R.
- For each T from 0 to (1<<R)−1:
  - Let allowedMask = (~T) masked to R bits.
  - For each column j, compute k_j = popcount(A[j] & allowedMask), then W[j] = pow2[k_j] − 1 (mod M).
  - Run the 2-state DP above to get g(T).
  - Add or subtract g(T) from the answer according to the parity of |T|.

This is O(C × 2^R) time:
- There are 2^R subsets T (≤ 16384).
- For each T we scan C columns (≤ 1000), computing popcount over at most 14 bits and updating two DP numbers.
- Memory is small: A[1..C] (C masks), pow2[0..R], and a few integers for DP and the accumulator.

Pseudocode (sketch, not language-specific)

- Read R, C and the grid.
- Build A[j]:
  - Initialize A[j] = 0 for all j.
  - For each row r in 0..R-1:
    - Read the r-th string s of length C.
    - For each column j in 0..C-1:
      - If s[j] == '.', set A[j] |= (1 << r).
- Precompute pow2[t] for t=0..R with pow2[0]=1 and pow2[t]=pow2[t-1]*2 mod M.
- ans = 0
- For T from 0 to (1<<R)-1:
  - allowedMask = ((1<<R)-1) ^ T
  - Let w1 = pow2[popcount(A[0] & allowedMask)] - 1; if negative, add M.
  - dp0 = 1
  - dp1 = w1
  - For j from 1 to C-1:
    - w = pow2[popcount(A[j] & allowedMask)] - 1; if negative, add M.
    - next_dp0 = dp1
    - next_dp1 = ((dp0 + dp1) mod M) * w mod M
    - dp0 = next_dp0
    - dp1 = next_dp1
  - g = (dp0 + dp1) mod M
  - If popcount(T) is even: ans = (ans + g) mod M
    else: ans = (ans - g) mod M
- If ans < 0, add M.
- Print ans.

Why it’s correct

- Inclusion–exclusion correctness: Let E_r be the event “row r never gets a scarecrow.” We want the count of sequences avoiding ∪_r E_r. By inclusion–exclusion, this equals Σ_T (-1)^{|T|} × #(sequences satisfying all E_r for r ∈ T). For fixed T, “satisfying all E_r for r ∈ T” is exactly forbidding rows T globally, i.e., S[j] ⊆ A[j] \ T for all j. That is precisely what our g(T) counts under the remaining column-pair constraint.
- DP correctness for g(T): After fixing T, columns are independent except for the adjacency rule that two consecutive empties are forbidden. The weighted choices per column are:
  - 1 way to be empty, and
  - W[j] ways to be non-empty.
  The 2-state DP exactly encodes the only inter-column restriction (no E→E transition). Summing the two end states counts all valid prefixes.

Edge cases
- C = 1: The constraint about consecutive columns is vacuous; dp0=1 and dp1=W[1] handle this correctly. Inclusion–exclusion then enforces the per-row coverage.
- Columns with no allowed cells: If B[j] is empty for some j (W[j] = 0), that column must be empty; if two such columns are consecutive, the DP yields g(T) = 0, as required.
- Rows with no '.' at all: Then the answer is 0. The inclusion–exclusion sum naturally collapses to 0.

Complexity

- Time: O(C × 2^R) ≈ 1000 × 16384 ≈ 1.6×10^7 state updates, easily within 2 seconds in a compiled language.
- Memory: O(C) for A plus O(R) for pow2; the DP uses O(1) extra memory.

Intuition on the samples

- When a row has no '.' anywhere (Sample 3), every arrangement misses that row, and inclusion–exclusion cancels everything to 0.
- When C is small and all cells are '.', the DP per T reduces to counting weighted binary strings with no consecutive zeros; inclusion–exclusion across rows collapses to the exact count of arrangements satisfying the per-row coverage.

This approach meets the limits, is straightforward to implement with bitmasks and a tiny DP per subset, and the correctness follows directly from inclusion–exclusion and a simple adjacency DP.