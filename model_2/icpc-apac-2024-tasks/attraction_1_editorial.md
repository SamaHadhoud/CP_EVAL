Restatement

You are given a straight-line planar graph G on n points in the plane (the cities) with m non-crossing edges (the highways). Each edge e has a nonnegative weight a_e (the number of attraction points), bounded by 10^6. For any non-empty subset S of vertices, define:

- W(S) = sum of a_e over edges e with both endpoints in S
- f(S) = number of unordered pairs in S that are not connected by an edge

The score is Score(S) = W(S) − 10^6 · f(S)^2. You must find the maximum Score(S) over all non-empty S.

Key observations and planarity

- The input graph is planar; coordinates are only there to certify a straight-line, non-crossing embedding. We do not need to use geometry explicitly.
- All edge weights are in [0, 10^6] and the penalty coefficient is exactly 10^6. This makes missing pairs extremely expensive.

Notation: Let k = |S|, t = number of edges induced by S (so t ≤ C(k, 2)), and recall W(S) ≤ 10^6 · t because a_e ≤ 10^6 for every edge.

Crucial upper bounds: why only tiny S matter

Using planarity and the weight bounds, we can restrict the search to very small sets.

- Planarity gives t ≤ 3k − 6 for k ≥ 3.

Thus Score(S) ≤ 10^6 · t − 10^6 · (C(k, 2) − t)^2. Plugging t ≤ 3k − 6:

- k = 1: Score = 0
- k = 2: Score ≤ 10^6
- k = 3: best is a triangle (t = 3) with Score ≤ 3 · 10^6
- k = 4: t ≤ 6. For t = 6 (K4), Score ≤ 6 · 10^6; for t = 5, Score ≤ 4 · 10^6; for t ≤ 4, Score ≤ 0
- k = 5: t ≤ 9. For t = 9, Score ≤ 8 · 10^6; for t = 8, Score ≤ 4 · 10^6; for t ≤ 7, Score ≤ 0
- k = 6: t ≤ 12 and C(6, 2) = 15, so Score ≤ 10^6 · (12 − 3^2) = 3 · 10^6
- k ≥ 7: Score < 0

From this:

1) The optimum has |S| ≤ 6.

2) k = 6 cannot be optimal. If S is best with k = 6, it must have t = 12 (otherwise Score ≤ 0). But any maximal planar graph on ≥ 4 vertices contains a K4. Let T be a K4 ⊆ S. Then

Score(S) ≤ W(T) + 6 · 10^6 − 9 · 10^6 = W(T) − 3 · 10^6 < W(T) = Score(T)

since W(T) ≥ 0. So any k = 6 candidate is dominated by some K4.

3) For k = 5, it suffices to check t = 9 only.

- For t ≤ 7: Score ≤ 0, never optimal.
- For t = 8: among the 5 choose 4 subsets, the average number of edges is (3t)/5 = 24/5 > 4. So there exists a 4-subset with at least 5 edges (a K4 or a “diamond” = two triangles sharing an edge). Let that 4-subset T have t_T edges; then Score(S) ≤ W(T) + (8 − t_T) · 10^6 − 4 · 10^6 ≤ Score(T). So any 5-vertex 8-edge set is dominated by some 4-vertex set with 5 or 6 edges.

4) For k = 4, only t = 5 or 6 matter (t ≤ 4 yields Score ≤ 0).

5) For k = 3, only triangles (t = 3) matter; for t = 2 (a path), Score ≤ 10^6, which never beats taking just the heavier of its two edges.

Bottom line: it is enough to check only the following shapes
- Single edge {u, v} (k = 2; Score = a_uv)
- Triangle (k = 3; Score = sum of its three edges)
- K4 (k = 4; Score = sum of its six edges)
- 4-vertex “diamond” (k = 4, exactly 5 edges; two triangles sharing an edge; Score = sum of its five edges − 10^6)
- 5-vertex K5 minus one edge (k = 5, exactly 9 edges; equivalently a K4 plus a fifth vertex adjacent to exactly the three vertices of some K4 face; Score = sum of those 9 edges − 10^6)

Any other subset S is never better than one of these.

How to enumerate all candidates in O(n + m)

We use the fact that planar graphs have degeneracy ≤ 5. Compute a degeneracy ordering of vertices (u1, u2, …, un) by repeatedly removing a vertex of minimum degree; in the final order, every vertex has at most 5 neighbors later in the order. Direct every edge from the earlier vertex to the later vertex. Then each vertex u has out-degree ≤ 5.

Data we keep:
- For every vertex u, the list out[u] of its out-neighbors (size ≤ 5).
- A way to test adjacency and retrieve weights: for each u, store neighbors and edge weights in a dictionary (e.g., hash map or sorted vector + binary search). Also keep a global map for undirected (min(u, v), max(u, v)) → weight and edge-id.
- While enumerating triangles, for each undirected edge e = {v, w} we will maintain the two largest values of legSum = a(u, v) + a(u, w) among all triangles (u, v, w). That lets us get the best diamond sharing edge {v, w} without enumerating O(k^2) pairs.

We process every vertex u once. Let L = out[u].

1) All edges: While reading input, track max_edge = max_e a_e.

2) All triangles (u, v, w) with u the smallest in the degeneracy order:
- For every unordered pair {v, w} ⊆ L, if (v, w) is an edge:
  - triSum = a(u, v) + a(u, w) + a(v, w). Update answer = max(answer, triSum).
  - For the edge e = {v, w}, update its top two legSums with value a(u, v) + a(u, w). (Keep the best and second-best over all u participating with this base edge.)
  - For K5 − e (five-vertex, 9 edges): build the small list Cand = {x ∈ L \ {v, w} | edges (x, v) and (x, w) exist}. Because |L| ≤ 5, this is constant-time work per triangle. For every unordered pair {x, y} ⊆ Cand:
    - score_5 = triSum
                + [a(u, x) + a(v, x) + a(w, x)]
                + [a(u, y) + a(v, y) + a(w, y)]
                − 10^6
      Update answer.
    Explanation: In any five-vertex K5 − e, the smallest vertex u (in degeneracy order) is in the base triangle and both “apex” vertices are > u, hence both lie in out[u]. This enumeration counts each such 5-set exactly once.
- Note for k = 5 we never need to consider t = 8, as argued.

3) All K4 (four-vertex cliques) with u the smallest in the degeneracy order:
- For every unordered triple {v, w, x} ⊆ L, check whether all three edges (v, w), (v, x), (w, x) exist.
- If yes, we have a K4 = {u, v, w, x}. Compute
  s4 = a(u, v) + a(u, w) + a(u, x) + a(v, w) + a(v, x) + a(w, x),
  and update answer.

Because |L| ≤ 5, the number of pairs per u is ≤ C(5, 2) = 10 and the number of triples is ≤ C(5, 3) = 10. Every adjacency check is O(1) or O(log deg) depending on your dictionary. Hence this entire part runs in O(n + m).

4) All “diamonds” (two triangles sharing an edge; 4 vertices, 5 edges):
- After step 2, for every edge e = {v, w} that has at least two distinct triangles (u, v, w), we have recorded the two largest legSum values leg1 and leg2 over all third vertices u that complete (v, w) to a triangle.
- The best diamond using base edge {v, w} takes those top two third vertices u1, u2 (if they are the same vertex, skip; otherwise):
  score_diamond = a(v, w) + leg1 + leg2 − 10^6
  Update answer.
- This computes the maximum over all pairs of triangles around {v, w} without enumerating O(k^2) pairs even if there are many such triangles (we only need the top 2).

5) Base cases:
- Keep answer ≥ 0 (the empty penalty-free singleton).
- Also compare with max_edge (k = 2).

Why this finds the true maximum

- We proved any optimal S has |S| ≤ 5 and f(S) ≤ 1, i.e., it is one of:
  - Single edge, triangle, K4,
  - A diamond (k = 4, 5 edges, f = 1), or
  - A five-vertex K5 minus one edge (k = 5, 9 edges, f = 1).
- Every triangle is enumerated exactly once (at its smallest vertex in the degeneracy order).
- Every K4 is enumerated exactly once (at its smallest vertex u, using a triple from out[u]).
- Every diamond is realized as the union of two triangles sharing the base edge. By keeping the top two legSums per edge we compute, for each base edge, the best possible diamond score; we don’t need to examine all pairs explicitly.
- Every five-vertex K5 − e is exactly a K4 plus a fifth vertex adjacent to the three vertices of one K4 face. In degeneracy order, the smallest vertex u of the 5-set lies in that face. Both “apex” vertices are in out[u] and we detect them as the pair {x, y} inside Cand when we process the base triangle (u, v, w). This counts each such 5-set once. No other 5-vertex type can beat these, as shown above.
- We also argued that 5-vertex 8-edge sets are dominated by some 4-vertex candidate, so we don’t need them.

Pseudocode sketch

- Precompute degeneracy order ord[u]; orient each edge u−v as u→v if ord[u] < ord[v]; build out[u].
- Build adjacency/weight lookup w(u, v); also map undirected pair (min(u, v), max(u, v)) to edge-id and weight.

Initialize ans = max(0, max_edge_weight)

For each edge e, init best1[e] = −∞, best2[e] = −∞

For u in vertices in degeneracy order:
  L = out[u]   // |L| ≤ 5

  // Triangles and K5−e
  For each unordered pair {v, w} ⊆ L:
    if edge (v, w) exists:
      tri = w(u, v) + w(u, w) + w(v, w)
      ans = max(ans, tri)

      // Update diamond base statistics for edge {v,w}
      leg = w(u, v) + w(u, w)
      e = id_of_edge(min(v, w), max(v, w))
      update (best1[e], best2[e]) with leg

      // K5−e anchored at this triangle
      Cand = []
      For x in L, x ≠ v, x ≠ w:
        if edges (x, v) and (x, w) exist:
          Cand.push(x)
      For each unordered pair {x, y} ⊆ Cand:
        score5 = tri
               + (w(u, x) + w(v, x) + w(w, x))
               + (w(u, y) + w(v, y) + w(w, y))
               − 10^6
        ans = max(ans, score5)

  // K4 with u as the smallest
  For each unordered triple {v, w, x} ⊆ L:
    if edges (v, w), (v, x), (w, x) all exist:
      s4 = w(u, v) + w(u, w) + w(u, x) + w(v, w) + w(v, x) + w(w, x)
      ans = max(ans, s4)

// Diamonds from best two triangles per edge
For each edge e = {v, w}:
  if best2[e] is defined:
    score_d = w(v, w) + best1[e] + best2[e] − 10^6
    ans = max(ans, score_d)

Output ans.

Complexity

- Computing a degeneracy ordering is O(n + m).
- Each vertex has out-degree ≤ 5, so per-vertex pairs/triples are O(1) each. We do O(n) adjacency checks and O(n) constant-time updates.
- The final diamond pass is O(m).
- Overall time O(n + m); memory O(n + m).

Why constants and planarity matter

- The degeneracy ≤ 5 crucially comes from planarity. It lets us enumerate all triangles, K4’s, and K5 − e’s in linear time by looking only at the tiny out-neighborhoods.
- The penalty coefficient 10^6 equals the maximum edge weight, letting us bound away larger sets and reduce to the few small patterns above.

Edge cases

- If m = 0, the answer is 0 (choose any single city).
- If the graph has no triangles or K4, the algorithm still returns the best single edge (or 0).
- Zero-weight edges are harmless; all sums are taken in 64-bit integer.

Correctness summary

- We proved optimal S has |S| ≤ 5 and f(S) ≤ 1. The only candidates are edges, triangles, K4, diamonds, and K5 − e; anything else is dominated.
- The enumeration above visits every instance of these patterns exactly once (or computes their maxima via “best two” per-base-edge).
- Therefore the maximum over all generated candidates equals the true maximum attraction score.

This fits comfortably within the time and memory limits for n ≤ 100,000 and m ≤ 300,000.