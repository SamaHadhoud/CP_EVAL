Restatement

We have N Pokemon in one long row (one box). Pichuu wants the Pokemon currently at position i to end up at position A[i] in the final PC ordering. A is a permutation of 1..N.

Team Galactic can partition the single row into K contiguous segments (K ≤ N), place each segment into its own box in the same left-to-right order, and then Pichuu can use AutoSort inside each box only (he can arbitrarily reorder the Pokemon inside a box, but cannot move Pokemon between boxes or reorder boxes). Galactic wants to maximize K, but must still allow Pichuu to reach his desired final order.

After each of M operations, Pichuu swaps his preferences at two positions (i.e., he swaps A[u] and A[v]). We must output the maximum K achievable before any swaps and after each swap.

Key idea 1: When is a cut valid?

Consider making a cut after position r (ending a box at r). After AutoSort inside each box, the global order is the concatenation of the sorted contents of each box. For the final PC to match Pichuu’s target order (which is: Pokemon appear by increasing desired position 1,2,3,…), the first box must contain exactly the desired positions {1,2,…,r} (in some arrangement), so that after sorting that box internally it produces 1,2,…,r as the prefix of the full order. This gives a necessary and sufficient condition for r to be a valid cut:

- Valid cut after r ⇔ the set {A[1], A[2], …, A[r]} equals {1, 2, …, r}
- Equivalently, max(A[1..r]) = r (since A is a permutation).

Thus the maximum number of boxes K equals the number of indices r ∈ {1..N} such that max(A[1..r]) = r. (Note that r = N is always valid.)

Key idea 2: A dynamic way to count valid cuts

We will count valid r via an equivalent “no-crossing” condition. For a boundary after r to be valid, the first r positions must contain no value > r. Let’s encode all violations across boundaries:

- For each element at position p with value v = A[p], and for each boundary r, this element “violates” the boundary r if and only if p ≤ r < v. In other words, an element to the left that wants to go (in the final order) strictly to the right of the boundary makes the boundary invalid.

Therefore, define an array C[1..N] as:
- C[r] = number of pairs (p, v) with p ≤ r < v.

Then:
- Boundary after r is valid ⇔ C[r] = 0.
- Because A is a permutation, C[r] ≥ 0 for all r and C[N] = 0 always (no value exceeds N), so there is always at least one zero.

Crucially, each element (p, v) contributes +1 to all C[r] with r ∈ [p, v-1] if and only if v > p; otherwise it contributes nothing (the interval is empty).

Hence:
- Initialize C as all zeros.
- For each position p, with v = A[p], do: if v > p, add +1 to C[p..v-1].

The answer K is exactly the count of indices r with C[r] = 0.

Key idea 3: Handling swaps efficiently

Each operation swaps A[u] and A[v]. Only the contributions of the two pairs (u, A[u]) and (v, A[v]) change. Before the swap, we remove their contributions; after, we add the contributions of the new pairs:

- Let x = A[u] and y = A[v].

Remove old:
- If x > u, add -1 to C[u..x-1].
- If y > v, add -1 to C[v..y-1].

Swap the values in A:
- A[u] ← y, A[v] ← x.

Add new:
- If y > u, add +1 to C[u..y-1].
- If x > v, add +1 to C[v..x-1].

After applying these four range updates, the entries of C again represent exactly the number of violations per boundary, so the number of zeros in C is the new answer.

Data structure

We must support up to 5e5 swaps and N up to 5e5, so O(N) per swap is impossible.

We need:
- Range add on C[l..r] by ±1
- After updates, quickly retrieve the number of indices r with C[r] = 0.

Use a segment tree with lazy propagation that maintains for each node:
- minVal: the minimum C[r] in the node’s interval
- cntMin: the number of positions r in the node’s interval achieving that minimum
- lazy: pending increment to apply to the interval

Operations:
- RangeAdd(l, r, delta): standard lazy propagation, update minVal and lazy.
- To combine two children L and R:
  - minVal = min(L.minVal, R.minVal)
  - cntMin = (L.minVal == minVal ? L.cntMin : 0) + (R.minVal == minVal ? R.cntMin : 0)

Why does this give the answer?
- In the final (consistent) state after an update, C[r] are counts of violations, so C[r] ≥ 0 and C[N] = 0 ⇒ the global minimum of C is 0.
- Therefore the number of zeros is exactly cntMin at the root (since minVal at the root equals 0). We output root.cntMin before swaps and after each swap.

Initialization and processing

- Build a segment tree over indices 1..N with initial values C[r] = 0 for all r (root.minVal = 0, root.cntMin = N).
- For p from 1 to N:
  - v = A[p]
  - If v > p: RangeAdd(p, v-1, +1)
- Print root.cntMin (the initial answer).
- For each swap (u, v):
  - x = A[u], y = A[v]
  - If x > u: RangeAdd(u, x-1, -1)
  - If y > v: RangeAdd(v, y-1, -1)
  - Swap A[u], A[v]
  - If y > u: RangeAdd(u, y-1, +1)
  - If x > v: RangeAdd(v, x-1, +1)
  - Print root.cntMin

Pseudocode outline

- Build:
  - for r in 1..N: C[r] = 0
  - build segment tree storing (minVal=0, cntMin=segment length)
  - for p in 1..N:
      v = A[p]
      if v > p: rangeAdd(p, v-1, +1)
  - print tree.root.cntMin

- For each query (u, v):
  - x = A[u]; y = A[v]
  - if x > u: rangeAdd(u, x-1, -1)
  - if y > v: rangeAdd(v, y-1, -1)
  - swap(A[u], A[v])
  - if y > u: rangeAdd(u, y-1, +1)
  - if x > v: rangeAdd(v, x-1, +1)
  - print tree.root.cntMin

Why this is correct

1) Characterization of valid cuts:
- If max(A[1..r]) = r, then {A[1..r]} ⊆ {1..r} and since both have size r, it equals {1..r}. Sorting inside the first box makes it exactly 1..r; iterating this argument over all chosen cut positions proves the concatenation equals 1..N, so all chosen cuts are simultaneously feasible.
- Conversely, if a cut after r is feasible, the first box’s values must be 1..r, so max(A[1..r]) = r.

Thus K equals the number of r with max(A[1..r]) = r.

2) Violations per boundary:
- For boundary r, a violation is exactly a pair (p, v) with p ≤ r < v. The count of such pairs is C[r]. Hence r is valid iff C[r] = 0.
- Each (p, v) contributes +1 to C[p], C[p+1], …, C[v-1] if v > p; otherwise contributes nothing. Therefore C is exactly the sum of those interval contributions.

3) Updates by swapping:
- Swapping A[u] and A[v] only changes contributions of those two pairs; replacing intervals [u..x-1] and [v..y-1] by [u..y-1] and [v..x-1] is exactly what happens to C. So applying those four range updates yields the correct new C, and the number of zeros is the new K.

4) The “count-of-min is count-of-zeros” fact:
- In a consistent state, C[r] are counts, thus ≥ 0, and C[N] = 0 (since no v > N exists). So the global minimum of C is 0. The segment tree’s (minVal, cntMin) at the root therefore gives the number of zeros.

Complexity

- Building initial C: O(N log N) via N range adds for elements with v > p.
- Each swap: up to 4 range adds, each O(log N).
- Total time: O((N + M) log N), well within 2 seconds for N, M ≤ 5e5.
- Memory: Segment tree over N indices with (min, cnt, lazy) per node uses O(N) memory (well below 1024 MB).

Handling all cases

- Empty intervals (when v ≤ p) are simply skipped.
- Swapping the same position (u = v) or swapping equal values (x = y) leads to zero net change; the logic still works.
- The last boundary r = N always has C[N] = 0, ensuring the minimum is 0 after each full update sequence.

Summary

- A cut after r is feasible iff the first r values are exactly {1..r}, i.e., max(A[1..r]) = r.
- Count these r by maintaining an array C where C[r] counts “violations” crossing r, with each (p, v) contributing +1 to C[p..v-1] if v > p.
- Under swaps, only two pairs change, so we update C with at most four range adds.
- Use a lazy segment tree tracking (min, count-of-min) to get in O(log N) per range add the number of zeros, which equals the maximum number of boxes.