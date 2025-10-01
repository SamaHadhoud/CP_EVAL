Restatement

You start with B = (0, …, 0). In one move you pick two indices i ≠ j and XOR the same value ai ⊕ aj into both positions bi and bj. After any number of moves (order/number doesn’t matter, parity does), how many distinct arrays B can you obtain? Output this count modulo 998244353.

Key observations

- Only parity matters. Doing an operation twice cancels (XOR), so each unordered pair {i, j} is either used 0 or 1 time. The set of attainable B is a linear subspace over GF(2).
- A single operation on {i, j} adds the same “label” s = ai ⊕ aj to both positions i and j and leaves all others unchanged.
- Therefore, attainable B is the linear span of m = n(n−1)/2 column vectors cij that have s = ai ⊕ aj at positions i and j and 0 elsewhere. The number of distinct B equals 2rank, where rank is the dimension of this span.

Group indices by equal values

Let the distinct values among a1, …, an be α1, α2, …, αs (s is the number of distinct ai), and let ck be the size of the group of indices whose ai = αk. So Σk ck = n.

- If two indices are in the same group (ai = aj), then ai ⊕ aj = 0 and the operation does nothing; such pairs can be ignored.
- If indices come from two different groups p ≠ q (values αp and αq), then every edge between these two groups has the same label Lpq = αp ⊕ αq (a fixed nonzero 30-bit number).

Crucial decomposition by labels

Think of the 30-bit numbers as vectors over GF(2). For a fixed label u ≠ 0, all columns with ai ⊕ aj = u live along the same 1-dimensional direction “u” in value space: each such column writes u at two positions and 0 elsewhere. Different labels u give linearly independent directions, so the total rank is the sum of ranks contributed by each label u separately.

What does one label u connect?

Fix u. It pairs each value class x with x ⊕ u. On the present classes {α1, …, αs}, this produces a perfect matching of classes into disjoint pairs {x, x ⊕ u} (some pairs may be missing if one side has no indices). For each present pair {p, q} with αp ⊕ αq = u, the vertices of the two classes form a complete bipartite graph Kck,cq. The columns corresponding to edges of this Kck,cq (in label direction u) are exactly the usual incidence columns of Kck,cq (scaled by u).

Standard fact (incidence rank over GF(2)): In a connected graph on t vertices, the columns of the (unsigned) incidence matrix over GF(2) have rank t − 1. For Kck,cq (connected if ck, cq > 0), the rank is ck + cq − 1.

Thus, for a fixed u, the rank contribution Ru equals the sum over all disjoint class pairs {p, q} with αp ⊕ αq = u of (ck + cq − 1).

Summing over all labels (and simplifying)

Each unordered pair of distinct value classes {p, q} appears in exactly one label u = αp ⊕ αq and contributes (ck + cq − 1) to the total rank. Therefore,

rank = Σ over all unordered pairs {p, q} (ck + cq − 1).

This collapses to a very simple formula depending only on s and n:

- Σ over pairs (ck + cq) = (s − 1) Σk ck = (s − 1) n, because each class ck is paired with s − 1 other classes.
- The number of unordered pairs is C(s, 2) = s(s − 1)/2.

Hence
rank = (s − 1) n − s(s − 1)/2.

Equivalently, rank = (s − 1)·(n − s/2). Note this is always an integer because of the first form.

Finally, the number of distinct B is 2rank, so the answer is 2rank modulo 998244353.

Why this is correct

- Upper bound per class pair: For fixed classes p, q, all columns live in the label direction αp ⊕ αq and are supported only on indices of these two classes. Over GF(2), the image in that component has dimension at most ck + cq − 1 (connected incidence). Summing over all unordered class pairs gives an upper bound for the total rank.
- Achieving the bound: For each unordered class pair {p, q}, pick a spanning tree in Kck,cq (e.g., take one “pivot” vertex in each class and include edges from one pivot to all vertices of the other class; total ck + cq − 1 edges). The corresponding columns are linearly independent within that component. Components with the same label u are on disjoint vertex sets (due to the fixed pairing {x, x ⊕ u}), so their contributions add. Different labels u lie in disjoint 1D directions in value space and thus are independent. Therefore we can realize the full sum of (ck + cq − 1) across all unordered pairs, matching the upper bound.
- Thus rank equals the derived formula.

Algorithm

- Let s be the number of distinct values in a1, …, an. This can be found by sorting and counting unique, or by using a hash set.
- Compute E = (s − 1)·n − s·(s − 1)/2 as a 64-bit integer (it can be up to about n(n − 1)/2 ≈ 2e10).
- Answer = pow_mod(2, E, 998244353).

Pseudocode

- Read n and array a[1..n]
- s = number_of_distinct_values(a)
- E = (s − 1) * n − s * (s − 1) / 2   // use 64-bit
- return fastPowMod(2, E, 998244353)

fastPowMod(base, exp, mod):
- res = 1
- while exp > 0:
    - if exp is odd: res = (res * base) % mod
    - base = (base * base) % mod
    - exp >>= 1
- return res

Complexity

- Counting distinct values: O(n log n) with sorting, or expected O(n) with hashing.
- Modular exponentiation: O(log E) = O(log n^2) ≈ O(log n).
- Memory: O(1) beyond input storage.

Sanity checks on samples

- Sample 1: a = [1, 2, 1], s = 2. E = (2 − 1)*3 − 2*1/2 = 3 − 1 = 2. Answer = 2^2 = 4.
- Sample 2: all equal, s = 1. E = 0. Answer = 1.

Edge cases

- All ai distinct (s = n): rank = (n − 1)n − n(n − 1)/2 = n(n − 1)/2, so every subset of pairs yields a distinct B, as expected.
- All ai equal (s = 1): no move changes B, only the zero array is reachable.

This approach fits well within the time and memory limits and gives a simple, surprisingly strong conclusion: the answer depends only on n and the number of distinct values in a.