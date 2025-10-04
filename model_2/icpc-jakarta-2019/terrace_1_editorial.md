Restating the problem

You have a 1×N line of cells, each either soil “.” or rock “#”. You can place non-overlapping mystical tiles:

- Type-1: 1×1 only on “.”, at most K tiles total, worth G1 each.
- Type-2: 1×2 on “..”, unlimited, worth G2 each.
- Type-3: 1×3 on pattern “.#.”, unlimited, worth G3 each.

Goal: maximize total ghosts repelled (sum of tile values), without covering any cell more than once. There are at most 50 rock cells.

Key ideas

1) Break the yard into soil segments separated by rocks
Let the positions of '#' be r1 < r2 < ... < rm (m ≤ 50). Define segments of soil between rocks and at the borders:

- Segment 1: cells 1..(r1−1)
- Segment j: cells (r_{j−1}+1)..(r_j−1) for 2 ≤ j ≤ m
- Segment m+1: cells (r_m+1)..N

Let L_j be the length (number of “.”) of segment j. There are m+1 segments, at most 51.

A Type-3 tile at rock j uses:
- The last cell of segment j (to the left)
- The rock cell itself
- The first cell of segment j+1 (to the right)

Hence, placing a Type-3 at rock j is possible iff L_j ≥ 1 and L_{j+1} ≥ 1. Also, if a segment has length 1 between two rocks, you cannot place Type-3 on both adjacent rocks (they would compete for the single soil end).

2) After choosing Type-3 tiles, what remains inside each segment is simple
Let x_j ∈ {0,1} indicate placing a Type-3 at rock j. For a segment of initial length L, with a = x_{left} and b = x_{right} indicating whether the Type-3 on its left/right rock eats one endpoint, the remaining soil cells in that segment is:
R = L − a − b, with R ≥ 0.

Inside a pure soil run of length R, the optimal use of Type-2 and Type-1 tiles is characterized by:
- D_segment = floor(R/2) dominoes (Type-2)
- O_segment = R mod 2 single leftover cell that can host a Type-1 without affecting dominoes.

Thus, over all segments:
- D = total number of Type-2 tiles = sum floor(R_j/2)
- O = total number of leftover single cells = sum (R_j mod 2)

These two numbers D and O summarize everything needed for the optimal Type-1 and Type-2 packing given the global budget K for Type-1.

3) How Type-3 choices affect D and O, locally
Crucially, the effect on each segment depends only on its length parity and whether its two endpoints are eaten.

Let L be the segment length; a,b ∈ {0,1}; define:
- O contribution: O_seg = (L + a + b) mod 2
- D contribution: D_seg = floor(L/2) − φ(L, a, b), where
  - If L is even: φ = 1 if a + b ≥ 1 else 0
  - If L is odd:  φ = 1 if a + b = 2 else 0

So the total D equals S − P, where:
- S = sum floor(L_j/2) over all segments (a constant independent of Type-3 choices)
- P = sum φ over all segments (the “penalty” = lost dominoes), with 0 ≤ P ≤ number of segments ≤ 51.

Also O = sum O_seg over all segments.

4) Coupling between adjacent Type-3 choices
- A Type-3 at rock j is allowed only if L_j ≥ 1 and L_{j+1} ≥ 1.
- If L_{j+1} = 1, you cannot have x_j = 1 and x_{j+1} = 1 simultaneously (they would overlap).

These are simple local constraints on adjacent x’s.

5) What about the Type-1 limit K?
Given totals D and O, the optimal use of Type-1 tiles is:

- You may place s1 Type-1 tiles on the O leftover single cells (0 ≤ s1 ≤ min(K, O)); this yields G1·s1 and does not affect D.
- You may also “break” some dominoes: replacing one Type-2 by two Type-1 uses 2 singles and changes value by Δ = 2·G1 − G2.
  - If Δ ≤ 0, never break dominoes.
  - If Δ > 0, you may break up to min(D, floor((K − s1)/2)) dominoes, gaining Δ per broken domino.

The subtlety: it’s not always optimal to fill all O leftover cells first; due to parity of K − s1, sometimes it’s better to leave one leftover unfilled to free up two singles to break an extra domino if Δ > G1. The safe way: for each final (D,O), simply try all s1 in 0..min(K,O) and take the best:
singles_gain(D, O) = max over 0 ≤ s1 ≤ min(K, O) of [ G1·s1 + max(0, 2·G1 − G2) · min(D, floor((K − s1)/2)) ]

Because O ≤ 51, this brute force over s1 is tiny.

6) Dynamic programming over rocks
Let x_j be the Type-3 decision at rock j. We build a DP along rocks from left to right. The state maintains:

- The previous rock’s decision prev = x_{j−1} ∈ {0,1}
- The cumulative O_count and P_count so far (each ≤ number of segments ≤ 51)
- The accumulated linear value so far:
  value_so_far = sum G3·x_j (unary) − G2·sum φ (pairwise penalties for segments finalized so far)

We process rocks one by one. When deciding x_j, we finalize segment j (between rock j−1 and j), whose (a,b) are (prev, x_j). That adds:
- o_inc = (L_j + prev + x_j) % 2 to O_count
- p_inc = φ(L_j, prev, x_j) to P_count
- val_inc = G3·x_j − G2·φ(L_j, prev, x_j) to the DP value

Transition constraints:
- If L_j = 1, the pair (prev=1, x_j=1) is forbidden (both Type-3 would overlap on that single dot).
- If x_j = 1, it must be feasible at that rock: L_j ≥ 1 and L_{j+1} ≥ 1; otherwise only x_j = 0 is allowed.

After processing all m rocks, we must also finalize the last segment m+1 with (a,b) = (x_m, 0) to update O_count, P_count, and value by −G2·φ for that last segment.

Finally, the real total value for a DP state is:
total = G2·S + value_so_far + singles_gain(D, O)
where S is the constant sum floor(L_j/2), D = S − P_count, and singles_gain computed as in section 5.

We take the maximum total over all final states.

Why this works

- Decomposing the yard into at most 51 soil segments is valid because each Type-3 only touches the two endpoint cells of its adjacent segments. After choosing Type-3 placements, the remaining tiling inside each segment is independent.
- For a run of R cells, the optimal number of Type-2 tiles is floor(R/2). Leftover single cells are exactly R mod 2. So segment contributions are exactly D_seg and O_seg.
- φ and O_seg formulas follow from simple parity/length casework (even/odd L; removing 0/1/2 endpoints). φ never exceeds 1 per segment, so P_count ≤ number of segments.
- Given totals D and O and a global Type-1 cap K, the only remaining decision is how many singles to place on leftover cells (s1) and how many dominoes to break. This depends only on the counts D and O, not on where they occur. Exhaustively trying s1 in [0..min(K,O)] is optimal and cheap.
- The DP over rocks is a standard chain DP with pairwise interactions (segments) and unary costs (Type-3 value), plus a simple adjacency constraint when a segment length is 1. It explores all feasible Type-3 configurations and accumulates O_count and P_count exactly.

Pseudocode (high level, no language specifics)

- Read input N, K, G1, G2, G3 and the string S[1..N].
- Collect rock positions r1..rm.
- Build segment lengths L[1..m+1].
- S_const = sum floor(L[i]/2). base = G2 * S_const.
- For each rock j, feasible[j] = (L[j] ≥ 1 and L[j+1] ≥ 1).

DP:
- Initialize dp_prev[prev][o][p] = −INF for prev in {0,1}, o,p in [0..SegCnt].
- dp_prev[0][0][0] = 0.
- For j = 1..m:
  - dp_cur[..] = −INF
  - For prev in {0,1}:
    - For all o,p where dp_prev[prev][o][p] > −INF:
      - For x in {0,1}:
        - If x == 1 and not feasible[j], continue.
        - If L[j] == 1 and prev == 1 and x == 1, continue. // overlap on length-1 segment
        - o2 = o + ((L[j] + prev + x) mod 2)
        - p2 = p + phi(L[j], prev, x) // using the φ rules above
        - v2 = dp_prev[prev][o][p] + G3 * x − G2 * phi(L[j], prev, x)
        - dp_cur[x][o2][p2] = max(dp_cur[x][o2][p2], v2)
  - swap(dp_prev, dp_cur)
- Finalize the last segment m+1:
  - best = −INF
  - For prev in {0,1} and all o,p with dp_prev[prev][o][p] > −INF:
    - o2 = o + ((L[m+1] + prev + 0) mod 2)
    - p2 = p + phi(L[m+1], prev, 0)
    - v2 = dp_prev[prev][o][p] − G2 * phi(L[m+1], prev, 0)
    - D = S_const − p2
    - singles_gain = max over s1 in [0..min(K, o2)] of:
        G1*s1 + max(0, 2*G1 − G2) * min(D, floor((K − s1)/2))
    - total = base + v2 + singles_gain
    - best = max(best, total)
- Output best.

Complexity

- Building segments: O(N).
- DP states: at most 2 × (m+1) × (O_max+1) × (P_max+1) with O_max, P_max ≤ m+1 ≤ 51. That’s about 2 × 51 × 52 × 52 ≈ 275k states; each has O(1) transitions. Very fast.
- The final singles scan: per final state at most 52 values of s1; number of final states is at most about 2 × 52 × 52 ≈ 5400, so ≈ 280k iterations. Also negligible.
- Memory: a couple of 2×52×52 arrays of 64-bit values (well under 1 MB).
- Time comfortably within 1 second.

Corner cases and correctness notes

- x_j = 1 is allowed only if both adjacent segments have ≥ 1 dot, guaranteeing the pattern “.#.” exists.
- If a segment length between two rocks is exactly 1, you can’t set both adjacent rocks to 1; the DP enforces it.
- If there are no rocks (m = 0), there’s a single segment of length N; the DP initialization/finalization handles this (only the last segment is processed).
- When 2·G1 ≤ G2, breaking dominoes is never profitable; the singles_gain routine automatically yields s1 = min(K, O) and no breaking. When 2·G1 > G2, the scan over s1 also handles parity nuances (sometimes skipping one leftover single enables an extra break and yields a better total, as in Sample 3).
- Use 64-bit integers for safety: totals can be up to about 2×10^8.

Why the solution is correct

- Any feasible tiling can be represented by:
  - A set of Type-3 decisions on rocks, subject to local feasibility and non-overlap on segments of length 1.
  - Independent fillings inside each remaining soil run with Type-2 and optionally some Type-1. The optimal Type-2 count is floor(R/2); any further use of Type-1 either fills leftover single cells (O) or replaces dominoes in pairs (limited by D).
- Thus, the global effect of Type-3 choices on Type-2/Type-1 capability is entirely captured by two totals (D and O), which are additive over segments and computable via simple local rules depending on (a,b) at segment endpoints.
- The chain DP enumerates all valid Type-3 configurations and computes the aggregate penalty P (so D = S − P) and the aggregate O. Because both are sums of pairwise contributions, no higher-order state is needed.
- Finally, for fixed (D,O,K), the best use of Type-1 across all segments is exactly the 1D optimization over s1 described, independent of where the segments are. This guarantees optimality.

Working through the samples

- Sample 1:
  - The DP finds it is optimal to place the Type-3 on the only rock; totals yield 75.
- Sample 2:
  - G2 is big, so Type-3 is not worth losing a domino; DP selects no Type-3; singles fill one leftover, total 210.
- Sample 3:
  - The best is to place the Type-3 on the first rock. Parity and K=2 make it better to use both singles to break one domino rather than filling the single leftover; total 160.

This matches the sample outputs and illustrates the parity nuance handled by the s1 scan.

Summary

- Compress into ≤ 51 soil segments.
- DP over rocks with state (prev Type-3, O_count, P_count), accumulating G3 and −G2·penalties; finalize last segment.
- Compute D = S − P; then add the optimal Type-1 contribution via a tiny scan over s1.
- Complexity O(N + m · 51 · 51), easily fits the limits.