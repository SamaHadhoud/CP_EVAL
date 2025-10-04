Restatement

You are given an n×n integer matrix A. A matrix T is called absolutely symmetric if for all i, j, we have |Tij| = |Tji| (equivalently, each off-diagonal pair is either equal or opposite in sign).

You must write A as a sum of k absolutely symmetric integer matrices X1 + … + Xk, minimizing k. If no such decomposition exists, print -1. If it exists, output the minimal k; when k ≤ 50 (always true here), also output one valid set of k matrices.


Key ideas

1) What an absolutely symmetric matrix contributes on a pair (i, j)
- Consider an off-diagonal pair (i, j) with i ≠ j. In any absolutely symmetric matrix X, the entries on this pair are either equal or negatives of each other: Xij = x, Xji = ±x for some integer x.
- Therefore, if we sum several absolutely symmetric matrices, the total on this pair (Aij, Aji) is a sum of vectors of the form (x, x) or (x, −x) (x ∈ Z).

2) Parity obstruction (necessary condition)
- For a single absolutely symmetric matrix, on a fixed pair (i, j), the sum and difference are even:
  - (x + x) = 2x is even, and (x − (±x)) ∈ {0, 2x} is also even.
- Summing any number of such matrices preserves evenness, hence for A to be decomposable, for every i ≠ j, both Aij + Aji and Aij − Aji must be even. Equivalently, Aij and Aji must have the same parity for every off-diagonal pair.
- If any pair (i, j) violates this parity condition, no decomposition exists.

3) A universal 2-term decomposition when parity holds
- If Aij and Aji have the same parity for all i ≠ j, define
  - S = (A + AT)/2 (a symmetric integer matrix),
  - K = (A − AT)/2 (a skew-symmetric integer matrix, i.e., K = −KT).
- Both S and K have integer entries thanks to the parity condition.
- Symmetric S is absolutely symmetric because Sij = Sji. Skew-symmetric K is also absolutely symmetric because Kji = −Kij ⇒ |Kji| = |Kij|.
- Clearly A = S + K.
- Thus, whenever a decomposition exists, k ≤ 2 always suffices.

4) When is k = 1 possible?
- k = 1 means A itself must be absolutely symmetric, i.e., |Aij| = |Aji| for all i, j. If so, take X1 = A.
- If A is not absolutely symmetric but parity holds, k cannot be 1 (since a single absolutely symmetric matrix would have to be A itself), so the minimal k is 2.

Summary of the structure of the answer
- If some pair (i, j) has Aij and Aji of different parity: print −1.
- Else, if A is absolutely symmetric (|Aij| = |Aji| for all i, j): minimal k = 1 and X1 = A.
- Else: minimal k = 2 and one optimal construction is:
  - X1 = S = (A + AT)/2 (symmetric),
  - X2 = K = (A − AT)/2 (skew-symmetric).
- Diagonal entries are unconstrained by the absolute symmetry condition; in the above construction they go entirely into S (Sii = Aii, Kii = 0).

Why this is correct

- Necessity: In any absolutely symmetric matrix, the off-diagonal pair (x, ±x) has even sum and even difference. Summing such pairs across any number of matrices keeps both Aij + Aji and Aij − Aji even. Hence Aij and Aji must have the same parity for every i ≠ j.
- Sufficiency: If all pairs have the same parity, S and K defined above are integer matrices, both absolutely symmetric, summing to A.
- Minimality: If A is absolutely symmetric, k = 1 is possible and clearly minimal. If A is not absolutely symmetric but parity holds, k = 1 is impossible, while k = 2 is always achievable via S + K; thus minimal k = 2.

Bounds and constraints

- n ≤ 50, so O(n^2) time and memory are trivial.
- Entries of S and K are at most about 10^9 in magnitude (since they are halves of sums/differences of entries bounded by 10^9), which fits in the required output bounds of ±10^18.
- k is either 1 or 2, hence ≤ 50, so matrices must be printed in all feasible cases.

Pseudocode

- Read n and matrix A.
- parity_ok = true
- abs_symmetric = true
- For i in [0..n−1]:
  - For j in [0..n−1], j ≠ i:
    - If (A[i][j] − A[j][i]) is odd: parity_ok = false
    - If abs(A[i][j]) ≠ abs(A[j][i]): abs_symmetric = false
- If not parity_ok: print −1 and stop.
- If abs_symmetric:
  - Print 1 and then print A.
- Else:
  - Construct S and K:
    - For all i:
      - S[i][i] = A[i][i], K[i][i] = 0
    - For i ≠ j:
      - S[i][j] = (A[i][j] + A[j][i]) / 2
      - K[i][j] = (A[i][j] − A[j][i]) / 2
  - Print 2, then print S, then K.

Examples

- Sample 1: A is the zero matrix, which is absolutely symmetric ⇒ k = 1, X1 = A.
- Sample 2: A12 = 1, A21 = 0 have different parity ⇒ −1.
- Sample 3: A12 = 3, A21 = 5 have same parity but |3| ≠ |5| ⇒ k = 2. One valid choice is S = (A + AT)/2 and K = (A − AT)/2; many others also work.

This algorithm runs in O(n^2), uses O(n^2) memory, always outputs a minimal decomposition when it exists, and the number of matrices is at most 2.