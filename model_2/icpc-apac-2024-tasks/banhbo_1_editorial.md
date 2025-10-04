Restatement

We color an r×c grid with red/white cells (two colors, indistinguishable per color). We call the coloring uniform if every 6×7 subgrid has the same number of red cells. Given that r is a multiple of 6 and c is a multiple of 7, count the number of uniform colorings modulo 998244353.

Key idea

Tile the r×c grid by aligned 6×7 blocks. Let R = r/6 and C = c/7, so there are M = R·C disjoint aligned 6×7 tiles that exactly partition the grid.

The crucial observation is that a 6×7 window sum depends only on how many cells it takes from each of the (up to) four aligned tiles it overlaps, and those overlaps are governed solely by the window’s offset relative to tile boundaries. Therefore, if all aligned tiles have the same total number of red cells, then every 6×7 window (aligned or not) has the same number of reds. Conversely, if all 6×7 windows have the same red count, then the aligned windows (which are themselves 6×7 tiles) must also have that same red count. This yields a complete characterization.

Formal characterization

- Let T[x,y] be the number of red cells in the aligned 6×7 tile whose top-left corner is at row 6x+1, column 7y+1, for x=0..R−1, y=0..C−1.

- Necessity (uniform ⇒ all T[x,y] equal):
  Consider the aligned windows (the ones starting at positions (6x+1, 7y+1)). They are exactly the tiles themselves, so uniformity forces T[x,y] to be the same for all x,y. Denote this common value by S.

- Sufficiency (all T[x,y] = S ⇒ uniform):
  Take any 6×7 window. Let its top-left corner be at (i0,j0), and write
  i0 = 6x + s with x ∈ [0..R−1], s ∈ [1..6], and
  j0 = 7y + t with y ∈ [0..C−1], t ∈ [1..7].
  This window intersects at most the four tiles (x,y), (x+1,y), (x,y+1), (x+1,y+1), with the following counts:
  - From tile (x,y): (7−s)·(8−t) cells
  - From tile (x+1,y): (s−1)·(8−t) cells
  - From tile (x,y+1): (7−s)·(t−1) cells
  - From tile (x+1,y+1): (s−1)·(t−1) cells

  The window’s total red count is the corresponding linear combination of tile totals:
  (7−s)(8−t)·T[x,y] + (s−1)(8−t)·T[x+1,y] + (7−s)(t−1)·T[x,y+1] + (s−1)(t−1)·T[x+1,y+1].

  If T[·,·] ≡ S, this becomes [(7−s)+(s−1)]·[(8−t)+(t−1)]·S = 6·7·S = 42S, independent of (i0,j0). Hence the coloring is uniform.

Thus:
A coloring is uniform if and only if every aligned 6×7 tile has the same number S of red cells.

Counting

- Each 6×7 tile has 42 cells. If a tile must contain exactly S red cells, the number of ways to color that tile is C(42, S).

- Tiles are independent except for the requirement that they all use the same S. With M = (r/6)·(c/7) tiles, the number of colorings with tile-sum S is C(42,S)^M.

- Summing over all possible S yields the total number of uniform colorings:
  Answer = sum_{S=0}^{42} C(42, S)^M modulo 998244353,
  where M = (r/6)·(c/7).

Algorithm

- Let R = r/6, C = c/7, M = R·C.

- Precompute binomial coefficients C(42, S) for S = 0..42 modulo 998244353. Because 42 is small, you can:
  - either precompute factorials/inverse factorials up to 42 and use C(42,S) = fact[42] · invfact[S] · invfact[42−S] mod p,
  - or build row 42 of Pascal’s triangle modulo p.

- For each S in 0..42:
  - Compute term = pow_mod(C(42,S), M, 998244353) by fast exponentiation.
  - Accumulate answer = (answer + term) mod 998244353.

- Output answer.

Pseudocode sketch

- read r, c
- R = r / 6
- C = c / 7
- M = R * C
- precompute comb[0..42] = C(42, S) mod p
- ans = 0
- for S in 0..42:
    ans = (ans + pow_mod(comb[S], M, p)) % p
- print ans

Complexity

- Precomputing C(42, S): O(42^2) or O(42) with factorials.
- The loop has 43 terms; each pow_mod takes O(log M) multiplications. With M ≤ 11111·11111 = 123,454,321, log2 M ≈ 27. Total time is negligible; memory usage is O(1).

Why it is correct

- Necessity is immediate from the uniformity applied to aligned 6×7 windows: all aligned 6×7 tiles must have the same red count S.

- Sufficiency follows from the overlap-count argument: any 6×7 window is made of portions of at most four aligned tiles with coefficients that sum to 42 regardless of the offsets. If each tile has S reds, every window has 42S reds, so the coloring is uniform.

- Counting leverages the independence between tiles once S is fixed: each tile can be colored in C(42,S) ways, independently across the M tiles. Summing over S counts all uniform colorings.

Sanity checks with samples

- r=6, c=7 ⇒ M=1 ⇒ sum_{S=0}^{42} C(42,S) = 2^42, matching the sample.
- r=12, c=14 ⇒ M=4 ⇒ sum_{S=0}^{42} C(42,S)^4, matching the sample.
- r=12, c=42 ⇒ M=12 ⇒ sum_{S=0}^{42} C(42,S)^12, matching the sample.
- r=42, c=14 ⇒ M=14 ⇒ sum_{S=0}^{42} C(42,S)^14, matching the sample.

This approach fits comfortably within the time and memory limits.