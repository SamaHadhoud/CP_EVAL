Restatement

You are given an n×n matrix X with entries in {1,…,n}. You may change any entries (to values in {1,…,n}). Your goal is to make every row contain a duplicate (some value appears at least twice in that row) and every column contain a duplicate. Minimize the number of modified entries and output one optimal set of modifications.

Key observations

1) What is a “bad” row/column?
- Call a row bad if all its n entries are pairwise distinct (i.e., it currently has no duplicates). Similarly for a column.
- Because entries are in {1,…,n} and there are exactly n positions in a row/column, a bad row (or column) must be a permutation of 1,…,n. Conversely, any row/column that is not a permutation already has a duplicate and is “good.”

2) A clean lower bound
- To make a bad row good, you must modify at least one cell in that row (only changes in that row can alter its multiset of values). Hence the total number of modifications m must be ≥ the number of bad rows r.
- Similarly, m ≥ the number of bad columns c.
- Therefore m ≥ max(r, c).

3) A single change can fix both a row and a column
- If row i and column j are both bad (both permutations), then at (i, j) you can change the value X[i][j] to any value v ≠ X[i][j]. Because both the row and the column contain each value 1,…,n exactly once, this v already appears elsewhere in row i and elsewhere in column j. That one change creates a duplicate in row i and in column j simultaneously.

This suggests that max(r, c) is not only a lower bound but also achievable by a constructive strategy.

High-level strategy

- Let R = set of bad rows, r = |R|.
- Let C = set of bad columns, c = |C|.

We will make exactly max(r, c) changes.

- First, pair up min(r, c) row/column intersections (i in R, j in C) and fix both in one shot per pair.
- If r > c, there remain r - c bad rows; fix each of those by one additional change in some good column.
- If c > r, there remain c - r bad columns; fix each of those by one additional change in some good row.

Choosing values safely

There are three situations you need to handle; the value assignment is simple in each.

A) Intersection of a bad row and a bad column (i in R, j in C)
- Fact: both the row and the column are permutations. So any v in {1,…,n} with v ≠ X[i][j] already appears elsewhere in that row and elsewhere in that column.
- Choice: set X[i][j] := v where v ≠ X[i][j] (e.g., v = 1 + (X[i][j] mod n)).
- Result: this single change makes row i and column j both have duplicates.

B) Fixing an extra bad row i when r > c (use a good column j ∉ C)
- Goal: make row i have a duplicate without breaking column j (which is already good).
- If column j is not constant: choose any row s ≠ i with X[s][j] ≠ X[i][j], and set v := X[s][j].
  - Column j still has a duplicate for value v (at row s and at row i after the change).
  - Row i gains a duplicate because v appears elsewhere in row i (row i was a permutation initially).
- If column j is constant (all entries equal to w): choose any v ≠ w that also appears in row i (that exists since row i is a permutation). Then column j remains good (it still has n−1 copies of w, with n≥3 giving ≥2 copies), and row i gains a duplicate.

C) Fixing an extra bad column j when c > r (use a good row i ∉ R)
- Symmetric to (B).
- Choose any column p ≠ j with X[i][p] ≠ X[i][j], and set v := X[i][p].
  - Row i remains good (we duplicated a value already in that row).
  - Column j was a permutation; it contains value v at some other row s, so after the change column j has a duplicate.

Construction plan (pseudocode-style)

For each test case:
1) Read n and X.
2) Identify bad rows R and bad columns C.
   - Row i is bad if all X[i, *] are distinct.
   - Column j is bad if all X[*, j] are distinct.
   Let r = |R|, c = |C|.

3) If r = 0 and c = 0: output 0 and continue.

4) If r ≥ c:
   - Pairing step (do c changes):
     - Take the first c rows of R, call them R1 = [i1, i2, …, ic].
     - Pair them one-to-one with the bad columns C = [j1, j2, …, jc].
     - For each k = 1..c:
       - At (ik, jk), set v := 1 + (X[ik][jk] mod n).
       - Record modification (ik, jk, v).
       - This fixes row ik and column jk at once.
   - Remaining bad rows (r - c changes):
     - Let G = list of good columns (those not in C). There are g = n - c of them.
     - Assign the remaining bad rows one-to-one to distinct columns in G (possible because r - c ≤ n - c).
     - For each such pair (i, j):
       - If there exists s ≠ i with X[s][j] ≠ X[i][j], set v := X[s][j].
       - Else (column j is constant with value w), pick any position p ≠ j in row i and set v := X[i][p] (note v ≠ w because row i was a permutation).
       - Record modification (i, j, v).

5) If c > r (symmetric):
   - Pairing step (do r changes):
     - Take the first r rows of R, R1 = [i1..ir], and the first r columns of C, C1 = [j1..jr], pair one-to-one.
     - For each k = 1..r:
       - At (ik, jk), set v := 1 + (X[ik][jk] mod n).
       - Record modification (ik, jk, v).
   - Remaining bad columns (c - r changes):
     - Let H = list of good rows (those not in R). There are h = n - r of them.
     - Assign the remaining bad columns one-to-one to distinct rows in H (possible because c - r ≤ n - r).
     - For each such pair (i, j):
       - Choose a column p ≠ j with X[i][p] ≠ X[i][j]; set v := X[i][p].
       - Record modification (i, j, v).

Why this is correct

- Lower bound: As argued, m ≥ max(r, c). We must change at least once in every bad row and at least once in every bad column; a single change touches exactly one row and one column.

- Achievability:
  - Pairing step: For (i in R, j in C), both row and column are permutations. Any v ≠ X[i][j] appears elsewhere in that row and elsewhere in that column, so a single change creates duplicates on both.
  - When r > c:
    - We never touch a column in C again, so their duplicates created in the pairing step remain intact.
    - For each remaining bad row i, we touch exactly one good column j and change its cell in a way that keeps column j good (either copy an existing different value in that column or, if the column is constant, any other value still leaves duplicates). Since row i was a permutation, choosing v as described makes that row have a duplicate.
  - When c > r: symmetric argument using good rows to host the remaining bad columns.
  - No other rows/columns are harmed because we avoid touching them or we touch them in a way that preserves their duplicates.

Therefore the algorithm meets the lower bound, so it is optimal: m = max(r, c).

Complexity

- Detect bad rows/columns in O(n^2) using frequency arrays (size n).
- Building the modification list and choosing values uses scans over rows/columns; overall still O(n^2).
- Across all test cases, the total n^2 ≤ 10,000, so this easily runs within time and memory limits.

Notes and implementation hints

- Detecting “bad” is easy: a boolean seen[1..n] per row/column; if you ever see a duplicate, it is good; otherwise bad.
- In the pairing step, any v ≠ X[i][j] works; v = 1 + (X[i][j] mod n) is a convenient choice.
- In the leftover steps:
  - When fixing a bad row i via a good column j, try to find s ≠ i with X[s][j] ≠ X[i][j]. If such s exists, v := X[s][j]. Otherwise the column is constant; choose v := X[i][p] for some p ≠ j with X[i][p] ≠ X[i][j] (exists as row i was a permutation).
  - When fixing a bad column j via a good row i, choose p ≠ j with X[i][p] ≠ X[i][j], set v := X[i][p].
- Keep indices 1-based in the output and ensure v ∈ [1..n].

Why it handles all cases

- If there are no bad rows/columns, we output 0.
- If both exist, pairing guarantees each change fixes two constraints until one side is exhausted.
- If only bad rows (or only bad columns) exist, we fix each with exactly one change without breaking any column (or row). Corner cases such as constant rows/columns are safe because changing one cell still leaves at least two copies of the constant value when n ≥ 3.
- We never need to modify a row (or column) more than once in this construction, but even if you do, the above value choices still preserve duplicates.

Conclusion

- The minimum number of modifications equals max(#bad rows, #bad columns).
- A simple two-phase constructive algorithm achieves this bound and is easy to implement with careful choice of the new values as outlined above.