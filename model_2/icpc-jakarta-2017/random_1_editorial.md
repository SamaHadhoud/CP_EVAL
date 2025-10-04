Restatement

You repeatedly draw a number uniformly from 1..N. The process stops at the first time every number 1..N has appeared at least twice. You are given the first K draws A1..AK and you know the process hasn’t stopped yet (so at least one value still has < 2 occurrences). From this partial history, compute the expected number of additional draws until stopping.

Key observations

- Only the multiplicities 0, 1, and ≥2 matter.
  - Let x = number of values seen 0 times, y = number of values seen exactly 1 time, z = N − x − y = number seen ≥2 times.
  - The future behavior depends only on (x, y), not on which particular labels they are.
- One draw updates (x, y) as follows:
  - With probability x/N, you hit a zero: (x, y) → (x−1, y+1).
  - With probability y/N, you hit a one: (x, y) → (x, y−1).
  - With probability z/N, you hit a ≥2: (x, y) unchanged (self-loop).
- The process ends when x = 0 and y = 0.

Let E[x][y] be the expected additional number of draws starting from state (x, y). We want E[x][y] for the (x, y) computed from the input history.

Recurrence

Condition on the next draw:

E[x][y] = 1
          + (x/N) · E[x−1][y+1]
          + (y/N) · E[x][y−1]
          + (z/N) · E[x][y],   where z = N − x − y.

Rearrange the self-loop term:

(1 − z/N) · E[x][y]
  = 1 + (x/N) · E[x−1][y+1] + (y/N) · E[x][y−1]

But 1 − z/N = (x + y)/N, so:

E[x][y] = N/(x + y)
        + (x/(x + y)) · E[x−1][y+1]
        + (y/(x + y)) · E[x][y−1],  for x + y > 0
E[0][0] = 0.

Interpretation: Among draws, only a fraction (x + y)/N are “effective” (they hit a value that hasn’t reached 2). The first term N/(x + y) is the expected number of raw draws per “effective” step; the convex combination describes how the state changes given an effective hit goes to a zero or a one.

Important simplification (scaling)

Define F[x][y] = E[x][y] / N. Then:

F[x][y] = 1/(x + y)
        + (x/(x + y)) · F[x−1][y+1]
        + (y/(x + y)) · F[x][y−1],  with F[0][0] = 0.

Crucially, F no longer depends on N. Therefore E[x][y] = N · F[x][y].

This lets us precompute F once for all x, y with x + y ≤ 3000 and answer each test in O(1) after counting x and y.

DP order

The recurrence for F[x][y] depends on:
- F[x−1][y+1] (same s = x + y, larger y),
- F[x][y−1] (smaller s).

So compute by increasing s = x + y, and for each fixed s, iterate y from s down to 0. Then x = s − y.

Base cases are implicit:
- If x = 0, the term with F[x−1][y+1] has weight 0.
- If y = 0, the term with F[x][y−1] has weight 0.
- F[0][0] = 0.

Counting x and y from the partial history

You only need:
- y = number of labels that appear exactly once in A1..AK.
- x = N − U, where U = number of labels that appear at least once.

You never need counts above 2. While reading A:
- Maintain an array cnt[1..N] capped at 2 (0, 1, or 2). Also keep a list of touched indices to avoid clearing O(N) each test.
- For each a:
  - If cnt[a] == 0: cnt[a] = 1.
  - Else if cnt[a] == 1: cnt[a] = 2.
  - Else: leave at 2.
- After reading, U = number of touched indices with cnt ≥ 1; y = number of touched indices with cnt == 1; x = N − U.
- Reset only touched entries back to 0.

Pseudocode (high level, no language specifics)

Precomputation:
- Let MAXN = 3000.
- Create dpF as a triangular table for 0 ≤ x, y, x + y ≤ MAXN.
- dpF[0][0] = 0.
- For s in 1..MAXN:
    For y in s..0 step −1:
        x = s − y
        val = 1.0 / s
        if x > 0: val += (x / s) * dpF[x−1][y+1]
        if y > 0: val += (y / s) * dpF[x][y−1]
        dpF[x][y] = val

Per test case:
- Read N, K.
- Initialize an empty list touched; cnt[1..N] not cleared yet (lazy).
- For i in 1..K:
    a = Ai
    if cnt[a] == 0: cnt[a] = 1; add a to touched
    else if cnt[a] == 1: cnt[a] = 2
- U = 0; y = 0
- For each a in touched:
    if cnt[a] >= 1: U += 1
    if cnt[a] == 1: y += 1
    cnt[a] = 0   // clear for next test
- x = N − U
- Answer = N * dpF[x][y]
- Print with sufficient precision (e.g., 1e−9)

Complexity

- Precomputation: O(MAXN^2) ≈ 4.5 million states, constant work per state; memory ~ 36–72 MB depending on storage. Done once.
- Per test: O(K) time and O(min(N, K)) extra memory due to “touched” list. The constraint sum of K over all tests ≤ 100,000 keeps input processing fast.

Why this is correct

- Symmetry: Given only counts, labels are indistinguishable except by their current multiplicity (0, 1, ≥2). Thus the process is fully described by (x, y).
- The recurrence follows from conditioning on the next draw and using linearity of expectation. The self-loop term accounts precisely for draws that hit already-doubled labels, and solving for E[x][y] moves that mass to the left, yielding the N/(x + y) term.
- The DP order ensures all dependencies are available: smaller s are computed earlier; within equal s, larger y first provides F[x−1][y+1].
- Scaling argument shows E[x][y] = N · F[x][y], so a single precomputed F works for all N.

Sanity checks with samples

- N=1, K=0 → x=1, y=0 → F[1][0]=2 → E=1·2=2.000000000
- N=1, K=1 (A1=1) → x=0, y=1 → F[0][1]=1 → E=1·1=1.000000000
- N=3, K=0 → x=3, y=0 → F[3][0]=3.212962963 → E=3·F=9.638888889

These match the provided outputs.

Notes

- Use double precision; the required error 1e−6 is easily met.
- If desired, you can precompute up to the maximum N seen in input instead of 3000, but precomputing to 3000 once is fast enough.

This approach respects both the time (2 s) and memory (256 MB) limits and handles all possible initial histories consistent with the promise that stopping hasn’t occurred yet (i.e., x + y > 0).