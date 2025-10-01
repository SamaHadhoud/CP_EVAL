Restatement

You must fill an array X of length N with signs −1 or 1. Some positions are already fixed. There are K constraints; each says that the sum of X on an interval [A, B] must be at least C. Among all valid arrays, output the lexicographically smallest one (i.e., prefer −1 as early as possible). If no valid array exists, output “Impossible”.

Key idea 1: Convert to a 0/1 problem

Let Y be a 0/1 array defined by Xi = 2·Yi − 1. Then:

- Yi = 0 corresponds to Xi = −1,
- Yi = 1 corresponds to Xi = +1,
- “lexicographically smallest X” becomes “lexicographically smallest Y” (prefer 0 as early as possible).

For an interval [A, B] of length L = B − A + 1:
sum_{i=A..B} Xi = 2·sum_{i=A..B} Yi − L.

So sum Xi ≥ C is equivalent to:
sum Yi ≥ T, where T = ceil((C + L)/2).

- If T ≤ 0: the constraint is vacuously true.
- If T > L: impossible (since sum Yi ≤ L always).

Prefilled positions:
- If Pi = 1, then Yi = 1 is fixed.
- If Pi = −1, then Yi = 0 is fixed.
- If Pi = 0, Yi is free.

Thus the problem becomes:
- Choose Yi ∈ {0, 1} (respecting fixed values).
- For each constraint [A, B], require sum Yi ≥ T = ceil((C + (B−A+1))/2).
- Among all feasible Y, minimize it lexicographically.

Key idea 2: Greedy “push ones to the right” with range deadlines

Lexicographically smallest Y means: put 1’s as late as possible. Each interval [A, B] with requirement T says: “by time B, pick at least T indices inside [A, B] to be 1”. This is a classic “at-least coverage by deadlines” pattern:

- Process positions from left to right (increasing R = B).
- When you reach R, ensure all constraints ending at R are satisfied.
- To satisfy a constraint [A, R] that still needs d extra ones, set to 1 the d rightmost available positions in [A, R].
- “Available” means: not fixed to 0 and not already chosen as 1. Fixed 1’s count automatically.

This guarantees feasibility if possible and yields the lexicographically smallest solution (zeros as early as allowed).

Data structures needed

- Group constraints by right endpoint B (i.e., store, for each B, the list of pairs (A, T) for intervals [A, B]).
- A Fenwick tree (or any BIT) over indices 1..N to maintain the current number of 1’s chosen; supports:
  - range sum query sum(A..B),
  - point update when choosing a position to become 1.
- An ordered set (balanced BST) S of indices that are still free and can be set to 1 later:
  - Initially S = { i | Pi = 0 }.
  - Remove i from S when we decide Yi = 1 (or if Pi was fixed to 1, i is not in S to begin with).
  - To pick the rightmost available in [A, R], we repeatedly take the largest element of S ≤ R and check if it is ≥ A; if so, use it; otherwise, the interval cannot be satisfied.

Greedy algorithm

Preprocessing:
- For each constraint (A, B, C): let L = B − A + 1, T = ceil((C + L)/2).
  - If T ≤ 0: ignore this constraint.
  - If T > L: immediately “Impossible”.
  - Otherwise, append (A, T) to list constraints_ending_at[B].
- Initialize BIT with 1 at i where Pi = 1 (fixed ones).
- Initialize S with indices i where Pi = 0 (free). Fixed zeros are never in S.

Main loop (R from 1 to N):
- For each (A, T) in constraints_ending_at[R]:
  - current = BIT.sum(A..R)  // number of 1’s already in [A, R] (includes fixed and previously chosen)
  - need = T − current
  - While need > 0:
    - Find j = largest index in S with A ≤ j ≤ R.
      - If such j does not exist → Impossible (not enough available spots).
    - Choose j: set Yj = 1, remove j from S, BIT.add(j, +1), need -= 1.

After finishing R = N, every constraint is satisfied. The remaining free positions are zeros. Finally, output Xi = 2·Yi − 1.

Pseudocode skeleton

- Read N, K, and P[1..N].
- Build vector constraints_ending_at[1..N].
- For each constraint (A, B, C):
  - L = B − A + 1
  - T = ceil((C + L)/2)  // compute carefully for negative values
  - if T ≤ 0: continue
  - if T > L: print “Impossible”
  - else constraints_ending_at[B].push_back(A, T)
- BIT init: for i with P[i] = 1, BIT.add(i, 1).
- S init: all i with P[i] = 0.
- For R = 1..N:
  - For each (A, T) in constraints_ending_at[R]:
    - current = BIT.sum(A..R)
    - need = T − current
    - While need > 0:
      - j = rightmost element of S with j ≤ R
      - if no such j or j < A: print “Impossible”
      - S.erase(j); BIT.add(j, +1); need -= 1
- For i = 1..N:
  - Yi = 1 if i not in S or P[i] = 1; Yi = 0 if P[i] = −1; otherwise Yi = 0 (still free)
  - Xi = 2·Yi − 1
- Print X[1..N]

Why this works

- Feasibility: When processing right endpoint R, the algorithm guarantees all constraints [A, R] have enough 1’s in [A, R]. It never removes 1’s afterward, so once satisfied they remain satisfied. After finishing R = N, every constraint is handled.
- Greedy optimality (lexicographically smallest):
  - We only add 1’s when a constraint demands it; thus the total number of 1’s in every prefix is minimal among all feasible solutions.
  - When we must add 1’s for [A, R], choosing the rightmost available positions within [A, R] can only help future constraints (which end later) and never hurts current ones (since they only require counts inside [A, R]). By an exchange argument, any feasible solution can be transformed to one that puts these necessary 1’s no earlier than ours. Therefore our Y is lexicographically smallest.
- Prefilled values are respected: fixed 1’s are counted from the start, fixed −1’s are excluded from availability forever.

Corner cases

- Constraints with T ≤ 0 are ignorable.
- If T > L, it’s immediately impossible (even without considering prefilled zeros; you simply cannot place more than L ones in L positions).
- The algorithm will also detect impossibility caused by prefilled zeros limiting available positions in an interval: while trying to add needed ones, S lacks enough indices in [A, R].

Complexity

- Each constraint is processed once; each time we add a 1 we remove an index from S permanently.
- Each BIT query/update is O(log N).
- Each removal from S is O(log N), and there are at most N removals in total.
- Total time: O((N + K) log N).
- Memory: O(N + K).
- This comfortably fits 1.0s and 256 MB for N, K ≤ 100000.

Implementation notes

- Carefully compute T = ceil((C + L)/2). With integers, use a safe rounding-up division for possibly negative C + L.
- Use 64-bit if you are worried about intermediate sums, although here values stay within ±2e5.
- Group constraints by their right endpoint B to process in a single left-to-right pass.

Why the solution is correct for all cases

- Any valid X corresponds to a valid Y and vice versa. The transformation preserves constraints and lex order.
- The algorithm enforces every constraint exactly at its deadline (right endpoint), adding the minimum number of ones needed at that moment and placing them as far right as possible. This ensures:
  - No constraint is ever violated.
  - For every prefix, the number of ones is minimized; hence the resulting Y (and thus X) is lexicographically smallest.
- If the algorithm fails to find enough available positions for some interval, then no completion exists that could satisfy that interval, so the original instance is impossible.

This yields the desired lexicographically smallest sign sequence or correctly reports “Impossible.”