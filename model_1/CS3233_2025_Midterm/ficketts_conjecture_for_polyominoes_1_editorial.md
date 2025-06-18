### Approach
The problem involves finding the maximum fraction of overlapping border cells between a convex polyomino and its rotated copy. The solution involves the following steps:

1. **Problem Analysis**: The problem requires placing a rotated copy (0°, 90°, 180°, or 270°) of the polyomino such that it overlaps with the original. The goal is to maximize the ratio of the number of border cells of the original polyomino that lie in the overlapping region to the number of border cells of the rotated copy that lie in the same overlapping region.

2. **Key Insights**:
   - **Convex Polyomino**: The polyomino is convex, meaning any horizontal or vertical line intersects it in a connected segment. This property allows efficient representation using minimal bounding boxes.
   - **Border Cells**: A cell is on the border if at least one of its 8 adjacent cells is not part of the polyomino.
   - **Rotations and Translations**: For each rotation (0°, 90°, 180°, 270°), the polyomino is transformed, and all possible translations (shifts) that result in overlap are considered.

3. **Algorithm Selection**:
   - **Minimal Bounding Box**: Extract the minimal bounding box of the polyomino to reduce the grid size.
   - **Border Identification**: For each cell, check its 8 neighbors to determine if it's a border cell.
   - **Rotation Handling**: For each rotation, transform the polyomino's cells and border cells, then adjust to a normalized coordinate system (minimal bounding box starting at (0, 0)).
   - **Overlap Calculation**: For each rotation and translation, count the overlapping border cells of the original and rotated polyomino. The translation is derived from the difference in coordinates of cells from the original and rotated polyomino.
   - **Fraction Maximization**: Track the maximum fraction encountered, comparing fractions as integers to avoid floating-point inaccuracies.

4. **Complexity Analysis**:
   - **Grid Processing**: Processing the grid to find the minimal bounding box and border cells is O(R*C) per test case.
   - **Rotation Handling**: Each rotation involves transforming and normalizing the polyomino's cells, which is O(area) per rotation, where area is the number of cells in the polyomino.
   - **Overlap Calculation**: For each rotation, the double loop over all cells of the original and rotated polyomino is O(area²) per rotation. Given 4 rotations, the total is O(4*area²) per test case.
   - **Overall Complexity**: The sum of R³ + C³ across test cases is bounded by 10^7, ensuring that the solution is efficient within constraints.

### Solution Code
```python
import math
from collections import defaultdict

def main():
    T = int(input().strip())
    for _ in range(T):
        data = input().split()
        R = int(data[0])
        C = int(data[1])
        grid = []
        for i in range(R):
            grid.append(input().strip())
        
        r0 = 0
        while r0 < R and '#' not in grid[r0]:
            r0 += 1
        if r0 == R:
            print("0/1")
            continue
        r1 = R - 1
        while r1 >= 0 and '#' not in grid[r1]:
            r1 -= 1
        cols_with_hash = []
        for j in range(C):
            for i in range(R):
                if grid[i][j] == '#':
                    cols_with_hash.append(j)
        if not cols_with_hash:
            print("0/1")
            continue
        c0 = min(cols_with_hash)
        c1 = max(cols_with_hash)
        
        H = r1 - r0 + 1
        W = c1 - c0 + 1
        grid0 = []
        for i in range(r0, r1 + 1):
            grid0.append(grid[i][c0:c1 + 1])
        
        S0 = set()
        border0 = set()
        directions = [(-1, -1), (-1, 0), (-1, 1),
                      (0, -1),           (0, 1),
                      (1, -1),  (1, 0),  (1, 1)]
        for i in range(H):
            for j in range(W):
                if grid0[i][j] == '#':
                    S0.add((i, j))
                    is_border = False
                    for dx, dy in directions:
                        ni, nj = i + dx, j + dy
                        if 0 <= ni < H and 0 <= nj < W:
                            if grid0[ni][nj] != '#':
                                is_border = True
                                break
                        else:
                            is_border = True
                            break
                    if is_border:
                        border0.add((i, j))
        
        rotations = [0, 90, 180, 270]
        best_num = 0
        best_den = 1
        for rot in rotations:
            if rot == 0:
                S_rot = S0
                border_rot = border0
                H_rot = H
                W_rot = W
            else:
                if rot == 90:
                    def f(i, j):
                        return (j, H - 1 - i)
                elif rot == 180:
                    def f(i, j):
                        return (H - 1 - i, W - 1 - j)
                elif rot == 270:
                    def f(i, j):
                        return (W - 1 - j, i)
                S_rot = set()
                border_rot = set()
                for (i, j) in S0:
                    x, y = f(i, j)
                    S_rot.add((x, y))
                for (i, j) in border0:
                    x, y = f(i, j)
                    border_rot.add((x, y))
                if not S_rot:
                    continue
                all_x = [p[0