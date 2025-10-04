Restatement

You are given an array a[1..n]. For every subarray a[i..j], take its minimum m(i, j) and maximum M(i, j), and sum m(i, j) · M(i, j) over all subarrays. Output the result modulo 1 000 003 233. Constraints: n ≤ 3e5, 1 ≤ a[i] ≤ 1e9.

Naive O(n^2) is impossible. Also, the standard tricks for summing all minimums or all maximums separately do not directly yield the sum of their products, because min and max interact nonlinearly.

Key idea: Divide-and-conquer on indices + four-way partition per fixed left endpoint

A classic way to sum something over all subarrays is divide-and-conquer on the index range:

- Split the array [l..r] at mid = (l + r) / 2.
- Recursively solve for the left half [l..mid] and right half [mid+1..r].
- Add the contribution of subarrays crossing the cut, i.e., i ≤ mid < j.

The only nontrivial part is computing the “cross” sum fast:
Sum over i in [l..mid], j in [mid+1..r] of min(a[i..j]) · max(a[i..j]).

If we define for any i ≤ mid and j ≥ mid+1:
- Lmin[i] = min over a[i..mid]
- Lmax[i] = max over a[i..mid]
- Rmin[j] = min over a[mid+1..j]
- Rmax[j] = max over a[mid+1..j]

then for each crossing pair (i, j),
min(a[i..j]) = min(Lmin[i], Rmin[j])
max(a[i..j]) = max(Lmax[i], Rmax[j])

For a fixed i, as j increases,
- Rmin[j] is nonincreasing,
- Rmax[j] is nondecreasing.

Let
- t1(i) = the largest j ∈ [mid+1..r] such that Rmin[j] ≥ Lmin[i] (or mid if none),
- t2(i) = the largest j ∈ [mid+1..r] such that Rmax[j] ≤ Lmax[i] (or mid if none).

These cut the j-axis into at most four regions where the expression min(Lmin[i], Rmin[j]) · max(Lmax[i], Rmax[j]) simplifies:

- Region A: j ∈ [mid+1 .. min(t1, t2)]
  Here Rmin[j] ≥ Lmin[i] and Rmax[j] ≤ Lmax[i], so
  min = Lmin[i], max = Lmax[i], product = Lmin[i] · Lmax[i] (constant).

- Region B: if t1 < t2, j ∈ (t1 .. t2]
  Here Rmin[j] < Lmin[i] but Rmax[j] ≤ Lmax[i], so
  min = Rmin[j], max = Lmax[i], product = Rmin[j] · Lmax[i].

- Region C: if t2 < t1, j ∈ (t2 .. t1]
  Here Rmin[j] ≥ Lmin[i] but Rmax[j] > Lmax[i], so
  min = Lmin[i], max = Rmax[j], product = Lmin[i] · Rmax[j].

- Region D: j ∈ (max(t1, t2) .. r]
  Here Rmin[j] < Lmin[i] and Rmax[j] > Lmax[i], so
  min = Rmin[j], max = Rmax[j], product = Rmin[j] · Rmax[j].

This partition is the crux: for a fixed i, the contribution is a constant term over A, a “sum of Rmin” over B times Lmax[i], a “sum of Rmax” over C times Lmin[i], and a “sum of Rmin · Rmax” over D.

Therefore, if we precompute along the right side:
- prefix sums Pmin[j] = sum of Rmin from mid+1 to j,
- prefix sums Pmax[j] = sum of Rmax from mid+1 to j,
- prefix sums Pprod[j] = sum of Rmin[k] · Rmax[k] from mid+1 to j,

we can get each region’s contribution in O(1) time per i, provided we know t1(i) and t2(i).

Finding t1(i), t2(i) for all i with two pointers

Process i from mid down to l. As i decreases:
- Lmin[i] is nonincreasing,
- Lmax[i] is nondecreasing.

Hence:
- t1(i) (largest j with Rmin[j] ≥ Lmin[i]) is nondecreasing in i,
- t2(i) (largest j with Rmax[j] ≤ Lmax[i]) is nondecreasing in i.

So we can maintain two pointers j1, j2, initially at mid, and for each i move j1 right while Rmin[j1+1] ≥ Lmin[i], and move j2 right while Rmax[j2+1] ≤ Lmax[i]. Across the whole left sweep, j1 and j2 each advance at most (r - mid) times. This makes the crossing part linear in segment length.

Putting it together for one merge step [l..r] with mid:

- Build Lmin[i], Lmax[i] for i = mid, mid-1, …, l:
  Lmin[mid] = a[mid], Lmin[i] = min(Lmin[i+1], a[i]).
  Lmax[mid] = a[mid], Lmax[i] = max(Lmax[i+1], a[i]).

- Build Rmin[j], Rmax[j] for j = mid+1, …, r:
  Rmin[mid+1] = a[mid+1], Rmin[j] = min(Rmin[j-1], a[j]).
  Rmax[mid+1] = a[mid+1], Rmax[j] = max(Rmax[j-1], a[j]).

- Build prefix sums on the right:
  Pmin[mid] = Pmax[mid] = Pprod[mid] = 0.
  For j = mid+1..r:
    Pmin[j]  = Pmin[j-1]  + Rmin[j]  (mod M)
    Pmax[j]  = Pmax[j-1]  + Rmax[j]  (mod M)
    Pprod[j] = Pprod[j-1] + Rmin[j] · Rmax[j] (mod M)

- Two pointers:
  j1 = j2 = mid
  For i = mid down to l:
    while j1 < r and Rmin[j1+1] ≥ Lmin[i]: j1++
    while j2 < r and Rmax[j2+1] ≤ Lmax[i]: j2++
    let jA = min(j1, j2)
    Contribution for i:
      A: Lmin[i]·Lmax[i] · (jA - mid)
      B: if j2 > j1: Lmax[i] · (Pmin[j2] - Pmin[j1])
      C: if j1 > j2: Lmin[i] · (Pmax[j1] - Pmax[j2])
      D: Pprod[r] - Pprod[max(j1, j2)]
    Add all four (where applicable) to the answer modulo M.

- The merge contribution is the sum over i. The total for [l..r] is:
  solve(l, mid) + solve(mid+1, r) + merge_cross(l, mid, r).

Base case: for l == r, the only subarray is [l, l], contribution a[l]^2.

Pseudocode (high level)

solve(l, r):
  if l == r: return a[l] * a[l] mod M
  mid = (l + r) // 2
  ans = solve(l, mid) + solve(mid+1, r) mod M

  // left arrays
  Lmin[mid] = a[mid], Lmax[mid] = a[mid]
  for i = mid-1 down to l:
    Lmin[i] = min(Lmin[i+1], a[i])
    Lmax[i] = max(Lmax[i+1], a[i])

  // right arrays
  Rmin[mid+1] = a[mid+1], Rmax[mid+1] = a[mid+1]
  for j = mid+2 to r:
    Rmin[j] = min(Rmin[j-1], a[j])
    Rmax[j] = max(Rmax[j-1], a[j])

  // prefix sums on right
  Pmin[mid] = Pmax[mid] = Pprod[mid] = 0
  for j = mid+1 to r:
    Pmin[j]  = (Pmin[j-1]  + Rmin[j]) mod M
    Pmax[j]  = (Pmax[j-1]  + Rmax[j]) mod M
    Pprod[j] = (Pprod[j-1] + Rmin[j] * Rmax[j]) mod M

  // two pointers over right
  j1 = j2 = mid
  for i = mid down to l:
    while j1 < r and Rmin[j1+1] >= Lmin[i]: j1++
    while j2 < r and Rmax[j2+1] <= Lmax[i]: j2++

    jA = min(j1, j2)

    // Region A
    ans += Lmin[i] * Lmax[i] * (jA - mid)

    // Region B
    if j2 > j1:
      ans += Lmax[i] * (Pmin[j2] - Pmin[j1])

    // Region C
    if j1 > j2:
      ans += Lmin[i] * (Pmax[j1] - Pmax[j2])

    // Region D
    ans += (Pprod[r] - Pprod[max(j1, j2)])

    ans %= M

  return ans

Important implementation notes

- Arithmetic width: a[i] ≤ 1e9, so products like Rmin · Rmax can be up to 1e18, and you will multiply these by counts as large as O(n). Use 128-bit intermediates for products before reducing modulo, then cast back to 64-bit. All comparisons (for mins/maxes) use original values, not modulo.

- Modulo: M = 1 000 003 233 is used only for additions/subtractions/multiplications. No division needed.

- Memory: The recursion is O(log n) deep. At each level you allocate O(r - l + 1) workspace (Lmin/Lmax on the left part; Rmin/Rmax/Pmin/Pmax/Pprod on the right part). Since recursive calls free their temporaries before siblings allocate theirs, peak memory is O(n). This fits easily into the 1024 MB limit.

- Time: Each level does O(r - l + 1) work; depth is O(log n). Total O(n log n), which is fine for n = 3e5 with an efficient implementation.

Why it works (correctness)

- Every subarray is counted exactly once: Either it lies entirely inside [l..mid], entirely inside [mid+1..r], or crosses the cut. The recursive calls handle the first two disjoint sets; the merge handles all and only the cross subarrays.

- For a fixed left endpoint i and any right endpoint j ≥ mid+1, the minimum over a[i..j] equals min(Lmin[i], Rmin[j]), and the maximum equals max(Lmax[i], Rmax[j]) by definition. Hence the target term is exactly min(Lmin[i], Rmin[j]) · max(Lmax[i], Rmax[j]).

- The partition into the four regions A/B/C/D is exhaustive and disjoint, and follows from monotonicity:
  - Rmin[j] decreases with j, so once Rmin[j] falls below Lmin[i], it stays below.
  - Rmax[j] increases with j, so once Rmax[j] exceeds Lmax[i], it stays above.
  Therefore there are at most two single “last j” thresholds t1, t2, creating the four simple ranges. Within each range the min-part and max-part are fixed sources (left or right), so the product is either constant, or linear in Rmin, or linear in Rmax, or the product Rmin·Rmax, exactly as used.

- The two-pointer process gives the correct thresholds t1(i), t2(i): as i moves left, Lmin[i] can only decrease and Lmax[i] can only increase, so the sets {j: Rmin[j] ≥ Lmin[i]} and {j: Rmax[j] ≤ Lmax[i]} can only expand to the right; thus t1 and t2 are nondecreasing and can be tracked in total linear time.

- Summing the prepared prefix sums over those ranges yields the exact cross contribution.

Edge cases

- Equal values (plateaus): Using ≥ for Rmin and ≤ for Rmax assigns equality to the “left” side (region A when both equal), which is consistent and prevents double counting.

- Single element segment: Base case returns a[l]^2, which is min=max=a[l].

Complexity

- Time: O(n log n).
- Space: O(n).
- Fits the 1.0 s / 1024 MB constraints in a compiled language when implemented carefully (linear-time per level, constant-factor small).

This approach directly targets the interaction between minima and maxima and avoids any O(n^2) enumeration, while staying exact and robust for all arrays.