Restatement

We are given a tree of N warehouses (N−1 special rails). For each query we are given K robot locations (with possible repeats). All K robots must gather at a single warehouse (chosen optimally) and each robot travels along a unique path in the tree. The total number of “special rail usages” is exactly the sum of distances (in edges) from each robot to the chosen meeting node. For each query we must output the minimum possible total number of rail usages.

Key ideas

1) It’s a tree 1-median problem (with multiplicities)

- For a fixed meeting node x, the total rail usage is S(x) = sum over robots r of dist(r, x).
- On a tree, S(x) is minimized at any weighted median: a node x where, for every incident edge e, the number of robots lying in the component of e not containing x is ≤ K/2 (counting multiplicities when there are repeated robot locations). There can be several minimizers; the minimum value is unique.

2) Edge-cut formula for the minimum

Let c(e) be the number of robots on one side of edge e after removing e (choose either side; the other side has K − c(e)). Then the minimum possible sum of distances is

min_x S(x) = sum over edges e of min(c(e), K − c(e)).

Why this is true:

- For any fixed x, each edge e is crossed by exactly the robots that lie on the opposite side of e from x, contributing either c(e) or K − c(e) to S(x).
- If we could choose, edge-by-edge, which side contains x, we’d always choose the heavier side so that the number crossing e is min(c(e), K − c(e)).
- On a tree, there always exists a node (a weighted median) that lies on the “heavier side” of every edge simultaneously, so the above sum is attainable, and thus it is the minimum.

3) Only edges in the Steiner (virtual) subtree matter

If we remove an edge e that separates no robots (c(e) = 0 or K), its contribution min(c(e), K − c(e)) is 0. Nonzero contributions occur only on the minimal subtree that connects all robot locations (including repeated nodes treated as weight). We can work on a compressed “virtual tree” consisting of the K given nodes plus their pairwise LCAs; edges of the virtual tree correspond to paths in the original tree, and the count c(e) is constant along an entire compressed edge.

Algorithm

Preprocessing (once)

- Root the tree (say at 1).
- Compute:
  - tin[u], tout[u]: Euler tour entry/exit times for ancestor checks,
  - depth[u],
  - binary lifting table up[j][u] for LCA, for j up to ⌊log2 N⌋,
  - distance function dist(u, v) = depth[u] + depth[v] − 2·depth[lca(u, v)].

This costs O(N log N) time and O(N log N) space.

Per query

We are given K robot nodes A1..AK (with possible repetitions). Let w[u] be the multiplicity (how many robots are at u), and let Ktot be the total robots (sum of w[u], equals K).

Step 1: Prepare the set of terminals

- Let T be the set of distinct nodes u with w[u] > 0.

Step 2: Build the virtual tree of T

- Add all LCAs of consecutive nodes in T when sorted by tin: this guarantees all necessary branching points are present.
- Sort the union U = T ∪ LCAs by tin, remove duplicates.
- Build the virtual tree using a stack. Each time you move from an ancestor to a descendant in U-order, connect the current stack top (the nearest ancestor) to the new node. The length of every virtual edge (p → v) is dist(p, v) in the original tree.

Properties:
- The virtual tree is a rooted tree (root = first in U, which is an ancestor of all others).
- Its size |U| is O(K): in fact ≤ 2|T| − 1 ≤ 2K − 1.
- Each virtual edge corresponds to a disjoint path segment in the original tree; there are no terminals inside a virtual edge segment except at its ends.

Step 3: One DFS on the virtual tree to compute the answer

- For each node u in U, initialize sz[u] = w[u].
- Do a post-order traversal:
  - For each child v of u with virtual edge length L = dist(u, v):
    - Recursively compute sz[v].
    - Add to the answer: L · min(sz[v], Ktot − sz[v]).
    - Update sz[u] += sz[v].
- After finishing, the accumulated sum is exactly min_x S(x).

Use 64-bit integer for the answer.

Why the DFS formula is correct

Consider a virtual edge (u → v) corresponding to a path segment of length L in the original tree. Every original edge in this segment separates exactly the same subset of robots: those in the subtree rooted at v within the virtual tree. Hence, for every original edge in this segment, c(e) = sz[v]. The contribution of the entire segment is L · min(sz[v], K − sz[v]). Summing over all segments yields sum over all original edges of min(c(e), K − c(e)), which equals the minimum possible sum of distances.

Pseudocode (high level)

Preprocessing:

- DFS(root = 1):
  - set tin[u], depth[u], up[0][u]
  - fill up[j][u] for all j
  - set tout[u]

- lca(u, v): standard binary lifting.

- dist(u, v) = depth[u] + depth[v] − 2*depth[lca(u, v)].

Per query:

- Input nodes A[1..K].
- Compute multiplicities w[u] over unique nodes T.

Build virtual tree:

- U = T
- Sort U by tin
- For i = 1..|U|-1:
    U.add(lca(U[i−1], U[i]))
- Sort U by tin and unique
- Stack S = empty
- Clear adjacency of virtual tree for nodes in U
- For u in U in tin order:
    while S not empty and not is_ancestor(S.back(), u):
        S.pop_back()
    if S not empty:
        add_edge(S.back(), u, length = dist(S.back(), u))  // directed parent->child
    S.push_back(u)

Compute answer:

- ans = 0
- dfs(u):
    sz = w[u]
    for each (u -> v) with length L:
        child_sz = dfs(v)
        ans += L * min(child_sz, Ktot − child_sz)
        sz += child_sz
    return sz
- Run dfs(root of virtual tree), print ans

Complexity analysis

- Preprocessing: O(N log N) time and O(N log N) memory.
- For each query:
  - Sorting and building U: O(K log K + K log N) due to LCA calls and sorting.
  - The virtual tree has O(K) nodes and edges, DFS is O(K).
- Total per query time O(K log N) with a small constant, which is fast enough for Q ≤ 5000 and K ≤ 50.

Correctness argument (summary)

- For any meeting point x, S(x) = sum over edges e of the number of robots whose path to x crosses e.
- The minimum over x equals sum_e min(c(e), K − c(e)) because a weighted median exists that lies on the “heavier side” of every edge.
- Only edges in the minimal subtree connecting the robots matter; compressing this subtree into the virtual tree preserves contributions by grouping runs of identical c(e) across path segments.
- The DFS computes exactly this sum by accumulating L · min(sz[v], K − sz[v]) for each virtual edge (u → v).

Implementation tips

- Treat repeated robot locations by counting multiplicities w[u].
- Use long long (64-bit) for the answer.
- To avoid O(N) clearing per query, keep a list of nodes used in U and only clear adjacency/weights for those nodes after the query.

This approach runs comfortably within the given limits and handles all edge cases (single robot, all robots at the same node, widely separated robots, etc.).