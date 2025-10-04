Problem restatement (in my own words)
- There are n cats. Cat i currently has wealth w_i and refuses to ever go below l_i (l_i ≤ w_i).
- We may repeatedly move 1 unit of wealth from any cat i with current wealth > l_i to any other cat j.
- We want to redistribute so that every cat ends with at least l_i, and the poorest cat is as rich as possible. Output that maximum possible minimum wealth.

Key observations
1) Only the lower bounds l_i and the total wealth W = sum_i w_i matter.
   - Because we can transfer between any pair of cats without restriction, the only limitation is that no cat can go below its own l_i. Who gives to whom doesn’t matter; only the global pool of “extra” wealth above the l_i floors matters.

2) Feasibility criterion for a target minimum x:
   - If we want every cat to end with at least x, cat i must end with at least max(l_i, x).
   - Therefore a necessary condition is sum_i final_i ≥ sum_i max(l_i, x).
   - Since total wealth is conserved, the condition is: W ≥ F(x), where F(x) = sum_i max(l_i, x).
   - This condition is also sufficient: If W ≥ sum_i max(l_i, x), set target t_i = max(l_i, x) for all i. The total needed is at most W, and we can transfer from cats with w_i > t_i to those with w_i < t_i, never pushing anyone below l_i (because t_i ≥ l_i). So we can realize such a distribution with pairwise transfers.

3) The function F(x) = sum_i max(l_i, x) is monotone non-decreasing in x, piecewise linear. Thus the answer is the largest integer x with F(x) ≤ W.

Two equivalent ways to compute the answer

A) Water-filling (most intuitive, O(n log n))
Interpretation: Think of each cat i as a column with “floor height” l_i, and we pour E = W − sum l_i units of extra water evenly from the bottom up. The water level (common minimum) rises, but when it hits the next higher floor, more columns “join” the pool.

Algorithm:
- Let L be the array l_i sorted in nondecreasing order: L[1] ≤ L[2] ≤ ... ≤ L[n].
- Compute total extra E = W − sum L[i] (E ≥ 0 always).
- We raise the first k columns together from level L[k] to L[k+1] step by step.
  - To raise the first k columns by Δ = L[k+1] − L[k], we need need = k * Δ units of extra.
  - If E ≥ need, we pay it (E -= need) and continue to the next k.
  - Otherwise, we can’t reach the next floor; we stop here. The level becomes L[k] + floor(E / k).
- If we pass all steps (i.e., raise up to L[n]), any remaining E raises all n columns equally, so the final level is L[n] + floor(E / n).

This final level is exactly the maximum possible wealth of the poorest cat.

Why it works:
- It’s precisely the discrete version of equalizing the minimum subject to hard floors L[i]. The piles whose floor is below the current water level must be carried along; once everyone’s at least at L[k], you must keep the first k together until the next floor L[k+1] is reached, etc. This matches the piecewise-linear F(x) and the feasibility condition W ≥ F(x).

Pseudocode (sketch):
- Read n, arrays w, l.
- W = sum(w), sort l ascending, L = sorted l.
- E = W − sum(L)
- For k = 1..n − 1:
    need = k * (L[k+1] − L[k])
    if E ≥ need: E -= need
    else: answer = L[k] + floor(E / k); output answer; stop
- If loop finishes: answer = L[n] + floor(E / n); output answer

Careful with 64-bit integers:
- W and E can be up to about 3e14; use 64-bit types for sums and products like k * (L[k+1] − L[k]).

Time and memory:
- Sorting L costs O(n log n). The single pass is O(n). Over all test cases, with sum of n ≤ 3e5, this easily fits in time and memory limits.

B) Binary search on the answer (also fine)
- Lower bound: lo = min(l_i). Upper bound: hi = floor(W / n). (Because F(x) ≥ n*x, hence x ≤ W/n; and min l_i ≤ average ≤ W/n.)
- Preprocess: sort L and build prefix sums P where P[k] = sum_{i=1..k} L[i], and totalL = P[n].
- To evaluate F(x):
  - Let k = number of L[i] strictly less than x (k = lower_bound(L, x)).
  - Then F(x) = k * x + (totalL − P[k]).
- Binary search for the largest integer x with F(x) ≤ W.

This is O(n log n + n_checks * log n), with about 50 checks per test. It also fits comfortably.

Equivalence to the piecewise formula
With L sorted and suffix sums S[k] = sum_{i=k..n} L[i]:
- On interval x ∈ (L[k], L[k+1]] (k = 0..n, with L[0] = −∞, L[n+1] = +∞), exactly k floors are strictly below x, so
  F(x) = k*x + S[k+1].
  The feasibility k*x + S[k+1] ≤ W gives x ≤ (W − S[k+1]) / k (for k > 0).
- The best x in that interval is min(L[k+1], floor((W − S[k+1]) / k)), provided it also satisfies x > L[k]. The water-filling method is exactly scanning k and applying this formula.

Why the solution is correct
- Necessity: Any final distribution with everyone at least x must satisfy sum final_i ≥ sum max(l_i, x) = F(x). Since sum final_i = W, we must have W ≥ F(x).
- Sufficiency: If W ≥ F(x), set target t_i = max(l_i, x). The total needed is at most W. Because transfers are allowed between any pair and t_i ≥ l_i, we can move wealth from those with w_i > t_i to those with w_i < t_i by unit transfers without violating any lower bound. Hence we can realize min wealth at least x.
- Maximizing x under this monotone feasibility condition yields the optimal answer. The water-filling precisely constructs the maximal x by spending the extra E optimally.

Edge cases and pitfalls
- n = 1: The answer is w_1. The water-filling formula gives L[1] + floor((W − L[1]) / 1) = w_1.
- All l_i equal: You simply add floor(E / n) to that common value.
- Large numbers: Use 64-bit for W, E, prefix/suffix sums, and products like k * gap.
- When implementing the piecewise/suffix formula directly, ensure the candidate x in the k-th interval actually satisfies x > L[k] (strict). The water-filling method avoids off-by-one mistakes naturally.

Complexity
- Over all test cases, the dominant cost is sorting the l array once per test: O(n log n) with sum n ≤ 3e5. The scan is O(n). Memory usage is O(n).

Illustration on Sample 1
- w: [4, 2, 3, 6, 12], l: [3, 2, 3, 3, 10]
- Sort l: L = [2, 3, 3, 3, 10], sum L = 21, W = 27, extra E = 6.
- Raise k=1 from 2 to 3: need = 1*(3−2) = 1 ≤ E, E -> 5.
- k=2 and k=3 to next distinct floor (still 3): need = 0, E unchanged.
- k=4 from 3 to 10: need = 4*(10−3) = 28 > E = 5, so stop with answer = 3 + floor(5/4) = 4.

This matches the sample output.

In summary
- Reduce the problem to checking W ≥ sum max(l_i, x).
- Solve either by water-filling over the sorted l_i with extra E = W − sum l_i, or by binary search using prefix sums.
- Both are correct and efficient; the water-filling scan is particularly simple and fast in practice.