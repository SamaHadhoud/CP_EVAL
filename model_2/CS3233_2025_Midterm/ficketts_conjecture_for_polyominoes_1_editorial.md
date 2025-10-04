Restatement

You are given a convex polyomino A: a finite union of unit squares (grid cells), edge-connected, such that every row intersection and every column intersection is a contiguous segment (HV-convex). A cell of A is on the border if not all of its 8 neighboring cells (in the king-move sense) also belong to A.

Make a second copy A1 of A (you may rotate by 0°, 90°, 180°, or 270°, but you must keep grid alignment; only integer translations are allowed). For any placement with non-empty overlap, define:
- X = number of border cells of A that lie inside A1
- Y = number of border cells of A1 that lie inside A

We must maximize X/Y over all allowed placements and print the result as a reduced fraction.

Key observations

1) Border via erosion (8-neighborhood):
- A cell is interior if its entire 3×3 block is in A.
- Let E = erosion of A by a 3×3 block (i.e., cells whose 8 neighbors are all inside A). Then border cells are exactly A \ E.
- For any overlay with A1, we have:
  X = |(A \ E) ∩ A1| = |A ∩ A1| − |E ∩ A1|
  Y = |(A1 \ E1) ∩ A| = |A ∩ A1| − |A ∩ E1|
  where E1 is the erosion of A1. So for a fixed placement, the ratio is:
  X/Y = (N − NE) / (N − N_E1)
  with N = |A ∩ A1|, NE = |E ∩ A1|, N_E1 = |A ∩ E1|.

2) Which rotations matter?
- For 0° (same orientation) or 180° (parallel orientation), A1 is a translate/rotate of A with axes aligned. For any translation vector v, the following bijection shows X = Y: a cell counted in X at position x corresponds to a cell counted in Y at x − v. Thus the ratio is constantly 1 for every overlap.
- Therefore, the only potentially larger ratio comes from 90°/270° rotations. Due to symmetry, considering 90° is enough (270° is the same after flipping the shift signs).
- So the final answer is max(1, best over 90°).

3) HV-convex structure = row/column intervals:
- In a convex polyomino, each row is a contiguous segment [Lr, Rr], and each column is [Tc, Bc].
- The eroded shape E is also HV-convex. Its row intervals are
  eLr = max(Lr, Lr−1, Lr+1) + 1, eRr = min(Rr, Rr−1, Rr+1) − 1
  (empty if r is top/bottom or if eLr > eRr).
- Similarly, eroded column intervals:
  eTc = max(Tc, T_{c−1}, T_{c+1}) + 1, eBc = min(Bc, B_{c−1}, B_{c+1}) − 1
  (empty if invalid).

4) Reduce 2D overlaps to sums of 1D interval overlaps:
- Let R be the number of rows, C be the number of columns of A.
- Consider A1 = A rotated by 90° CCW (dimensions C×R).
- For a translation (Δr, Δc) applied to A1, the area overlap N(Δr, Δc) can be written as a sum, over matching row/column indices, of 1D overlaps between intervals on the r-axis (rows).
- Precisely, define:
  - Row intervals of A: row j has [Lj, Rj] with length b = Rj − Lj + 1.
  - Column intervals of A: column k has [Tk, Bk] with length a = Bk − Tk + 1.
  - For a fixed column shift Δc, we pair row j of A with column k = j + Δc of A (if k is in bounds).
  - For 90° rotation, the row interval of A1 derived from row j of A is Jj(Δr) = [C−1 + Δr − Rj, C−1 + Δr − Lj] (same length b, shifted by Δr).
  - The overlap contributed by the pair (j, k) at a given Δr is the length of intersection between [Tk, Bk] and Jj(Δr).
- Therefore,
  N(Δr, Δc) = Σ over valid j ( overlapLen( [Tk, Bk], Jj(Δr) ) )
- The same structure holds for NE and N_E1 by replacing the relevant intervals with those of the eroded shape on the appropriate side:
  - NE(Δr, Δc): replace [Tk, Bk] by [eTk, eBk].
  - N_E1(Δr, Δc): replace [Lj, Rj] by [eLj, eRj] inside Jj(Δr).

A single pair (j, k) contributes a “tent” (triangular-with-plateau) function of Δr

Let A = [aL, aR] and B(Δr) = [Δr + bL0, Δr + bR0] be two intervals of lengths a and b (a = aR − aL + 1, b = bR0 − bL0 + 1). The set of Δr with non-zero overlap is a contiguous range [δL, δR] where:
- δL = aL − bR0
- δR = aR − bL0
Within this range, the overlap as a function of Δr is:
overlap(Δr) = min( m, Δr − δL + 1, δR − Δr + 1 ), where m = min(a, b)
This is a ramp up by +1 for m steps, then possibly a plateau, then a ramp down by −1 for m steps. The total number of integer Δr in [δL, δR] is a + b − 1, and the plateau length is (a + b − 1 − 2m), possibly zero.

We can add this tent to an accumulator over Δr using four O(1) “slope events”:
- Let slope[.] be a difference array of the slope of the value array over Δr (we scan Δr from left to right).
- For each tent with parameters (δL, δR, m):

  slope[δL] += +1
  slope[δL + m] += −1
  slope[δR − m + 1] += −1
  slope[δR + 1] += +1

If we then sweep Δr from the minimum to maximum feasible values (−(C−1) to (R−1)), maintaining the current slope and integrating it, we obtain the value array of overlaps for all Δr in O(R + C) time.

Putting it all together

We evaluate the three arrays simultaneously for each fixed Δc:
- Base overlaps N
- Erow overlaps NE (eroded columns against rows)
- Ecol overlaps N_E1 (columns against eroded rows)

From these, pointwise over Δr:
- numerator = N − NE
- denominator = N − N_E1
- If denominator > 0, update the answer with numerator/denominator.

Finally, take the maximum over all Δc, and compare with 1 (from the parallel rotations). Return the reduced fraction.

Pseudocode sketch (no code, just logic)

- Read R, C and the grid.
- Build row intervals Lr, Rr for j = 0..R−1, and column intervals Tc, Bc for c = 0..C−1.
- Build eroded intervals:
  - For rows j with 1 ≤ j ≤ R−2:
    eLr = max(L_{j−1}, L_j, L_{j+1}) + 1
    eRr = min(R_{j−1}, R_j, R_{j+1}) − 1
    if eLr > eRr, mark empty
  - For columns c with 1 ≤ c ≤ C−2:
    eTc = max(T_{c−1}, T_c, T_{c+1}) + 1
    eBc = min(B_{c−1}, B_c, B_{c+1}) − 1
    if eTc > eBc, mark empty
- Initialize the best ratio as 1/1.

- For Δc from −(R−1) to (C−1):
  - Create three slope arrays slopeN[Δr], slopeNE[Δr], slopeNE1[Δr] indexed over Δr in [−(C−1) .. (R−1)], all zero.
  - For j from max(0, −Δc) to min(R−1, C−1 − Δc):
    - k = j + Δc.
    - Base (N): A = [Tc, Bc], B0 from row j
      aL = Tc, aR = Bc, a = aR−aL+1
      bL0 = C−1 − Rj, bR0 = C−1 − Lj, b = Rj−Lj+1
      m = min(a, b), δL = aL − bR0 = Tc + Lj − (C−1), δR = aR − bL0 = Bc + Rj − (C−1)
      Add tent to slopeN using (δL, δR, m).
    - NE (eroded columns vs rows): if eTc..eBc exists:
      aL = eTc, aR = eBc, a’ = aR−aL+1; b, bL0, bR0 as above
      m = min(a’, b), δL = eTc + Lj − (C−1), δR = eBc + Rj − (C−1)
      Add tent to slopeNE with (δL, δR, m).
    - N_E1 (columns vs eroded rows): if eLj..eRj exists:
      aL = Tc, aR = Bc, a = aR−aL+1; b_e = eRj − eLj + 1
      bL0 = C−1 − eRj, bR0 = C−1 − eLj
      m = min(a, b_e), δL = Tc + eLj − (C−1), δR = Bc + eRj − (C−1)
      Add tent to slopeNE1 with (δL, δR, m).
  - Sweep Δr from −(C−1) to (R−1):
    - Integrate slope arrays to obtain values N[Δr], NE[Δr], NE1[Δr].
    - For each Δr:
      num = N − NE
      den = N − NE1
      If den > 0: update best ratio with num/den.

- Output the best ratio reduced to lowest terms.

Notes/intuition

- Why only 90°? For aligned copies (0° or 180°), X = Y for all translations by a simple bijection argument; hence the best ratio in those orientations is exactly 1. Thus only the “crossed” orientation (90°/270°) can improve it.
- Why triangles? Row/column overlaps under translation in one axis are exactly cross-correlations of 1D indicator intervals, which are triangular (with possible plateau). Summing many such contributions can be done in linear time per Δc using difference arrays of slopes.
- The border counts are handled by erosion because ∂A = A \ (A ⊖ 3×3). That turns “border overlap” into “overlap minus overlap with eroded,” drastically simplifying the counting.

Complexity analysis

- Preprocessing intervals and eroded intervals: O(R + C) after scanning the grid (O(R·C) to read).
- For each Δc, we process exactly the pairs (j, k) with k = j + Δc in bounds. Across all Δc these are all R·C row–column pairs. Each contributes O(1) events to three slope arrays (a small constant factor).
- For each Δc, we do one linear sweep over Δr in [−(C−1), …, (R−1)], which costs O(R + C).
- Total per test: O(R·C + (R + C)^2). With the given constraint Σ(R^3 + C^3) ≤ 10^7, this is easily within time (note that for integers n ≥ 1, n^2 ≤ n^3, hence Σ(R + C)^2 is dominated by Σ(R^3 + C^3) up to a constant factor). Memory is O(R + C).

Correctness argument

- For any fixed rotation and translation, N(Δr, Δc) computed by summing 1D interval overlaps equals the exact number of overlapping cells, because the polyomino is HV-convex and every row/column is a single interval. The rotation-by-90° mapping turns “row of A1” into a translation of “row of A” along the row axis, while “column of A” stays a column interval, and overlap is exactly the number of integer row indices in the intersection of these two intervals.
- The identities X = N − NE and Y = N − N_E1 follow directly from ∂A = A \ E and ∂A1 = A1 \ E1, hence we are computing the exact numerator and denominator for each placement.
- We scan all valid Δc and Δr, so the maximization is exact. We also compare against 1 (achievable by aligned copies), so the final answer is the true maximum over all allowed rotations.
- The ratio is bounded above by 2 for convex bodies (Fickett’s conjecture; in the discrete HV-convex setting the same bound holds). Our algorithm does not rely on this bound, but it explains why answers like 2/1 (e.g., rectangles) and 1/1 (highly symmetric shapes) are typical and why the maximum is finite.

Pitfalls and implementation hints

- Be careful with indices and ranges:
  - Δc ranges from −(R−1) to (C−1).
  - Δr ranges from −(C−1) to (R−1).
  - Use an index shift when storing slope arrays so you can use 0-based arrays.
- Skip adding a tent if the eroded interval on that side is empty (length ≤ 0).
- Use 64-bit integers for all counts; areas can be up to R·C and you sum many contributions.
- Reduce the final fraction by gcd before printing.
- There is no need to consider 270° separately if you sweep Δc over its full negative-to-positive range, since it just mirrors the shift.

Sanity checks

- Full rectangle R×C: The best orientation is 90°, and the algorithm typically finds a ratio of 2/1 (as in the sample) when both dimensions ≥ 2.
- Highly symmetric shapes (e.g., nearly round, balanced profiles): The best ratio often turns out to be 1/1.

This approach stays within the time and memory limits and computes the exact maximum fraction.