Restatement

You are given an axis-aligned rectangle with opposite corners at (x1, y1) and (x2, y2). Inside the rectangle, there are N circular “burning areas” (open disks): the i-th has center (fx_i, fy_i) and radius r_i. A point is safe if it lies inside the rectangle (including its boundary) and is not strictly inside any disk (lying on a circle boundary is safe). The two towers are at the bottom-left and top-right corners respectively; both are guaranteed to be safe.

We must decide whether there exists a continuous safe path entirely inside the rectangle that connects the two towers.

Key ideas

1) Think in terms of barriers

- The unsafe region is the union of the interiors of the disks.
- A safe path exists if and only if the unsafe region does NOT separate the two corners inside the rectangle.
- How can separation happen? Only if the unsafe region (possibly using parts of the rectangle’s boundary where disks intersect it) forms a connected “wall” that blocks all such paths.

In a rectangle, there are only four ways such a wall can block the two opposite corners (bottom-left to top-right):

- A component of disks touches both the left and the right sides (a left–right wall), which separates the top from the bottom, so the two opposite corners are on different sides.
- A component touches both the bottom and the top sides (a bottom–top wall), which separates the left from the right.
- A component touches both the bottom and the left sides (this traps the bottom-left corner in the corner pocket).
- A component touches both the top and the right sides (this traps the top-right corner).

If none of the four happens, the two corners can still be connected by a safe path.

2) Model disks and sides as a graph

- Make a graph whose nodes are:
  - Each disk (N nodes).
  - The 4 rectangle sides: Left (L), Right (R), Bottom (B), Top (T).
- Put an edge between two disks if their interiors overlap (distance between centers < r_i + r_j). Tangency (distance = r_i + r_j) does not count, because touching at a single point still leaves a safe point to pass through.
- Put an edge between a disk and a side if the disk’s interior intersects that side segment (distance from center to the side segment < r_i). Again, strict inequality: tangency leaves a safe point on the side.

Then connected components in this graph correspond to connected chunks of the unsafe region (plus any boundary segments they touch).

3) When does the path not exist?

After building the adjacency (via Union-Find/DSU for components), the path is impossible if and only if any of these hold:

- L connected to R (left–right wall), or
- B connected to T (bottom–top wall), or
- L connected to B (bottom-left corner trapped), or
- R connected to T (top-right corner trapped).

Otherwise, answer YES.

Why strict inequalities?

- Points at distance exactly r from a disk center are safe (on the circle). If two disks are tangent or a disk is tangent to a side, there is still a safe point at the contact, and a safe path may pass through that point. Hence we must use “<” (not “<=”) for overlaps and for disk–side intersections.

Geometric checks (all with 64-bit integers; avoid sqrt)

- Disk–disk overlap: Let dx = fx_i − fx_j, dy = fy_i − fy_j, s = r_i + r_j.
  - Overlap if dx*dx + dy*dy < s*s.

- Disk–side intersection: Use the squared distance from point to segment. For axis-aligned sides, you can clamp to the segment in O(1).
  - Left side is the segment from (x1, y1) to (x1, y2):
    - y0 = clamp(fy_i, y1, y2).
    - dx = x1 − fx_i, dy = y0 − fy_i.
    - Intersects if dx*dx + dy*dy < r_i*r_i.
  - Right side (x2): same with x2.
  - Bottom side (y1), segment (x1, y1)–(x2, y1):
    - x0 = clamp(fx_i, x1, x2).
    - dx = x0 − fx_i, dy = y1 − fy_i.
    - Intersects if dx*dx + dy*dy < r_i*r_i.
  - Top side (y2): same with y2.

Here clamp(v, a, b) = min(max(v, a), b).

Pseudocode sketch

- Read x1, y1, x2, y2, N and the disks (fx[i], fy[i], r[i]).
- Create DSU with N + 4 nodes. Index sides as L, R, B, T.

- For each pair i < j:
  - if (dx^2 + dy^2 < (r[i] + r[j])^2) union(i, j).

- For each disk i:
  - if dist2_to_left_segment(i)   < r[i]^2 union(i, L).
  - if dist2_to_right_segment(i)  < r[i]^2 union(i, R).
  - if dist2_to_bottom_segment(i) < r[i]^2 union(i, B).
  - if dist2_to_top_segment(i)    < r[i]^2 union(i, T).

- If find(L) == find(R) or find(B) == find(T) or find(L) == find(B) or find(R) == find(T): print NO; else print YES.

Correctness argument

- If any of the four DSU connections holds:
  - L–R or B–T: the unsafe set forms a connected wall spanning opposite sides, thus separating the rectangle into two disjoint parts (by Jordan separation). The towers lie in different parts, so no safe path exists.
  - L–B: the unsafe set connects the two sides meeting at the bottom-left corner. Together with the domain boundary, this traps that corner’s neighborhood; you cannot leave it without crossing the unsafe set, so no safe path to the other corner. Similarly for R–T trapping the top-right corner.
- Conversely, assume none of the four holds but suppose no safe path existed. Then the unsafe set must separate the rectangle so that bottom-left and top-right lie in different components of the safe region. Any such separation in a rectangle must produce one of the four blockers above (either a wall between opposite sides or trapping one of the two corners). This contradicts the assumption. Therefore a safe path exists.

Complexity and limits

- DSU unions/finds are amortized near-constant.
- Pairwise disk checks: O(N^2) ≤ ~5e5 when N=1000.
- Disk–side checks: O(4N).
- All distance tests use only 64-bit integer arithmetic; use strict “<”.
- This fits comfortably in 0.5s and 256MB in a compiled language.

Common pitfalls

- Using <= instead of < will incorrectly treat tangencies as blocking, producing false NO.
- Measuring distance to infinite lines instead of the side segments can spuriously connect disks to sides outside the rectangle; always use distance to the segment.
- Beware integer overflow: use 64-bit for coordinates, deltas, and squared distances.

Why this handles all cases

- Disks completely outside the rectangle are ignored unless their interiors reach a side segment.
- Chains of disks can block only if their interiors actually overlap (strictly) to form a connected component; mere tangencies leave a safe point to pass.
- Huge disks covering multiple sides are correctly detected as connecting to several sides.
- Corner safety is respected: since towers are safe, any “blocking” via mere tangency at the corner does not create a DSU connection to both adjacent sides.

With these checks, the procedure exactly captures when the union of burning areas forms a topological barrier preventing any safe path between the two towers.