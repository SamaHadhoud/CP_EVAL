Restatement

You start at level 0. There are N quests; each quest i requires you to have at least level Mi to take it, and upon completion gives Xi more levels. Each quest has a trauma value Ti. You can do each quest at most once.

Your trauma rating is the sum of the K largest Ti among the quests you actually do (if you do fewer than K quests, it’s the sum of all their Ti). You must reach level at least L. Among all valid ways to reach L (respecting the Mi constraints and choosing an order), minimize the trauma rating.

Key observations

- The order matters only because each quest requires your current level to be at least Mi. Doing an easier quest can unlock harder ones.
- The trauma rating is not the sum of all Ti you used; it’s the sum of the K largest ones among them (“top-K sum”).
- Ti are small: 1 ≤ Ti ≤ 150. N ≤ 150, but L and Mi can be large (up to 1.5e11), so we cannot DP over level. We need a DP dimension that stays small.

Turning top-K into an additive cost (the λ-trick)

The main difficulty is the “sum of largest K” objective. There is a classic transformation that converts the top-K sum into a standard additive sum, parameterized by a scalar λ ≥ 0:

For any multiset of nonnegative numbers {a1, a2, …}, the sum of the largest K elements equals:
min over λ ≥ 0 of [K·λ + sum over i of max(0, ai − λ)].

Why this is true (sketch):
- Sort a in nonincreasing order: a1 ≥ a2 ≥ … ≥ ar ≥ 0.
- For any λ in [aK+1, aK] (defining aK+1 = 0 if r < K), exactly the top K elements exceed λ, and the rest do not. Then:
  K·λ + sum max(0, ai − λ) = K·λ + sum_{i=1..K} (ai − λ) = sum_{i=1..K} ai,
  which is the top-K sum.
- For other λ ≥ 0, the expression is at least this value, so taking the minimum over λ yields the top-K sum.

Using this identity, if we choose a set of quests S, its trauma rating equals:
min over λ ≥ 0 of (K·λ + sum_{i ∈ S} max(0, Ti − λ)).

Therefore the optimal answer over all valid S equals:
min over λ ≥ 0 of [K·λ + min over valid S of sum_{i ∈ S} max(0, Ti − λ)].

Crucially, for a fixed λ, each quest i contributes an additive “cost”
ci(λ) = max(0, Ti − λ),
and our objective becomes: choose and order quests to reach L while minimizing the sum of these costs; then add K·λ at the end, and finally minimize over λ.

Since Ti ≤ 150, it suffices to try integer λ in {0, 1, 2, …, max Ti} (at most 151 values). For any λ in this set, ci(λ) ∈ [0, 150], and all costs are nonnegative.

Reduced problem for fixed λ

Given:
- Initial level 0,
- Quests with requirement Mi, gain Xi, and nonnegative cost ci,
minimize the total cost to reach level ≥ L, where a quest can only be taken after the current level ≥ Mi, and each quest can be taken at most once.

Solving the reduced problem: DP over total cost

We cannot DP over level (too large), but we can DP over total cost because the maximum total cost is small:
- For fixed λ, each ci ≤ 150.
- In the worst case we take all N quests, so the total cost is at most 150·N ≤ 22500.

Sort quests by Mi in nondecreasing order. This is safe because any quest with higher requirement cannot be used to unlock a quest with a lower requirement; to become eligible for a quest with requirement Mj, you only need quests with requirement ≤ Mj.

Define a DP over cost budgets:
- DP[b] = the maximum level reachable with total cost exactly b using only quests from the prefix processed so far.
- Initialize DP[0] = 0, and DP[b] = −∞ (unreachable) for b > 0.
- Process quests in nondecreasing order of Mi. For each quest (Mi, Xi, ci):
  - You may skip it: DP’[b] = max(DP’[b], DP[b]).
  - You may take it from any state with DP[b] ≥ Mi: then
    DP’[b + ci] = max(DP’[b + ci], DP[b] + Xi).
  - After processing, DP ← DP’.
- Clamp DP values to at most L (once you reach L, extra level doesn’t matter) to keep numbers small.

At the end (after all quests), the minimal cost to reach L is:
min b such that DP[b] ≥ L.

For this λ, the total trauma bound is: K·λ + (that minimal cost).
Take the minimum over all λ ∈ {0, …, max Ti}.

Pseudocode (high-level)

- Read N, K, L and quests (Mi, Xi, Ti).
- Sort quests by Mi ascending.
- ans = +∞
- For λ in 0..maxTi:
  - For each i: ci = max(0, Ti − λ).
  - B = sum of all ci (upper bound for total cost; or use a fixed bound 150*N).
  - DP[0..B] = −∞; DP[0] = 0
  - For each quest in sorted order:
    - DP’ = DP (or update DP in decreasing b to reuse one array)
    - For b from 0 to B − ci:
        If DP[b] ≥ Mi:
           DP’[b + ci] = max(DP’[b + ci], min(L, DP[b] + Xi))
    - DP = DP’
  - bestCost = min{ b | DP[b] ≥ L }
  - ans = min(ans, K·λ + bestCost)
- Output ans

Correctness

- The λ-trick: For any chosen set S, the top-K sum equals minλ≥0 [K·λ + sum_{i∈S} max(0, Ti − λ)]. Therefore minimizing trauma over all valid schedules S equals minλ≥0 [K·λ + (minimum over valid S of sum ci(λ))].
- For fixed λ, the DP finds the minimum sum of ci among all subsets that can be scheduled to reach L:
  - Sorting by Mi ensures that when considering quest j, any feasible way to make DP[b] ≥ Mj uses only earlier quests (with requirements ≤ Mj). Thus taking quest j is allowed precisely when DP[b] ≥ Mj.
  - The DP explores all 0/1 choices with that feasibility filter, so it computes the best achievable level for every cost b. Taking the smallest b with level ≥ L yields the optimal cost under λ.
- Combining both steps yields the exact minimal trauma rating.

Handling all cases

- If you end up using ≤ K quests, the trauma rating is the sum of their Ti. This is handled automatically: choose λ = 0. Then ci = Ti and K·λ = 0, so the DP minimizes the sum of all Ti you use.
- If you use more than K quests, the λ that minimizes K·λ + sum max(0, Ti − λ) will sit between the K-th and (K+1)-th largest Ti of your optimal set, reproducing exactly the top-K sum.
- Mi can be 0: these quests are available initially, DP captures them.
- Xi and L can be large: we never iterate over level values; we only store “current best level” per cost as a 64-bit integer, and clamp it at L to keep numbers bounded.
- Nonnegativity: ci ≥ 0 so the DP over costs is well-defined and monotone.

Complexity and limits

- There are at most maxTi + 1 ≤ 151 values of λ to try.
- For each λ, total cost bound B ≤ N · 150 ≤ 22500.
- The DP per λ runs in O(N · B) transitions, i.e., at most about 150 · 22500 ≈ 3.4×10^6 updates.
- Over all λ, about 151 · 3.4×10^6 ≈ 5.1×10^8 simple updates. With standard low-constant implementations (descending b to use one array, clamping levels, cutting b at the best known cost for which level ≥ L), this fits within 2 seconds in compiled languages and trivially within the 1024 MB memory limit (DP array ~ 22501 entries of 64-bit integers).

Practical implementation tips

- Iterate λ only over 0..maxTi (or even only the distinct Ti values plus 0); both are ≤ 151.
- Maintain an upper bound on the best cost found so far for this λ (the smallest b with DP[b] ≥ L) and never update states beyond it. This prunes many transitions in practice.
- Update DP in decreasing b to avoid a second array and forbid reusing the same quest multiple times.
- Cap DP[b] at L to reduce value sizes.
- Store levels in 64-bit integers.

Why this works on the samples

Sample 1:
- For λ = 6, the induced costs are c = [0, 3, 0, 0, 1].
- A feasible low-cost schedule is: do quests with T = 7 (cost 1, +4 levels), 3 (cost 0, +2), 5 (cost 0, +3), 6 (cost 0, +4) in an order respecting Mi; after reaching L = 10, total cost = 1. Then K·λ = 2·6 = 12; total = 13 (optimal).

Sample 2:
- The algorithm will find the minimum over λ, which evaluates to 13.

Conclusion

- Use the λ-trick to linearize the top-K trauma sum into an additive cost with a fixed offset K·λ.
- For each λ, solve a knapsack-like DP over total cost that tracks the maximum reachable level while respecting the Mi constraints (by sorting quests by Mi and allowing transitions only from states with DP[b] ≥ Mi).
- Take the minimum K·λ + bestCost over λ ∈ [0, max Ti].

This approach is correct, handles all edge cases, and fits comfortably in the given time and memory limits.