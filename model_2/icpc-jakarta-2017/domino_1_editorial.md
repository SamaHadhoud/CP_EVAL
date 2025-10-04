Restatement

You are given N triples (ui, vi, wi) with all wi distinct. You must build a forest of rooted trees with exactly N edges, where:
- Every node carries a value A.
- Every edge carries a value B.
- Each triple must appear as an edge from a parent node p to a child node c such that p.A = ui, c.A = vi, and the edge’s B equals wi.
- A non-root, non-leaf node x must satisfy: the B of the edge from its parent is smaller than the B of every edge to its children.
- Every node has at most M children.
- The goal is to minimize the number of trees (roots).

Key observations

1) Nodes are identified by their A-values but are not unique by value. The forest may contain many distinct nodes having the same A. You decide when same A’s refer to the same node or to different nodes, as long as all constraints are satisfied.

2) The strict inequality on B around a non-root node implies a global time ordering. Since all wi are distinct, sort the edges by increasing w. If a node has an incoming edge with weight w_in, then all its outgoing edges must have weight > w_in. Therefore, when processing edges in increasing w:
   - The child node “is created” now (when its incoming edge arrives).
   - The parent node must already exist and be allowed to have children now.

3) Capacity model per node:
   - Each node can have at most M children. Think of each node as providing M “child slots” (capacity M) that can be used for outgoing edges from that node’s A-value in the future.
   - A node becomes available to parent future edges only after it is created:
     - Root node: available from the beginning (no incoming), capacity M.
     - Child node: available after its incoming edge is processed (because future edges must have larger weights).
   - Every edge (u → v, w) consumes one child slot of some node with A = u that is already available, and creates a new node with A = v that will later provide M child slots.

4) Minimizing the number of trees = minimizing the number of roots you create. A root is exactly a node with no incoming. You must create a root with A = u if, when you process an edge whose parent has A = u, there is no available child slot among nodes of A = u.

Greedy and the “token” view

Think in terms of tokens per A-value:
- Each node with value A contributes M tokens for A (its child slots).
- Each edge (u → v, w) needs to consume one A=u token from the pool available before time w.
- When that edge is processed, it also creates a new node with A=v, which adds M tokens for A=v, usable only for later times.

Thus:
- Sort edges by ascending w.
- Maintain for each A the number available[A] of tokens currently usable (i.e., from nodes created earlier, including roots).
- When processing (u, v, w):
  - If available[u] == 0, you must create a root with A = u: answer++, available[u] += M.
  - Consume one token for the current edge: available[u]--.
  - The child just created will be able to parent later edges: available[v] += M.
- Important order: consume the parent’s token before adding the child’s M. This enforces the strict w-increase at a non-root node (if u = v, the node created now cannot parent its own edge).

Pseudocode sketch

- Read N, M and the list of triples.
- Sort triples by w ascending.
- Use a hash map (or coordinate compression + array) to store available[A] as 64-bit integers.
- ans = 0
- For each edge (u, v) in sorted order:
  - if available[u] == 0:
      ans += 1
      available[u] += M
  - available[u] -= 1
  - available[v] += M
- Print ans

Why it is correct

A) Feasibility (constructing a valid forest)
- Each time you process (u → v, w), you attach it to some existing node with A = u that still has capacity (token available). If none exists, you create a root with A = u and use one of its M slots. This guarantees outdegree ≤ M for every node, by construction.
- The child v is a new node getting its incoming edge now. Later, it may be used (up to M times) as a parent for edges with larger w. This enforces the strict B-increase condition at non-root nodes automatically.
- Each node gets at most one incoming (either it’s a root or it is created as the child of exactly one edge).
- Edges always go from an older node to a newly created node, so cycles cannot form; you get a forest with exactly N edges.
- Therefore the construction always yields a valid forest.

B) Optimality (minimum number of trees)
- Fix a value A and consider the edges processed in increasing w. Let:
  - D_A(t) be the number of edges with parent A among the first t edges (by w).
  - S_A(t) be the number of edges with child A strictly before time t (i.e., among the first t−1 edges).
- By the rules, every parent use must come from an already existing node. Before time t, the only available nodes with A are:
  - all roots of A (say R_A many), and
  - the S_A(t) child-created nodes, each with capacity M.
- So for all t: D_A(t) ≤ M·(R_A + S_A(t)).
  Hence R_A ≥ ceil((D_A(t) − M·S_A(t)) / M) for every t, and thus
  R_A ≥ ceil(max_t (D_A(t) − M·S_A(t)) / M).
- The greedy updates available[A] = M·(roots created so far + child nodes already seen) − parent uses so far, and creates a root exactly when this would go negative. Therefore it creates the least number of roots necessary to keep this nonnegative across all prefixes, i.e., exactly ceil(max deficit / M) per A. Summing over all A gives the minimum possible number of trees.

Edge cases and implementation notes

- Values ui, vi can be up to 2e9, so use a hash map or coordinate compression for A-values.
- available[A] can become large: in the worst case, O(M · count_of_nodes_with_A). Use 64-bit integers.
- Time complexity: O(N log N) for sorting + O(N) for the sweep with hash map.
- Memory: O(number of distinct A) ≤ 2N, well within the 256 MB limit.

Walkthrough of Sample 1

Input
N=5, M=2
Edges sorted by w: (4→7,1), (4→8,2), (2→4,3), (7→2,4), (4→4,5)

Process:
- w=1, u=4: available[4]=0 → create root, ans=1, available[4]=2 → consume → 1; child v=7: available[7]+=2 → 2
- w=2, u=4: available[4]=1 → consume → 0; v=8: available[8]+=2 → 2
- w=3, u=2: available[2]=0 → create root, ans=2, available[2]=2 → consume → 1; v=4: available[4]+=2 → 2
- w=4, u=7: available[7]=2 → consume → 1; v=2: available[2]+=2 → 3
- w=5, u=4: available[4]=2 → consume → 1; v=4: available[4]+=2 → 3

Answer = 2, matching the sample.

Why the construction yields a forest

- Each edge connects an earlier-available node (root or node with earlier incoming) to a freshly created child node. No node ever receives two incoming edges. Outdegree per node is bounded by its M tokens. The strict increase on B at non-root nodes holds because every outgoing edge from such a node is processed later than its incoming edge. Therefore, the result is a valid forest, and our count is the minimum number of trees.

Summary

- Sort edges by B ascending.
- Sweep once, maintaining per-A “available parent slots” in a hash map.
- When an edge needs a parent slot for A = u and none is available, start a new root (ans++) and add M slots; then consume one. Always add M slots to A = v after processing the edge.
- The final ans is the minimum number of trees. This runs in O(N log N) time and O(N) memory and satisfies all constraints.