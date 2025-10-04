Restating the problem
You are given a permutation A of size N (containing each number from 1 to N exactly once). You must output a permutation B of size N (also of 1..N, all distinct) such that the sum of absolute differences diff(A, B) = Σ |Ai − Bi| is at least N. You may output a permutation that maximizes this difference (and doing so automatically satisfies the ≥ N requirement).

Key idea
To make Σ |Ai − Bi| large, we want each Ai to be paired with a number Bi that is “as far as possible” from Ai. A classic tool here is the rearrangement inequality / exchange argument: for two sequences, the sum of absolute differences is maximized by pairing the smallest element of one with the largest element of the other, the second smallest with the second largest, and so on.

Because B must be a permutation of {1, 2, …, N}, the multiset of possible Bi is fixed. Hence, the optimal (maximum) sum is achieved by pairing:
- value 1 with N,
- value 2 with N − 1,
- …
- value N with 1.

Since A already contains each value 1..N exactly once, this pairing translates into a simple, position-wise formula:
Bi = (N + 1) − Ai for every i.

This single-line mapping produces a valid permutation B and maximizes the total difference.

Why this is optimal (exchange argument)
Consider any two positions i and j with Ai ≤ Aj and Bi ≤ Bj. Then
|Ai − Bi| + |Aj − Bj| ≤ |Ai − Bj| + |Aj − Bi|.
So if in some solution the smaller Ai is matched with the smaller Bi (and the larger Aj with the larger Bj), swapping Bi and Bj does not decrease the sum; in fact, it increases it unless both pairs were already in the “opposite order” sense.

By repeatedly applying such swaps, any permutation B can be transformed (without decreasing the sum) until the Bi’s are in the reverse order of the Ai’s. When the set of available Bi is exactly {1..N}, this reverse ordering uniquely assigns the value N to the position where Ai = 1, N − 1 to where Ai = 2, …, and 1 to where Ai = N. That is precisely Bi = (N + 1) − Ai.

Why this always satisfies diff ≥ N
With Bi = (N + 1) − Ai,
diff(A, B) = Σ |Ai − ((N + 1) − Ai)| = Σ |2Ai − (N + 1)|.

Because A is a permutation of 1..N, the multiset {Ai} is {1, 2, …, N}. Hence the sum is the same for every permutation A and equals:
- If N = 2m (even): diff = 2m^2 = N^2 / 2.
- If N = 2m + 1 (odd): diff = 2m(m + 1) = (N^2 − 1) / 2.
In both cases N ≥ 2 implies diff ≥ N. Thus this construction is always valid (and in fact, optimal).

Algorithm
- Input N and the permutation A[1..N].
- For each i from 1 to N, set B[i] = N + 1 − A[i].
- Output B.

Pseudocode
- read N
- read array A[1..N]
- for i = 1..N:
    B[i] = N + 1 − A[i]
- print B[1], B[2], …, B[N]

Correctness proof
- B is a permutation: Since Ai are all distinct and the map x ↦ (N + 1 − x) is a bijection on {1..N}, the Bi’s are also all distinct and lie in [1..N].
- Maximality: By the exchange argument, the sum of absolute differences is maximized when Bi’s order is reverse of Ai’s order. Because {Bi} must be exactly {1..N}, this reverse pairing uniquely yields Bi = (N + 1) − Ai.
- Validity threshold: As shown above, the achieved sum is at least N for N ≥ 2.

Complexity
- Time: O(N). Each Bi is computed in O(1), repeated N times.
- Memory: O(1) extra beyond the input and output arrays.

Handling all cases
- Works for every N ≥ 2 and any permutation A.
- Produces a valid permutation B and guarantees diff(A, B) ≥ N.
- Achieves the maximum possible sum, which is Θ(N^2), far exceeding the required threshold.

Examples
- Sample 1: N=4, A=[1,3,2,4] → B[i]=5−Ai → B=[4,2,3,1], diff=8 ≥ 4.
- Sample 2: N=2, A=[2,1] → B[i]=3−Ai → B=[1,2], diff=2 ≥ 2.

In summary, the one-line construction Bi = (N + 1) − Ai is optimal, simple, and easily meets the time and memory limits.