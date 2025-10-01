Restatement

You are given two integer arrays R[1..N] and C[1..N]. They define an N×N grid A where A[i][j] = R[i] + C[j]. A path moves between orthogonally adjacent cells. An even path is a path that visits only cells with even values. Each query gives two cells (ra, ca) and (rb, cb), and it is guaranteed that both endpoints are even. For each query, decide whether there exists an even path between the two cells.

Key observations

1) Parity structure is rank-1:
- A[i][j] is even if and only if R[i] and C[j] have the same parity.
- Define pR[i] = R[i] mod 2 and pC[j] = C[j] mod 2. Then A[i][j] is even ⇔ pR[i] = pC[j].

2) Where can edges exist?
- Horizontal edge between (i, j) and (i, j+1) exists iff both endpoints are even:
  pR[i] = pC[j] = pC[j+1].
  In particular, this requires pC[j] = pC[j+1] (no horizontal edges across a column-parity flip) and that common parity equals pR[i].
- Vertical edge between (i, j) and (i+1, j) exists iff
  pR[i] = pR[i+1] = pC[j].
  So no vertical edges across a row-parity flip, and the common parity must equal pC[j].

3) Impenetrable walls:
- If pC[j] ≠ pC[j+1], there is no horizontal edge across the boundary between columns j and j+1 anywhere in the grid (for any row).
- If pR[i] ≠ pR[i+1], there is no vertical edge across the boundary between rows i and i+1 anywhere in the grid (for any column).
Thus, the entire grid is sliced by these “parity flip” walls.

4) One-dimensional components:
- Rows split into maximal contiguous blocks where pR is constant. Call these row blocks.
- Columns split into maximal contiguous blocks where pC is constant. Call these column blocks.

5) Two-dimensional components are rectangles:
- Fix a row block S of parity x and a column block T of the same parity x.
- Every cell in S × T is even (since pR = x and pC = x).
- Moreover, within S × T, for every adjacent row pair (i, i+1) in S and every column j in T, vertical edges exist (because pR[i] = pR[i+1] = pC[j] = x); and for every adjacent column pair (j, j+1) in T and every row i in S, horizontal edges exist (pC[j] = pC[j+1] = pR[i] = x).
- Therefore, S × T is a single connected component under even moves.
- Conversely, no path can cross from S × T to any other block because that would require crossing a row- or column-parity flip (which has no edges anywhere).

Conclusion:
- The connected components of the even cells are exactly the rectangles S × T where S is a row block and T is a column block of the same parity.

Decision criterion per query

Let rowBlock[i] be the index of the row block containing row i (consecutive rows with the same pR). Let colBlock[j] be the index of the column block containing column j (consecutive columns with the same pC).

For a query (ra, ca) → (rb, cb), there exists an even path if and only if:
- rowBlock[ra] = rowBlock[rb], and
- colBlock[ca] = colBlock[cb].

Because each endpoint is guaranteed even, both blocks necessarily have the same parity, so no extra parity checks are needed for correctness under this problem’s guarantee.

Why this is necessary and sufficient
- Sufficiency: If both endpoints are in the same row block S and column block T, then both cells lie inside the rectangle S × T, which is a single connected component; a path exists (e.g., move horizontally within T, then vertically within S).
- Necessity: If rowBlock[ra] ≠ rowBlock[rb], then somewhere between ra and rb there is a row-parity flip. No vertical edges cross that flip anywhere, so no path can connect the two rows. Similarly, if colBlock[ca] ≠ colBlock[cb], a column-parity flip blocks all horizontal transitions across it. Thus both equalities are required.

Algorithm

Preprocessing (O(N)):
- Build parity arrays pR[i] = R[i] mod 2 and pC[j] = C[j] mod 2.
- Compute rowBlock:
  - Scan i = 1..N; start a new block whenever pR[i] ≠ pR[i−1].
- Compute colBlock similarly by scanning columns.

Answering queries (O(1) each):
- For each (ra, ca, rb, cb):
  - If rowBlock[ra] = rowBlock[rb] and colBlock[ca] = colBlock[cb], print YES; else NO.

Pseudocode

- Build row blocks:
  block = 1
  rowBlock[1] = block
  for i = 2..N:
    if pR[i] == pR[i-1]:
      rowBlock[i] = block
    else:
      block += 1
      rowBlock[i] = block

- Build column blocks analogously to get colBlock[j].

- For each query (ra, ca, rb, cb):
  if rowBlock[ra] == rowBlock[rb] and colBlock[ca] == colBlock[cb]:
    answer = "YES"
  else:
    answer = "NO"

Correctness proof

- From the impenetrable walls observation, any path that crosses a row parity flip or column parity flip is impossible, hence endpoints must lie within the same row block and the same column block.
- Within any S × T rectangle formed by a row block S and a column block T with equal parity, all adjacent edges exist, so S × T is connected. Therefore, if both block equalities hold, a path exists. Combined, this establishes necessity and sufficiency.

Complexity and limits

- Preprocessing: O(N) time and O(N) memory to store pR, pC, rowBlock, colBlock.
- Each query: O(1).
- Total: O(N + Q) time, O(N) memory. This comfortably fits the 1.0s time limit and 256MB memory limit for N, Q ≤ 100,000.

Notes

- The problem guarantees both endpoints are even; if that guarantee were absent, you would first check that pR[ra] = pC[ca] and pR[rb] = pC[cb]; otherwise the answer is immediately NO.
- Indices are 1-based as per the statement; be careful with boundaries when scanning.