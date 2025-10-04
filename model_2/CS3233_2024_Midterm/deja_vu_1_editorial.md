Restatement

You have n positive integers H1..Hn. You must choose exactly k of them; the “happiness” of a choice is the sum of the chosen values. Among the C(n, k) possible choices, you need to output the l largest sums (with multiplicity: equal sums should be repeated as many times as they occur among the combinations). If i > C(n, k), output −1.

Constraints are large (n and l up to 10^6), so any method that enumerates many combinations is infeasible. We need to exploit structure to generate only the top l sums efficiently.

Key ideas

1) Sort and truncate to a small prefix

- Sort H in non-increasing order: A[0] ≥ A[1] ≥ … ≥ A[n−1].
- Because all values are positive, the largest k-sums come from the largest values of A. In fact, there is a smallest T (k ≤ T ≤ n) such that C(T, k) ≥ l, and the l largest k-sums among all C(n, k) combinations are already contained among the k-combinations of the first T numbers.
- Intuition: if a combination uses any index ≥ T, swap that element with some unused element from the first T; the sum cannot decrease because A is non-increasing. Since the first T numbers already yield at least l distinct k-combinations, we can source the top l sums from that prefix.

How to find T efficiently:
- Start from t = k and the running value bin = C(t, k) = 1.
- While t < n and bin < l:
  bin = bin * (t + 1) / (t + 1 − k); t++
- Stop at T = t. Because l ≤ 10^6, bin never needs to exceed ~10^6; with 64-bit arithmetic, bin * (t + 1) stays well within 64-bit range (≤ ~10^12), so the update is exact.

If T reaches n and still bin < l, then l > C(n, k). We will generate all C(n, k) sums (which is then < l and, crucially, ≤ 10^6), and print −1 for the remaining lines.

2) Base sum and “penalty” representation

Let S0 = A[0] + A[1] + … + A[k−1] be the maximum k-sum (take the top k elements).

Any other k-combination can be described as “pushing” some of these k indices to the right. Define differences:
D[i] = A[i] − A[i+1] for i = 0..T−2, and note D[i] ≥ 0.

If we move the r-th chosen index (0-based) from position r to position r + s_r (s_r ≥ 0), the value at that position decreases by
f_r(s_r) = D[r] + D[r+1] + … + D[r + s_r − 1]
(the sum of differences we step over). The chosen indices must remain strictly increasing, which is equivalent to the non-decreasing constraint on the shifts:
0 ≤ s_0 ≤ s_1 ≤ … ≤ s_{k−1} ≤ s_max, where s_max = T − k.

Thus every k-combination corresponds to a shift-vector s with these constraints, and its sum equals:
S0 − (f_0(s_0) + f_1(s_1) + … + f_{k−1}(s_{k−1})).

So the problem becomes: enumerate the l smallest penalties (sum of f_r’s) over non-decreasing s, and subtract from S0. We will do this without enumerating all C(T, k) combinations.

3) Best-first enumeration with a tiny state (two children per state)

We can enumerate valid s in increasing penalty using a min-heap (priority queue) and an implicit tree. Each node in the heap represents a partially decided shift-vector where we have “opened” positions from 0 up to some p (0 ≤ p < k); position p is the only one currently allowed to increase further; positions > p are fixed at their minimal values implied by the current prefix.

Represent a node by:
- cost: the current penalty (sum of f_r for r ≤ p),
- p: the highest index opened so far,
- x: the absolute index currently used at position p (so x = p + s_p). The earlier opened positions are implicit in the path that produced this node.

The full k-combination represented by a node is obtained by:
- taking the indices chosen at positions 0..p (as accumulated along the path), and
- filling the tail minimally: position p+1 gets x+1, then x+2, …, up to k−1. This is a valid k-combination.

Children of a node (cost, p, x):
- Same position advance (if possible):
  If x < (T − k + p), we can increase the p-th position by 1:
  child A: (cost + D[x], p, x + 1).
  Explanation: moving from x to x+1 adds penalty D[x].
- Open the next position (only if it could ever increase):
  If p + 1 < k and x < (T − k + p), we can “open” position p+1 at its minimal value x+1:
  child B: (cost, p + 1, x + 1).
  We require the same condition x < T − k + p; otherwise, opening a new position would never be able to move and would only duplicate the same combination.

Start node:
- The maximum-sum combination is with no shifts: indices [0, 1, …, k−1].
- That corresponds to the node (cost = 0, p = 0, x = 0).

Algorithm:
- Push the start node into a min-heap keyed by cost.
- Repeat until you have output l numbers or the heap becomes empty:
  - Pop the node with the smallest cost.
  - Output S0 − cost (the sum of the combination represented by this node).
  - Generate child A and/or child B when their conditions hold, and push them.
- If the heap empties before printing l lines, print −1 for the remaining lines.

Why this enumerates each combination exactly once:
- Any valid k-combination (indices i0 < i1 < … < i{k−1}) can be uniquely decomposed into shift amounts:
  t0 = i0 − 0, and for r ≥ 1, tr = ir − (i{r−1} + 1). These are non-negative integers with t0 ≤ t1 ≤ … ≤ t{k−1} ≤ s_max.
- The unique path to that combination performs child A exactly t0 times at p=0, then child B to open p=1, then child A exactly t1 times at p=1, and so on. We never go back to earlier positions, so there’s no duplicate path.

Why the order is correct:
- Both children have cost ≥ parent’s cost (child A increases cost; child B preserves it). Using a global min-heap thus pops nodes in non-decreasing penalty, hence the printed S0 − cost is in non-increasing sum order.

Step-by-step plan

1) Read n, k, l and the n values H.

2) Sort and truncate:
- Sort H in non-increasing order (or use nth_element to isolate the top T; see below).
- Compute T as the smallest t (k ≤ t ≤ n) such that C(t, k) ≥ l, by the iterative update:
  bin = 1; t = k;
  while t < n and bin < l:
    bin = bin * (t + 1) / (t + 1 − k);
    t++
  T = t
- Keep only the first T elements of the sorted array.

3) Precompute:
- S0 = sum of A[0..k−1] (64-bit).
- Differences D[i] = A[i] − A[i+1] for i = 0..T−2.

4) Best-first enumeration:
- Priority queue of nodes (cost, p, x). Start with (0, 0, 0).
- Let printed = 0.
- While printed < l and heap not empty:
  - pop (c, p, x)
  - print S0 − c; printed++
  - If x < T − k + p: push (c + D[x], p, x + 1)
  - If p + 1 < k and x < T − k + p: push (c, p + 1, x + 1)
- If heap empties and printed < l, print −1 for the remaining lines.

Implementation hints

- Sorting: If n is large and T is much smaller than n, do this:
  - Use nth_element to move the top T elements to the front in linear time.
  - Then sort only that prefix in non-increasing order. Complexity O(n + T log T), faster than sorting all n.

- 64-bit everywhere for sums: S0 and outputs fit in 64-bit signed (k ≤ 10^6 and H_i ≤ 10^9 ⇒ sum ≤ 10^15).

- Computing T: The update bin = bin * (t + 1) / (t + 1 − k) is exact when bin = C(t, k). Because l ≤ 10^6, bin never grows beyond ~10^6 before stop; bin * (t + 1) ≤ 10^12, so 64-bit is safe. If you want to be extra cautious, use 128-bit temporaries for the product.

- Memory: You store the top T values and D[0..T−2], both size ≤ 10^6. The heap stores O(l) nodes, each with three integers/long longs. Well within 1024 MB.

- Ties: Many sums may be equal (repeated values). The procedure emits duplicates automatically because distinct combinations can have the same penalty.

Correctness proof sketch

- Restriction to prefix:
  Let T be the smallest index with C(T, k) ≥ l. For any combination that uses an index ≥ T, replace that element by a not-yet-used element among the first T; the sum cannot decrease. Since there are at least l combinations fully inside the first T, we can always choose the top l sums from these. Therefore, computing the l largest sums among k-combinations of the first T elements suffices.

- Enumeration completeness and uniqueness:
  Every k-combination corresponds to a unique non-decreasing shift vector s of length k. The described tree fixes these shifts in order (position 0, then 1, etc.). The “child A” performs one more unit of shift at the current position; “child B” advances to the next position and sets it to the minimal feasible value. This yields a bijection between tree nodes popped from the heap and k-combinations.

- Order of output:
  The penalty of a child is never smaller than that of its parent. A global min-heap thus returns nodes in non-decreasing penalty, i.e., in non-increasing k-sum. The l values printed first are the l largest k-sums.

Complexity

- Sorting (or partial sort): O(n log n) in the simplest version; or O(n + T log T) with nth_element + sort on prefix.
- Preprocessing differences: O(T).
- Enumeration: Each output performs one pop and up to two pushes: O(l log l) time, O(l) extra space.

Given n, l ≤ 10^6, this fits easily in time and memory if implemented carefully (fast I/O, 64-bit arithmetic, and partial sort if helpful).

Pseudocode (high level)

- Read n, k, l, array H
- Sort H descending
- Find T by iteratively increasing t from k while C(t, k) < l and t < n, using bin = bin * (t+1)/(t+1−k)
- Truncate A = H[0..T−1]
- S0 = sum(A[0..k−1])
- For i in 0..T−2: D[i] = A[i] − A[i+1]
- Heap ← empty min-heap
- Push (cost=0, p=0, x=0)
- printed = 0
- While printed < l and Heap not empty:
  - (c, p, x) = pop min by c
  - print S0 − c
  - printed++
  - if x < T − k + p:
      push (c + D[x], p, x + 1)
      if p + 1 < k:
         push (c, p + 1, x + 1)
- While printed < l: print −1; printed++

Why it meets the limits

- The heavy work is heap-based generation: O(l log l) for up to 10^6 outputs is fine with a constant-factor-efficient implementation.
- Sorting only a prefix (optional) reduces cost substantially in cases where T ≪ n.
- Memory stays O(n) with small constants.

Walk-through on Sample 1

Input:
4 2 6
3 2 3 3
Sorted A: [3, 3, 3, 2]
Find T: minimal with C(T,2) ≥ 6 ⇒ T = 4
S0 = 3 + 3 = 6; D = [0, 0, 1]
Start (0,0,0) ⇒ print 6
Children: (0,0,1), (0,1,1)
Next pops at cost 0 produce sums 6,6, then more zeros, then next penalties become 1, giving sums 5, etc. The printed sequence is:
6,6,6,5,5,5 as in the sample.

This approach similarly reproduces the other samples.

Summary

- Sort values descending and restrict to the first T elements where T is the smallest index with C(T,k) ≥ l (or T = n if not achieved).
- The top l k-sums equal S0 minus the l smallest “penalties,” each penalty built by shifting indices to the right with non-decreasing shift amounts.
- Enumerate these penalties in increasing order using a best-first search over a simple two-child implicit tree with a min-heap; output S0 − penalty each time.
- Time O(n log n + l log l) (or O(n + T log T + l log l) with partial sort), memory O(n + l).
- Correctness follows from the positive-number truncation lemma, the one-to-one mapping between combinations and paths in the tree, and the monotone costs.