Restating the problem

- You are given a connected simple graph with n vertices and m edges (m ≥ n − 1).
- There are m available colors, 1..m. Each color c can be used on at most a_c edges.
- You may color any subset of edges; uncolored edges are allowed.
- For each color c, all edges colored c must “stick together”: the subgraph induced by those edges (and the vertices touching them) must be connected.
- Task: decide if you can color at least ceil(n/2) edges under these constraints; if yes, output one valid coloring.

Key observations

1) A necessary condition is trivial:
- The total number of colored edges is at most sum_c a_c.
- Therefore, if sum_c a_c < ceil(n/2), the answer is No.

2) A spanning tree suffices:
- The graph is connected, so it has a spanning tree with n − 1 edges.
- Since ceil(n/2) ≤ n − 1 for n ≥ 2, if we can color up to min(sum a, n − 1) edges in a spanning tree, that will already meet the threshold whenever sum a ≥ ceil(n/2).
- We can leave all non-tree edges uncolored. They impose no constraints.

3) The only real difficulty is respecting the “each color induces a connected subgraph” constraint while using as many edges as capacities allow. We will show how to partition the edges of a spanning tree into connected groups of prescribed sizes that sum to any S ≤ n − 1.

Core lemma (tree postorder suffixes are connected)

Root the spanning tree at any vertex r. For each non-root vertex v, let e(v) be its parent edge (between v and parent(v)). Do a DFS from r. When you finish exploring a child subtree, append its parent edge to a list L (postorder on edges). Thus L is a permutation of the n − 1 tree edges.

Lemma: For any t (1 ≤ t ≤ n − 1), the set of edges {L[t], L[t+1], …, L[n−1]} induces a connected subgraph.

Proof sketch:
- Proceed by downward induction on t.
- Base t = n − 1: the set contains one edge, connected.
- Inductive step: let e = L[t] connect node u to its parent p.
  In the DFS, the parent-edge of p (if it exists) appears after e in L, hence lies in {L[t+1], …}. Therefore vertex p already belongs to the vertex set of the current suffix. Adding e joins u to that connected set at p, preserving connectivity.

Consequence: If we take a suffix of L, we get a connected set of edges. If we then remove that suffix and repeat on the remaining prefix, the “new suffix” of what remains is again connected (it’s a suffix of the postorder of the remaining forest), and so on. Hence we can cut L from the end into blocks; each block induces a connected subgraph.

From capacities to connected blocks

Let S = min(sum_c a_c, n − 1).
- If S < ceil(n/2), answer is No.
- Otherwise, we can color exactly S edges in the spanning tree.

Choose any order of colors (1..m suffices). Consume capacity greedily: for each color c, let s_c = min(a_c, remaining S). This yields a multiset of nonnegative integers s_c with sum S.

Now partition the postorder list L from the back:
- Color the last s_c edges of L with color c, then shrink L by removing that suffix.
- By the lemma, those s_c edges form a connected subgraph for color c.
- Continue until S = 0.

This yields:
- Exactly S colored edges (S ≥ ceil(n/2)).
- Each color c uses at most a_c edges.
- Every color’s edges are connected (each is a suffix block at its moment of assignment).
- All used edges are disjoint, as we partitioned the list.

Hence sum a ≥ ceil(n/2) is not only necessary but also sufficient.

Algorithm outline

For each test case:
1) Read n, m, array a[1..m], and the m edges (u_i, v_i) with original indices i.

2) Build a spanning tree T and a postorder list L of its edges:
- Run a DFS (iterative to avoid recursion limits) from any root (say 1).
- When finishing vertex v (returning to its parent), append the edge e(v) to L.
- L has exactly n − 1 tree edges, in postorder.

3) Compute S = min(sum a, n − 1). If S < ceil(n/2), print No.

4) Otherwise, print Yes and construct an assignment ans[1..m] (initialized to 0):
- Maintain a pointer p at the end of L.
- For c = 1..m while S > 0:
  - t = min(a[c], S)
  - Assign ans[L[p]] = c for t edges, decreasing p each time.
  - Reduce S by t.
- Non-tree edges (and any leftover tree edges) remain 0 (uncolored).

5) Output the ans array.

Pseudocode (high level)

- build adjacency list with (neighbor, edge_id)
- DFS(root=1):
    visited[root] = true
    For each tree-edge discovered (u -> v) with edge id eid:
        DFS(v)
        append eid to L   // after finishing v
- S = min(sum(a), n-1)
- if S < (n+1)/2: print "No"; continue
- else:
    print "Yes"
    ans[1..m] = 0
    p = |L| - 1
    for c in 1..m:
        if S == 0: break
        t = min(a[c], S)
        repeat t times:
            ans[L[p]] = c
            p -= 1
        S -= t
    print ans[1], ans[2], ..., ans[m]

Why it fits the limits

- Building the spanning tree + postorder is O(n + m).
- Coloring uses at most n − 1 assignments.
- Across all test cases, sum of n + m ≤ 3e5, so total time is linear; memory is O(n + m).

Why the solution is correct

- Necessity: total colored edges ≤ sum a; hence if sum a < ceil(n/2) it’s impossible.
- Sufficiency:
  - We color only tree edges, at most n − 1 of them; since ceil(n/2) ≤ n − 1, that’s enough if sum a ≥ ceil(n/2).
  - Partitioning the postorder list from the end into capacity-sized blocks guarantees each block (i.e., the edges colored with one color) is connected by the lemma.
  - Capacities are respected by construction; edges get at most one color.
- Uncolored edges are irrelevant per statement.

Handling all cases

- If some a_c are zero, they are just skipped; if one a_c is huge, it may get all edges in one block (still connected).
- If sum a ≥ n − 1, we can color the whole spanning tree; if sum a is smaller but ≥ ceil(n/2), we color exactly sum a edges.
- Non-tree edges remain 0 and do not affect validity.

This reduces a seemingly hard global connectivity constraint to a simple linear-time check and a deterministic constructive coloring tied to a DFS postorder partition.