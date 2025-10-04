Restatement

We have an R×C grid whose cells are either dry (#) or icy (.). A move in one of the four directions keeps sliding until it reaches the first dry cell in that direction, or, if there is none, the last cell on that edge of the board in that direction (a border cell). Only the cells where the slide starts and ends are considered “visited”; the intermediate cells passed through are not.

We may turn any icy cell into a dry cell. We want to change the fewest cells so that, no matter where we start, there exists a sequence of moves that visits every cell in the board. In graph terms: the directed graph induced by these “slide endpoints” must be strongly connected for the final board.

Key observations

1) Which cells can ever be visited without starting there?

- A slide ends only on:
  - the first dry cell encountered along the ray, or
  - the last cell along that direction on the board (a border cell in that direction).

An icy cell that is not on the border cannot be the end of any slide (it is neither dry nor an edge cell). So such a cell can be visited only if it is the starting position.

Because we must be able to visit all cells regardless of where we start, every non-border icy cell must be turned into dry. This gives a universal and unavoidable lower bound: at least all interior dots must be flipped.

2) After we make all interior cells dry, what remains to worry about?

- If both dimensions are at least 3 (R≥3 and C≥3), turning all interior dots to # makes the interior a solid rectangle of dry cells.
  - From any interior cell, moving N/S/E/W goes to the adjacent interior cell (because the neighbor is dry), so inside the interior we have ordinary 4-neighbor adjacency.
  - From any border cell, one move goes into the interior (to the adjacent inner cell), and from any interior cell we can step back to any border cell by moving outward (the outward slide stops exactly at that border cell).
  - This is enough to make the whole board strongly connected without touching any border dots.

- If one dimension is small (1 or 2), interior does not exist (or is too thin) and the above argument breaks. Then some extra flips on border cells are necessary. These cases reduce to simple per-line coverage conditions, described below.

Complete characterization by cases

A. Thick grids: R≥3 and C≥3
- Necessary: All strictly interior dots (2≤r≤R−1, 2≤c≤C−1) must be flipped.
- Sufficient: Flipping exactly those is enough to make the interior a connected block of dry cells. From any cell you can enter the interior, walk inside one cell at a time to the target’s row/column, and then slide outward to the exact border cell you want. Hence the whole board becomes strongly connected.
- Answer: number of interior dots.

B. One-dimensional grids: R=1 or C=1
Without interior, slides in the long direction end only at dry cells or at the two endpoints of the line. No move can ever stop at a middle icy cell unless it is dry.

- R=1:
  - Only columns 1 and C are unavoidable endpoints. Any middle column 2..C−1 can be visited from elsewhere only if it is dry.
  - Answer: number of dots among columns 2..C−1.

- C=1:
  - Symmetric. Answer: number of dots among rows 2..R−1.

C. Two-row or two-column grids: R=2 (C≥2) or C=2 (R≥2)
Think in columns (for R=2) or rows (for C=2).

- R=2:
  - Vertically, you can always move between the two rows in the same column.
  - Horizontally in a given row, you can stop only at columns that contain a # in that row, or at the two end columns (1 and C).
  - If in some interior column c (2..C−1) both cells (1,c) and (2,c) are dots, then no horizontal move will ever stop at column c in either row, and vertical moves do not change the column. Therefore, starting elsewhere, you can never visit column c. So at least one of those two cells must be turned to #.
  - Conversely, if every interior column has at least one # (in row 1 or row 2), then from any starting column you can, in the row that has a # at the target column, hop along “anchor” columns (the #’s plus the two ends) until you land on that column; then a single vertical move visits the other row’s cell. Repeating this covers all columns.
  - Answer: number of interior columns c in 2..C−1 where both (1,c) and (2,c) are dots.

- C=2:
  - Symmetric reasoning on rows.
  - Answer: number of interior rows r in 2..R−1 where both (r,1) and (r,2) are dots.

Why these answers are minimal

- Lower bounds:
  - Any interior dot (R,C≥3) is never a landing cell; if it remains dot, it cannot be visited from a different start.
  - In R=1 or C=1, any middle dot cannot be a landing cell unless turned dry.
  - In R=2, any interior column where both cells are dots cannot be landed on from another column; at least one must be flipped. Similarly for C=2 on rows.

- Upper bounds (sufficiency):
  - In R,C≥3, making all interior cells dry provides an interior “walkway” that enables reaching any border cell as well; since reaching is mutual for any two cells, the graph is strongly connected.
  - In R=1 or C=1, flipping all middle cells makes the line a chain of adjacent dry (or endpoint) landing cells; you can traverse it end to end.
  - In R=2 (resp. C=2), ensuring at least one dry in each interior column (resp. row) provides “anchors” that let you move column by column (resp. row by row), and vertical (resp. horizontal) moves visit the paired cell.

Thus the formulas above are both necessary and sufficient.

Algorithm

- Let grid be 1-indexed for exposition.

If R≥3 and C≥3:
- ans = count of cells (r,c) with 2≤r≤R−1 and 2≤c≤C−1 and grid[r][c]=='.'

Else if R==1:
- ans = count of c in 2..C−1 with grid[1][c]=='.'

Else if C==1:
- ans = count of r in 2..R−1 with grid[r][1]=='.'

Else if R==2:
- ans = count of c in 2..C−1 with grid[1][c]=='.' and grid[2][c]=='.'

Else (so C==2):
- ans = count of r in 2..R−1 with grid[r][1]=='.' and grid[r][2]=='.'

Output ans.

Pseudocode sketch

- read R, C and grid

- if R≥3 and C≥3:
    ans = 0
    for r = 2..R-1:
      for c = 2..C-1:
        if grid[r][c] == '.': ans++
    print ans
  else if R == 1:
    ans = 0
    for c = 2..C-1:
      if grid[1][c] == '.': ans++
    print ans
  else if C == 1:
    ans = 0
    for r = 2..R-1:
      if grid[r][1] == '.': ans++
    print ans
  else if R == 2:
    ans = 0
    for c = 2..C-1:
      if grid[1][c]=='.' and grid[2][c]=='.': ans++
    print ans
  else:  // C == 2
    ans = 0
    for r = 2..R-1:
      if grid[r][1]=='.' and grid[r][2]=='.': ans++
    print ans

Complexity

- Time: O(R·C)
- Memory: O(1) extra

Why the solution works for all cases

- The constraints R,C≤500 let us simply count as above.
- The “regardless of the starting position” requirement is equivalent to asking the slide-endpoint graph to be strongly connected. The case analysis shows precisely the minimal modifications that make every cell a possible landing cell from any other:
  - In thick grids, interior dry cells create a connected “walkable” core, and every border cell is reachable from that core and back.
  - In 1D and 2-layer grids, reachability hinges on having enough dry “anchors” to stop horizontally (or vertically). The per-column (or per-row) conditions are exactly the minimal anchors needed.

Sample checked

For the sample:
4 4
....
.###
##..
###.

Only one strictly interior dot exists: (3,3). Our rule for R,C≥3 returns 1, matching the sample.