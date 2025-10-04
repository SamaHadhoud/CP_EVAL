Restatement

You have an undirected, positively weighted graph with cities 1…n and roads (edges). You must make two traversals:
- one from 1 to n, and
- one from n to 1.

Each traversal is any walk: vertices and edges can be revisited, and you’re even allowed to continue moving after first reaching the destination, as long as you eventually finish at the destination. The cost of a traversal is the sum of edge weights with multiplicity. The two traversals must be different in the sense that the set of distinct roads used in one is different from the set of distinct roads used in the other. The goal is to minimize the sum of the two traversal costs.

Key observations

1) Symmetry and optimal structure.
- Because the graph is undirected and weights are symmetric, the problem reduces to choosing two walks between 1 and n that have different sets of edges. (Travel back n→1 is the same as 1→n.)
- In an optimal pair, at least one traversal is a shortest 1→n path. Indeed, if neither is shortest, replacing the more expensive one by a shortest path strictly reduces the total while keeping the other traversal unchanged; the sets remain different unless the other also uses exactly the shortest-path edge set, which cannot happen in an optimal pair where both were non-shortest.

2) If there are at least two distinct shortest 1→n paths (i.e., different edge sets), the answer is simply twice the shortest distance d: use two different shortest paths.

3) If the shortest 1→n path is unique, the second traversal must use at least one edge not on that unique path. The cheapest such second traversal can be characterized very cleanly with standard shortest-path distances:
- Let dist_s[x] be the shortest distance from 1 to x.
- Let dist_t[x] be the shortest distance from x to n.
- Let d = dist_s[n].

An oriented edge u→v is called tight if dist_s[u] + w(u,v) + dist_t[v] = d. The set of all tight edges forms the directed acyclic graph (DAG) of all shortest 1→n paths: a directed 1→n path in this DAG is exactly a shortest path.

If there is only one shortest path, all tight edges lie on that single directed chain.

Now consider any edge e = {x,y} that is not on any shortest path (i.e., neither x→y nor y→x is tight). If you form the walk:
(shortest 1→x) + edge (x,y) + (shortest y→n),
its cost is dist_s[x] + w(x,y) + dist_t[y]. This walk necessarily uses an edge outside the unique shortest path, so the edge set differs. The best such choice is the minimum of this value taken over both orientations of every edge not on any shortest path.

Crucial lemma (why this is optimal): For any walk from 1 to n that uses at least one edge not on the unique shortest path, let e be the first such non-shortest-path edge you traverse when scanning from 1 to n. The part before e can be no shorter than dist_s[u], and the part after e can be no shorter than dist_t[v], so the total length is at least dist_s[u] + w(e) + dist_t[v] for the oriented edge u→v equal to e’s direction in the walk. Hence the minimum over all such walks equals the minimum of that expression over edges not on the shortest-path DAG.

Plan

Compute:
- d = shortest distance from 1 to n.
- Whether there are at least two distinct shortest 1→n paths; if yes, answer = 2d.
- Otherwise (unique shortest path), compute:
  M = min over all undirected edges e = {u,v} that are not on any shortest 1→n path of min(dist_s[u] + w_e + dist_t[v], dist_s[v] + w_e + dist_t[u]).
The answer is d + M, if M is finite; otherwise, -1.

Why this covers loops and “continuing after reaching the destination”
- Walks with loops can only increase cost. Allowing them is useful when there is no alternative simple path: you can still change the edge set by inserting a cheap back-and-forth on some extra edge. The M-formula naturally captures such “detours”: if an extra edge attaches like a leaf to the unique shortest path, the best constructed walk must traverse it forth and back, and the formula yields d + 2·w(edge). So we do not need to treat loops separately.

Algorithm details

1) Two Dijkstra runs
- Run Dijkstra from 1 to get dist_s[·].
- Run Dijkstra from n to get dist_t[·].
- If dist_s[n] = ∞, output -1 (cannot reach the contest).
- Let d = dist_s[n].

2) Build the “shortest-path graph” conceptually
- For each undirected edge {u,v} of weight w:
  - The directed edge u→v is tight if dist_s[u] + w + dist_t[v] = d.
  - The directed edge v→u is tight if dist_s[v] + w + dist_t[u] = d.
- Because w > 0, at most one orientation of an edge can be tight.

3) Count how many shortest 1→n paths exist in this DAG
- The DAG edges are exactly the tight directed edges (u→v).
- Topologically sort vertices by increasing dist_s (this is valid because all tight edges go from smaller to larger dist_s).
- Dynamic programming over the DAG:
  - ways_s[1] = 1; all others 0.
  - In increasing dist_s order, for each tight edge u→v:
    ways_s[v] = min(2, ways_s[v] + ways_s[u]).
- Then ways_s[n] is the number of shortest 1→n paths, capped at 2. If ways_s[n] ≥ 2, output 2*d and stop.

4) Unique shortest path case: compute M
- Mark an undirected edge {u,v} as “on some shortest path” if either u→v or v→u is tight. In the unique case, these are exactly the edges on the unique shortest path.
- Initialize M = ∞.
- For every undirected edge {u,v} of weight w that is not marked:
  - If dist_s[u] and dist_t[v] are both finite, M = min(M, dist_s[u] + w + dist_t[v]).
  - If dist_s[v] and dist_t[u] are both finite, M = min(M, dist_s[v] + w + dist_t[u]).
- If M = ∞, output -1 (the whole connected component is just a chain, so you cannot change the edge set at any cost).
- Otherwise output d + M.

Pseudocode sketch

- Read n, m and edges.
- Dijkstra(1) -> dist_s[], Dijkstra(n) -> dist_t[].
- If dist_s[n] = INF: print -1, return.
- d = dist_s[n].
- For each edge (u,v,w):
  - tight_uv = (dist_s[u] + w + dist_t[v] == d)
  - tight_vu = (dist_s[v] + w + dist_t[u] == d)
- Count shortest paths on the DAG of tight oriented edges:
  - Make array nodes = 1..n sorted by increasing dist_s.
  - ways_s[1] = 1; others 0.
  - For u in nodes:
      for each neighbor v with tight edge u→v:
          ways_s[v] = min(2, ways_s[v] + ways_s[u])
- If ways_s[n] ≥ 2: print 2*d, return.
- Else:
  - For each edge mark on_shortest_edge = tight_uv or tight_vu.
  - M = INF
  - For each edge (u,v,w):
      if not on_shortest_edge:
          if dist_s[u] < INF and dist_t[v] < INF:
              M = min(M, dist_s[u] + w + dist_t[v])
          if dist_s[v] < INF and dist_t[u] < INF:
              M = min(M, dist_s[v] + w + dist_t[u])
  - If M == INF: print -1
  - Else print d + M

Correctness argument

- Existence of an optimal pair with one shortest traversal: Let W1, W2 be any feasible pair with different edge sets. Replace whichever has larger cost by a shortest 1→n path S (cost d). If the other walk’s edge set equals S’s edge set, they would not have been different to begin with unless both were S; that contradicts the feasibility of the original pair. Thus there exists an optimal pair with one traversal of length d.

- If the DAG of tight edges contains ≥ 2 distinct 1→n directed paths, they correspond to at least two simple shortest paths that use different edge sets (in undirected graphs, a simple path’s set of edges determines the path). Using those two gives total cost 2d, which is optimal because every traversal must cost at least d.

- Unique shortest path case: Let E0 be the set of undirected edges on that unique path. Any second traversal with a different edge set must use at least one edge e ∉ E0. Let e first appear in the traversal oriented u→v. Let P be the prefix before e and Q the suffix after e. P has length ≥ dist_s[u], Q has length ≥ dist_t[v]. Hence total length ≥ dist_s[u] + w(e) + dist_t[v]. Minimizing over all such e ∉ E0 and both orientations gives exactly M. Conversely, for the edge and orientation attaining M, the walk (shortest 1→u) + e + (shortest v→n) uses e and has length M, so the total d + M is achievable. If no such edge exists (M = ∞), all edges in the component lie on the unique path, making it impossible to form a different edge set; answer is -1.

Complexity

- Two Dijkstra runs: O((n + m) log n).
- Building tight edges and counting shortest paths in the DAG: O(m) with one sort of vertices by dist_s: O(n log n) or you can reuse Dijkstra’s output order.
- Scanning all edges for M: O(m).

Overall time O((n + m) log n), memory O(n + m), well within 2 seconds and typical memory limits for n ≤ 1e5, m ≤ 3e5.

Notes and pitfalls

- Use 64-bit integers for distances and sums to be safe (distances up to about 3e8, sums up to about 6e8, but 64-bit is robust).
- When counting shortest paths, only propagate along tight edges u→v with dist_s[u] + w + dist_t[v] = d. This ensures you count only 1→n shortest paths, not all “shortest to v” prefixes that don’t reach n.
- You only need to know whether there are at least two shortest paths; cap counts at 2 to avoid overflow.
- Mark an undirected edge as “on some shortest path” if either orientation is tight; in the unique case this is exactly the edge set of the unique shortest path.

Why the samples match

- Sample 1: Unique shortest path 1–3 has length d = 5. The only non-shortest edge is 1–2 with cost 10. M = min(dist_s[1]+10+dist_t[2], dist_s[2]+10+dist_t[1]) = min(0+10+15, 10+10+5) = 25. Answer = 5 + 25 = 30.
- Sample 2: Unique path 1–2–3–4; there is no other edge, so M = ∞; answer -1.
- Sample 3: d = 5 via 1–2–4. Non-shortest edges are 1–3 and 3–4. M = min(0+3+4, 3+4+0, 3+3+5, 5+4+3) = 7. Answer = 5 + 7 = 12.

This approach is optimal, simple to implement, and robust for the given constraints.