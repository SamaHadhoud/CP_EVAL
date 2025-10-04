Restatement

You are given N numbers A1..N with deletion costs B1..N. From A you may delete any elements; the cost is the sum of their Bi. You want the remaining array V to be perfect: its maximum value must be the majority element, i.e., it appears strictly more times than all other remaining elements combined. Find the minimum total deletion cost to achieve this.

Key observations

1) Choose the final maximum x. If the final maximum is x, then:
- Every element with value > x must be deleted (otherwise the maximum would exceed x).
- Among elements equal to x, keeping more copies only helps: it increases the majority margin, and every Bi is positive, so keeping a copy of x saves its cost.
- Let ex be the number of copies of x you keep (ex ≥ 1). Then among elements with value < x, you can keep at most ex − 1 of them; otherwise x wouldn’t be a strict majority (ex > others → others ≤ ex − 1). To save as much cost as possible, the kept “others” should be the ex − 1 most expensive (largest Bi) among values < x.

2) Therefore, for a fixed x, in an optimal solution you keep all copies of x (call their total cost Sx) and then you keep the min(ex − 1, count of values < x) most expensive elements among those with value < x. But since you keep all copies of x, ex = cx (the total count of x in A). So for fixed x:
- You keep all cx copies of x: gain Sx (sum of their Bi).
- You keep Kx = min(cx − 1, Lx) items from the multiset of all values < x, where Lx is the count of those items. Gain is the sum of the Kx largest Bi among values < x.
- Everything else is deleted.

Since the total cost of all elements is constant T = sum Bi, minimizing deletion cost equals maximizing the saved cost:
saved(x) = Sx + sum of the Kx largest Bi among {i : Ai < x}.
Answer = T − max over x (saved(x)).

Crucial lemma (why we keep all copies of x)

Suppose for some feasible solution with final maximum x you delete a copy of x. All Bi > 0, so that loses saving Bi. Further, the majority constraint is on counts only: if you re-include that copy of x, the max stays x, ex increases by 1, the allowed “others” limit increases (ex − 1 grows), and the solution remains feasible (or becomes less restrictive). Hence any solution deleting a copy of x can be improved by re-inserting it. So in optimal solutions, for the chosen x, all its copies are kept.

Algorithm outline (offline sweep over values)

We must compute for every x that appears in A:
saved(x) = Sx + topK_sum(L<x, Kx), where Kx = min(cx − 1, count of L<x).

Do this in one left-to-right sweep over values:
- Group elements by value v: store the list of deletion costs (Bi) for all Ai = v. Also compute:
  - cv = number of items with value v.
  - Sv = sum of Bi over Ai = v.
- Maintain a multiset M of deletion costs of all elements with value < current v.
- For each v in increasing order:
  - If cv > 0:
    - Let Lcnt = |M| (how many elements have Ai < v).
    - K = min(cv − 1, Lcnt).
    - saved(v) = Sv + sum of the K largest values in M.
    - Update the best answer with saved(v).
  - Insert all costs Bi with Ai = v into M (so they are available when we move to v + 1).

The only nontrivial operation is “sum of the K largest numbers in a dynamic multiset M.” With Bi up to 3·10^5, we can maintain:
- A frequency Fenwick tree (BIT) over cost values (counts per cost).
- A sum Fenwick tree over cost values (sum of costs per bucket).

Then:
- totalCnt = number of items in M (query count BIT).
- totalSum = sum of all Bi in M (query sum BIT).
- To get sum of the K largest elements:
  - If K ≤ 0 → 0; if K ≥ totalCnt → totalSum.
  - Otherwise, let t = totalCnt − K be how many smallest items we exclude.
  - Find the smallest cost c such that the prefix count up to c is ≥ t (standard BIT “find_kth” on the count tree).
  - Let countBelow = prefix count up to c−1, sumBelow = prefix sum up to c−1.
  - Let takeAtC = t − countBelow (how many items at cost c belong to the excluded smallest t).
  - sumSmallest = sumBelow + takeAtC · c.
  - sumLargestK = totalSum − sumSmallest.

Use 64-bit integers for sums: N and Bi can be up to 3e5, so sums may exceed 32-bit.

Pseudocode (high level)

- Read N, arrays A[1..N], B[1..N].
- Let maxA = max(A).
- Buckets[v] = empty vector for v = 1..maxA.
- For i in 1..N:
  - Buckets[A[i]].push_back(B[i]).
  - S[A[i]] += B[i], C[A[i]] += 1.
- total = sum of all B[i].
- Initialize two Fenwick trees over cost domain 1..maxB (maxB = max(B)).
- bestSaved = 0.
- For v = 1..maxA:
  - Lcnt = countBIT.total().
  - If C[v] > 0:
    - K = min(C[v] − 1, Lcnt).
    - topK = queryTopK(K) using the two BITs as described.
    - bestSaved = max(bestSaved, S[v] + topK).
  - For each cost c in Buckets[v]:
    - countBIT.add(c, +1), sumBIT.add(c, +c).
- Answer = total − bestSaved.
- Print Answer.

Why this works

- For any chosen final maximum x, all values > x must be deleted; within values equal to x and less than x, the only constraint is the count inequality. With positive Bi, it’s always optimal to keep all copies of x (proven above).
- Among the remaining values < x, the only limitation is a count cap of cx − 1. To maximize saved cost, we must pick the most expensive items up to that cap. That is exactly what topK_sum computes.
- The sweep constructs M as the set of all elements with value < v before evaluating v, so the query indeed considers exactly the eligible “others.”
- Taking the maximum over all v that appear in the array enumerates all possible final maxima. The best saved(x) yields the minimum deletion cost T − saved(x).
- All operations preserve feasibility of the resulting subset; conversely, any optimal perfect array must correspond to one of these x and to the selection described, so the algorithm finds the optimum.

Complexity

- Building buckets: O(N).
- For each v (at most maxA ≤ 3·10^5), one query of sum of top K if C[v] > 0. There are at most N such queries (one per distinct value actually present).
- Each insertion into the BITs is O(log maxB). Each top-K query is O(log maxB) using the “find_kth” + a few prefix sums.
- Total time: O((N + number_of_queries) · log maxB) = O(N log maxB). With maxB ≤ 3·10^5, this is easily within 1 second in a compiled language.
- Memory: O(N + maxA + maxB), well within the limit.

Implementation notes

- Use 64-bit integers for all sums (total, S[v], BIT sums, outputs).
- If Bi were unbounded, you would coordinate-compress the Bi and build the BITs on the compressed ranks; here Bi ≤ 3·10^5 so direct indexing is fine.
- Handle edge cases:
  - If C[v] = 1, K = 0 → only keep the single copy of v and nothing else below v.
  - If Lcnt = 0, topK = 0.
  - If the original array is already perfect with the global maximum m, then Lcnt ≤ C[m] − 1 and saved(m) equals the sum of all Bi, giving answer 0 as desired.

Sanity check on samples

- Sample 1:
  - For x = 3: keep all three 3’s (save 2+2+2 = 6), L<x is empty, saved = 6, total = 9, answer = 3.
  - For x = 5: keep the single 5 (save 3), cannot keep any others, answer = 9 − 3 = 6. Minimum is 3.

- Sample 2:
  - The sweep computes saved(x) for each value x present and picks the maximum; result is 34 as given.

This approach is optimal, respects the strict-majority requirement, and runs comfortably within the time and memory limits.