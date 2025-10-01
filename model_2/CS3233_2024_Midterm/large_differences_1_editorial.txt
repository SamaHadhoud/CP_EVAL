Restatement

You are given N heights H1..HN. You may permute them in a line. For that permutation, let X be the minimum absolute difference between adjacent heights. Your goal is to arrange them to maximize X, and output this maximum possible value.

Key idea in one line

Sort the heights. Let t = floor(N/2). The answer is exactly
min over i = 1..t of (a_{i+t} − a_i),
where a is the sorted array (1-indexed). An optimal arrangement is obtained by interleaving the upper half and the lower half in order: a_{t+1}, a_1, a_{t+2}, a_2, … (and if N is odd, the extra upper-half element sits at one end).

Why sorting and splitting works

Intuition:
- To make the minimum adjacent gap large, avoid placing numbers that are close (in value) next to each other.
- The most aggressive way to separate close values on a line is to split the sorted array into two halves and alternate between them: always put a “large” next to a “small”.
- The unavoidable bottleneck then comes from the worst-matched pair between the i-th smallest of the lower half and the i-th smallest of the upper half.

Construction that achieves the bound (lower bound)

1) Sort H to get a1 ≤ a2 ≤ … ≤ aN.
2) Let t = floor(N/2).
3) Define the lower half L = [a1, a2, …, a_t] and the upper half U = [a_{t+1}, a_{t+2}, …, a_N].
4) Build the line by interleaving U and L in order:
   - If N is even: U1, L1, U2, L2, …, U_t, L_t.
   - If N is odd: U1, L1, U2, L2, …, U_t, L_t, U_{t+1}.

What are the adjacent differences in this line?
- Between U_i and L_i: difference = U_i − L_i.
- Between L_i and U_{i+1}: difference = U_{i+1} − L_i.

Because U is increasing, U_{i+1} − L_i ≥ U_i − L_i. Therefore, the minimum over all adjacent differences in this construction is exactly
min over i = 1..t of (U_i − L_i) = min over i = 1..t of (a_{t+i} − a_i).

Thus we can always achieve X ≥ min_i (a_{t+i} − a_i).

Why you cannot do better (upper bound)

We now show that for any permutation, its minimum adjacent difference is at most
M = min over i = 1..t of (a_{t+i} − a_i).

Consider any fixed arrangement in a row. Partition the row into t disjoint adjacent pairs:
(1,2), (3,4), (5,6), …, (2t−1, 2t).
- If N is odd, the last element at position N is unpaired; that doesn’t hurt the upper bound because we are looking for a pair whose difference is small.

Now fix any i from 1 to t and look at the “window” Wi = {a_i, a_{i+1}, …, a_{i+t}} of t+1 sorted values.
- There are only t disjoint adjacent pairs in the row, but Wi contains t+1 elements. By the pigeonhole principle, at least one of these t pairs must contain two elements both from Wi (if the unpaired tail element happens to be in Wi when N is odd, either choose the other parity pairing (2,3), (4,5), …, or note that the unpaired element is adjacent to some neighbor anyway; in either case, an adjacent pair drawn from Wi exists).
- The difference of those two values is at most the width of Wi, which is a_{i+t} − a_i.
- Therefore, the minimum adjacent difference in this arrangement is ≤ a_{i+t} − a_i.

Since this holds for every i, the minimum adjacent difference in any arrangement is ≤ M = min_i (a_{i+t} − a_i).

Combining with the construction, we conclude the optimal value is exactly M.

Pseudocode (high level)

- Read N and array H.
- Sort H as a[1..N].
- Let t = floor(N/2).
- ans = +infinity
- For i from 1 to t:
  - ans = min(ans, a[i + t] − a[i])
- Output ans.

Correctness and corner cases

- N = 2: t = 1; answer = a2 − a1, which is exact.
- Duplicates: If many values are equal, some a_{t+i} − a_i can be zero, and the construction interleaves equal values, giving minimum gap zero; no arrangement can avoid this bound.
- Odd N: The extra element in U can be placed at one end; all adjacencies remain between a lower and an upper element except at that end, and we already showed these “cross pairs” determine the minimum and equal the claimed formula.
- Large values (up to 1e9): compute differences using sufficient integer range; conceptually no issues.

Complexity

- Sorting dominates: O(N log N).
- One linear pass to compute the minimum over i.
- Memory: O(N) to store the array.
- Fits easily in 1s and 1024 MB for N up to 5e5.

Why this handles all cases

- The lower-half/upper-half interleaving produces a line where every adjacency is between well-separated ranks, pushing the minimum gap as high as possible.
- The window/pigeonhole argument shows that any line, no matter how cleverly built, must contain an adjacent pair drawn from some tight window of t+1 consecutive ranks, capping the minimum by that window’s width. The tightest such window determines the global maximum.
- Together, these prove optimality.

Worked through samples

- Sample 1: a = [2,2,6,10], t = 2. min(a3−a1, a4−a2) = min(6−2, 10−2) = min(4,8) = 4.
  Interleave: [6,2,10,2], gaps = [4,8,8], min = 4.

- Sample 2: a = [2,3,6,7,8], t = 2. min(a3−a1, a4−a2) = min(6−2, 7−3) = min(4,4) = 4.
  Interleave: [6,2,7,3,8], gaps = [4,5,4,5], min = 4.

Takeaway

The problem reduces to a clean formula after sorting: the answer is the tightest gap between the i-th element of the lower half and the i-th element of the upper half. Interleaving these halves achieves the bound and is optimal.