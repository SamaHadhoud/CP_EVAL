Restatement

You are given N integer points in the plane. You want an axis-aligned square (sides parallel to the axes) such that every given point lies on the border of that square (edges or vertices). Degenerate squares (side length zero) are allowed. Output any such square as two opposite corners (X1, X2, Y1, Y2) meaning the square is [X1, X2] × [Y1, Y2] with X1 ≤ X2 and Y1 ≤ Y2 and X2 − X1 = Y2 − Y1. If no square exists, print “Impossible”.

Key observations

1) Any valid axis-aligned square must contain the entire set of points in both x and y:
- So necessarily X1 ≤ minX, X2 ≥ maxX where minX and maxX are the minimum and maximum x among points.
- Similarly Y1 ≤ minY, Y2 ≥ maxY.

Let dx = maxX − minX, dy = maxY − minY.
The square’s side length L must satisfy L ≥ dx and L ≥ dy, hence L ≥ max(dx, dy).

2) No point may lie strictly inside the open rectangle (minX, maxX) × (minY, maxY).
- If there exists a point with minX < x < maxX and minY < y < maxY, then no matter how you enlarge the square outward, that point stays strictly inside the square. Hence it is impossible.

3) When the bounding rectangle [minX, maxX] × [minY, maxY] is already a square (dx = dy), it is a valid answer if and only if no point is strictly inside that rectangle (Observation 2). In that case, all points are on the rectangle’s border, so output (minX, maxX, minY, maxY).

4) When the bounding rectangle is taller than it is wide (dy > dx), you must increase the x-range to length L = dy so that X2 − X1 = dy.
- But you cannot keep both vertical sides simultaneously at x = minX and x = maxX unless dx = dy. So at most one of those two vertical lines can remain the square’s vertical side.
- Any point that currently lies on a vertical side with y strictly between minY and maxY must remain on a vertical side after enlargement; otherwise it would become interior.
- Therefore, if there are points with x = minX and minY < y < maxY and also points with x = maxX and minY < y < maxY, it’s impossible when dy > dx.

Symmetrically, when the rectangle is wider than tall (dx > dy), at most one of the horizontal sides y = minY or y = maxY may host points with x strictly between minX and maxX.

These lead to a very simple constructive approach.

Constructive approach (try at most two candidates)

Let dx = maxX − minX, dy = maxY − minY, and L = max(dx, dy).

- First, compute minX, maxX, minY, maxY.

- If dx = dy:
  - Check that no point lies strictly inside (minX, maxX) × (minY, maxY).
  - If none do, output minX, maxX, minY, maxY.
  - Else Impossible.

- If dx < dy (taller than wide):
  - The y-range must remain [minY, maxY]. We must pick an x-interval of length L covering [minX, maxX]. There are only two “extreme” positions worth trying that can possibly keep points on the boundary:
    1) Keep the left side: X1 = minX, X2 = minX + L, Y1 = minY, Y2 = maxY.
    2) Keep the right side: X1 = maxX − L, X2 = maxX, Y1 = minY, Y2 = maxY.
  - Check candidate 1: verify every point lies on the border of this square.
  - If not, check candidate 2.
  - If neither works, Impossible.

- If dx > dy (wider than tall), symmetric:
  - Keep x-range [minX, maxX] and pick y-interval of length L:
    1) Keep the bottom: X1 = minX, X2 = maxX, Y1 = minY, Y2 = minY + L.
    2) Keep the top:    X1 = minX, X2 = maxX, Y1 = maxY − L, Y2 = maxY.
  - Check the two candidates; if neither works, Impossible.

Why only two candidates?
- For dx < dy, any x-interval of length L that covers [minX, maxX] can slide between [maxX − L, minX]. If any vertical side contains a “midpoint” (a point with y strictly between minY and maxY), that side must coincide with one of the square’s vertical sides. That forces the interval to be anchored at minX or at maxX. If both vertical extremes have midpoints, no placement is possible. Otherwise, one of the two anchors (left or right) suffices. Analogous reasoning holds when dx > dy.

Checking a candidate
Given a candidate (X1, X2, Y1, Y2) with X2 − X1 = Y2 − Y1, a point (x, y) lies on the square’s border if and only if:
- (x = X1 or x = X2) and Y1 ≤ y ≤ Y2, or
- (y = Y1 or y = Y2) and X1 ≤ x ≤ X2.

Reject the candidate if any point fails this test.

Edge cases and degeneracy
- N = 1 is always solvable with the zero-side square at that point.
- If all points share the same x (dx = 0) or the same y (dy = 0), the above candidate method handles it. For example, with dx = 0 < dy, the two candidates are X1 = minX, X2 = minX + L and X1 = maxX − L, X2 = maxX; one of them will keep the existing vertical line as a side.
- Coordinates and computed endpoints remain within the required output bounds. Inputs are within ±1e8, so L ≤ 2e8, and shifting by at most L still keeps X1, X2, Y1, Y2 within ±3e8, far below 1e9.

Pseudocode

- Read points into an array P of size N.
- Compute minX, maxX, minY, maxY.
- Let dx = maxX − minX, dy = maxY − minY, L = max(dx, dy).

Helper function onBorder(x, y, X1, X2, Y1, Y2):
  return ((x = X1 or x = X2) and Y1 ≤ y ≤ Y2) or
         ((y = Y1 or y = Y2) and X1 ≤ x ≤ X2)

Helper function validSquare(X1, X2, Y1, Y2):
  if X2 − X1 ≠ Y2 − Y1: return false
  for each (x, y) in P:
    if not onBorder(x, y, X1, X2, Y1, Y2): return false
  return true

Main:
- If dx = dy:
    if validSquare(minX, maxX, minY, maxY): print minX, maxX, minY, maxY
    else print Impossible
- Else if dx < dy:
    Candidate A: (minX, minX + L, minY, maxY)
    if validSquare(A): print it and stop
    Candidate B: (maxX − L, maxX, minY, maxY)
    if validSquare(B): print it and stop
    print Impossible
- Else (dx > dy):
    Candidate A: (minX, maxX, minY, minY + L)
    if validSquare(A): print it and stop
    Candidate B: (minX, maxX, maxY − L, maxY)
    if validSquare(B): print it and stop
    print Impossible

Why this is correct

- Necessity: Any valid square must enclose the x- and y-extremes, hence L ≥ max(dx, dy). Any point strictly inside the bounding rectangle remains strictly inside any outward expansion, so if such a point exists, a solution is impossible.
- Sufficiency: If no point is strictly inside the bounding rectangle, every point lies on its border. When the rectangle is already a square (dx = dy), that rectangle works. Otherwise, suppose dy > dx. Then the square’s y-range must be [≤minY, ≥maxY] with length L = dy. To achieve x-length L while covering [minX, maxX], set [X1, X2] to length L placed so as to keep any side containing “midpoints” (y strictly between minY and maxY) as a vertical side. If both vertical extremes have such midpoints, no placement works; otherwise, one of the two extreme placements (left-anchored or right-anchored) is valid, and it keeps every point on the border. The dx > dy case is symmetric.
- The explicit validation ensures all cases are handled, including degenerates.

Complexity

- Computing extremes: O(N).
- Trying up to two candidates and validating each in O(N).
- Total time O(N), memory O(N) if you store points (or O(1) extra memory beyond input if you stream and keep them).

This algorithm comfortably meets 1 second and 1024 MB limits for N up to 1e5.