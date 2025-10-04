Restatement

You are given a permutation P of size N. Define the product A × B by (A × B)i = ABi, so Pz is the z-fold composition. Let M be the smallest integer greater than 1 with P = PM. Then PM−1 is the identity, so L := M − 1 is the order of P. Consider the set S = {P1, P2, …, PL}. Sort S in lexicographic order (compare at the first index where they differ); call the sorted list A. For each query K (1 ≤ K ≤ L), you must output T ∈ {1, …, L} such that PT = AK (i.e., T is the exponent of the K-th smallest power).

Constraints: N ≤ 100, Q ≤ 3 · 10^5, time 3 s, memory 256 MB.


Key ideas

1) Cycle structure and the order L

- Decompose P into disjoint cycles. If an index i lies in a cycle of length ci, then (Pz)i depends only on z mod ci.
- The order L = lcm of the cycle lengths. The powers P0, P1, …, PL−1 are all distinct; PL = identity.

2) Comparing two powers lexicographically

Fix two exponents a, b. At an index j:
- (P a)j = (P b)j iff a ≡ b (mod cj).
- The first index j where (P a) and (P b) differ is characterized by:
  a ≡ b (mod lcm(c1, c2, …, cj−1)) but a not ≡ b (mod cj).

Define L0 = 1 and Lj = lcm(Lj−1, cj) for j = 1…N. Also let dj = gcd(Lj−1, cj) and mj = Lj/Lj−1 = cj/dj.

Then “same prefix up to position j−1” means a ≡ b (mod Lj−1). Inside such a residue class modulo Lj−1, the j-th components are determined by a mod cj subject to a ≡ b (mod dj).

3) The lexicographic order is a nested mixed-radix order

For a fixed residue r modulo Lj−1, the allowed residues for s = a mod cj are exactly the cj/dj = mj residues of the form s ≡ r (mod dj). Among those mj choices, the order is determined by sorting the values (P s)j in increasing order.

Therefore, the global lexicographic sort over all exponents a ∈ {0, 1, …, L−1} (we use 0 for PL) can be seen as:
- At j = 1: choose one of m1 = c1 residues s1 ∈ {0, 1, …, c1−1}, in the order that makes (P s1)1 minimal to maximal. Each choice produces a block of size L/L1.
- At j = 2: within each block, choose one of m2 choices consistent with s2 ≡ s1 (mod d2), ordered by the value at index 2; each choice produces a sub-block of size L/L2.
- …
- At j = N: within each (j − 1)-prefix, choose one of mN choices to fix the full exponent.

This is exactly a mixed-radix enumeration with radices mj, and block weights Bj = L/Lj.

Thus, if K − 1 is written in mixed-radix with digit weights Bj:
- zj = floor((K − 1) / Bj) mod mj
- Then zj selects which choice to take at position j.

The only “dependency” is that the mj choices at level j depend on rj−1 = a mod Lj−1 (specifically r0 = rj−1 mod dj), and thus the order of the mj children depends on the already chosen prefix. That dependency is local and easy to handle.

4) What must be precomputed

- Cycle decomposition: For each position j, know its cycle C (as a list), its length cj = |C|, and its position pj in that cycle such that C[pj] = j. Then (P s)j = C[(pj + s) mod cj].
- Sequence Lj, dj, mj:
  - L0 = 1
  - dj = gcd(Lj−1, cj)
  - Lj = lcm(Lj−1, cj) = Lj−1 · cj / dj
- The global order L = LN, hence M = L + 1.
- For each j and each residue r0 in 0…dj−1, precompute the order of the mj admissible residues s ∈ {r0, r0 + dj, …, r0 + (mj − 1)dj} by sorting their values at index j:
  - For t = 0…mj−1, let s = r0 + t · dj and val = C[(pj + s) mod cj].
  - Sort pairs (val, t) by val. Store the sequence t in ordj[r0][0…mj−1].
- For each j, precompute the modular inverse invj needed for CRT merging:
  - We repeatedly solve x ≡ a (mod Lj−1), x ≡ s (mod cj), with the consistency ensured by s ≡ a (mod dj).
  - Let m′ = Lj−1/dj and n′ = cj/dj. Then m′ and n′ are coprime, and we need invj = (m′)−1 modulo n′ (via extended GCD).

5) Answer a query K

We find the K-th exponent (in 0…L−1), then map 0 to L.

- Compute z1, z2, …, zN by mixed-radix expansion using weights Bj = L/Lj:
  - rem = K − 1
  - For j = 1…N:
    - zj = rem / Bj
    - rem = rem % Bj
- Now reconstruct the exponent a (mod L) and, along the way, maintain rj = a mod Lj for the next step.
  - Initialize r0 = 0.
  - For j = 1…N:
    - Compute r0_small = rj−1 mod dj.
    - Let tindex = ordj[r0_small][zj], i.e., which of the mj sorted options is chosen.
    - Let s = r0_small + dj · tindex (this is a mod cj for this level).
    - Merge rj−1 (mod Lj−1) with s (mod cj) via CRT:
      - g = dj, m′ = Lj−1/g, n′ = cj/g.
      - diff = (s − rj−1) / g (well-defined since s ≡ rj−1 (mod g)).
      - t = (diff mod n′) · invj mod n′.
      - rj = rj−1 + Lj−1 · t, reduced to [0, Lj − 1].
- At the end, a = rN ∈ [0, L − 1]. If a = 0, output T = L, else output T = a.

Why this works

- Lex structure: For two exponents a, b, the first differing index j is exactly characterized by a ≡ b (mod Lj−1) and a ≢ b (mod cj). Inside a fixed residue class mod Lj−1, the lex order at position j is increasing in the numeric values (P a)j, which is a function of a mod cj only. Among residues modulo cj that satisfy a ≡ rj−1 (mod dj), the values are precisely the mj samples s = r0 + t dj around the cycle; sorting those gives the correct within-block order. Ties cannot happen because the values in a permutation cycle are distinct.
- Mixed-radix weights: After fixing the first j choices (i.e., fixing a mod Lj), there are exactly L/Lj completions. Hence the digit weights are Bj = L/Lj, and the number of choices at level j is mj = Lj/Lj−1, exactly matching the mixed-radix system. Therefore the K-th exponent has digits zj = floor((K − 1)/Bj) mod mj.
- CRT update: Fixing the residue modulo cj consistent with the previous prefix uniquely determines the residue modulo Lj = lcm(Lj−1, cj). Using the standard two-moduli CRT formula with a precomputed inverse makes this update O(1) per j. Since Lj fits in 64-bit for N ≤ 100, arithmetic is safe.

Pseudocode (high level, no code)

Preprocessing:
- Decompose P into cycles. For each index j:
  - Record its cycle list Cj, length cj, and position pj in that list.
- L0 ← 1
- For j from 1 to N:
  - dj ← gcd(Lj−1, cj)
  - Lj ← Lj−1 · cj / dj
- L ← LN
- For j from 1 to N:
  - Bj ← L / Lj
  - invj ← inverse of (Lj−1/dj) modulo (cj/dj)
  - For r0 in 0…dj−1:
    - Build list pairs (value = Cj[(pj + (r0 + t·dj)) mod cj], t) for t = 0…(cj/dj − 1)
    - Sort by value; store just the sequence t as ordj[r0][0…mj−1]

Answering a query K:
- rem ← K − 1
- For j from 1 to N:
  - zj ← rem / Bj
  - rem ← rem % Bj
- r ← 0
- For j from 1 to N:
  - r0 ← r mod dj
  - tindex ← ordj[r0][zj]
  - s ← r0 + dj · tindex
  - Merge r (mod Lj−1) and s (mod cj):
    - g ← dj; m′ ← Lj−1/g; n′ ← cj/g
    - diff ← (s − r) / g
    - t ← (diff mod n′) · invj mod n′
    - r ← r + Lj−1 · t  (reduce to [0, Lj − 1])
- If r = 0, print T = L, else print T = r

Complexity

- Preprocessing:
  - Cycle decomposition: O(N).
  - Computing Lj, Bj, inverses: O(N) with small-integer gcds and extended GCD.
  - Building ordj: For each j, total items over all r0 is cj; sorting costs O(cj log cj). Summed over j: O(∑ cj log cj) ≤ O(N · N log N) with N ≤ 100, negligible.
- Each query:
  - Compute zj: N divisions/remainders with 64-bit integers.
  - Reconstruct r: N steps; each step is O(1) arithmetic plus a table lookup.
  - Total O(N) per query; with N ≤ 100 and Q ≤ 3 · 10^5 this is about 3 · 10^7 simple operations, well within 3 seconds in a compiled language.
- Memory: Storing ordj costs ∑j cj integers, but per position j you store cj items; worst case a single 100-cycle contributes 100 · 100 = 10^4 ints overall, easily within limits.

Edge cases and correctness notes

- The identity is always the lexicographically smallest element in S:
  - At index 1, the minimum possible value among all powers is 1 (because 1 is in the cycle of 1), achieved by exponent a ≡ 0 (mod c1).
  - Proceeding inductively, once you force a ≡ 0 (mod L1), the minimum at index 2 is 2 (again achievable because 2 is in its cycle and 0 is compatible with gcd(L1, c2)), etc. Hence the all-zero choice zj = 0 yields the identity P^0 = PL, which maps to T = L and K = 1.
- No ties ever happen when sorting the mj candidates at a fixed index j because they are distinct elements of a permutation cycle.
- We never enumerate all L powers; we only need O(N) work per query.
- Numeric safety: For N ≤ 100, L = order(P) is at most around 2 · 10^9 (Landau’s function), so 64-bit integers are plenty. If you want to be extra safe, do intermediate products in 128-bit and store L, Lj, Bj as 64-bit.

Why the solution is correct

The crux is that lex order aligns perfectly with a hierarchical refinement by indices: two powers share the same prefix up to j − 1 iff their exponents are equal modulo Lj−1; then the order within that class is determined solely by the value at index j, which in turn depends only on the residue modulo cj constrained by dj. This creates a tree with branching factor mj at level j and uniform subtree sizes Bj. Therefore the K-th element corresponds to the mixed-radix digits zj = floor((K − 1)/Bj) mod mj. Each chosen child translates into a unique residue s modulo cj consistent with the already fixed prefix; merging these residues with CRT yields the unique exponent r modulo Lj. By induction over j, we obtain the unique exponent r modulo L that corresponds to the K-th lexicographically smallest power. Mapping r = 0 to T = L finishes the answer.

This handles all cases, including cycles of length 1 (fixed points), multiple indices in the same cycle, and large L, without enumerating all powers.