Restatement

You are given T queries. For each N, consider all permutations P of {0, 1, …, N−1}. Define Q as the permutation satisfying QP[i] = i for all i, i.e., Q = P inverse. Count, modulo M, how many permutations P are such that P ≠ P inverse.

Key observations

- Q is exactly the inverse permutation of P.
- A permutation equals its inverse if and only if it is an involution: its cycle decomposition consists only of 1-cycles (fixed points) and 2-cycles (transpositions).
- Therefore, for each N, the answer is:
  Answer(N) = N! − I(N) (mod M),
  where I(N) is the number of involutions on N elements.

Counting involutions

- Combinatorial characterization: An involution is obtained by choosing k disjoint pairs (2-cycles) and leaving the remaining N−2k elements fixed. This leads to the exact (integer) formula
  I(N) = sum_{k=0..floor(N/2)} N! / (2^k k! (N−2k)!).
- However, modulo M we must not use this formula directly because M may be composite (no safe modular inverses for 2^k or k!).
- Use the classic recurrence instead:
  I(0) = 1, I(1) = 1,
  I(n) = I(n−1) + (n−1)·I(n−2) for n ≥ 2.
  Proof: Consider element n−1. Either it is fixed (contributing I(n−1)), or it is paired with one of the (n−1) other elements (contributing (n−1)·I(n−2)).

Algorithm

We must answer up to 10^6 queries with N up to 10^6. Precompute factorials and involution counts up to the maximum N that appears, all modulo M. This avoids recomputation and respects the time limit.

Steps:
1. Read T and M.
2. Read all N’s and record maxN = max over queries.
3. Precompute:
   - fact[0] = 1; for n = 1..maxN: fact[n] = (fact[n−1] · n) mod M.
   - inv[0] = 1; if maxN ≥ 1 then inv[1] = 1; for n = 2..maxN:
       inv[n] = (inv[n−1] + (n−1) · inv[n−2]) mod M.
4. For each query N:
   - ans = fact[N] − inv[N] (mod M). If negative, add M once.
   - Output ans.

Pseudocode

- Input:
  read T, M
  N_list = [read N for i in 1..T]
  maxN = max(N_list)

- Precompute:
  fact[0] = 1
  for n in 1..maxN:
      fact[n] = (fact[n−1] * n) mod M

  inv[0] = 1
  if maxN ≥ 1: inv[1] = 1
  for n in 2..maxN:
      inv[n] = (inv[n−1] + (n−1) * inv[n−2]) mod M

- Answer queries:
  for each N in N_list:
      ans = fact[N] − inv[N]
      ans = ans mod M  // if ans < 0, add M
      print ans

Correctness

- The map P → P inverse is a bijection on permutations of size N. Its fixed points are exactly the involutions.
- Thus the number of permutations with P ≠ P inverse equals total permutations minus the fixed points, i.e., N! − I(N).
- The recurrence for I(N) is a standard, correct counting argument distinguishing whether the last element is fixed or part of a 2-cycle.
- All computations are done modulo M using only addition and multiplication, so they are valid for any M (not necessarily prime).

Complexity and constraints

- Precomputation takes O(maxN) time and memory.
- Answering T queries takes O(T) time.
- With maxN ≤ 10^6 and T ≤ 10^6, this easily fits in 1 second in typical environments if fast I/O is used.
- Memory: two arrays of length maxN+1 (factorials and involutions). Using 64-bit integers for intermediates but storing modulo M, total memory is well under the given limit.

Implementation notes

- Use 64-bit for intermediate products like (n−1) * inv[n−2] before taking modulo M to avoid overflow; values are < ~10^15 and fit in 64-bit safely.
- After subtraction, normalize: ans = (ans % M + M) % M.
- Edge cases:
  - N = 1: 1! − I(1) = 0.
  - N = 2: 2! − I(2) = 0.
- Reading and writing up to 10^6 lines requires fast I/O.