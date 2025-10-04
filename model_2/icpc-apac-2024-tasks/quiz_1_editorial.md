Restatement

You have a rooted tree with nodes 1..n. Let L be the multiset of n^2 numbers
V(x, y) = (x-1)·n^2 + (LCA(x, y) - 1)·n + (y-1)
for all ordered pairs (x, y). After sorting L in non-decreasing order, for each query k you must output the k-th element of L (1-indexed). You must do this for up to 100000 queries with n up to 100000.

Key observation: sorting those numbers is equivalent to lexicographically sorting the triples (x, l, y), where l = LCA(x, y), by x increasing, then l increasing, then y increasing. This allows us to avoid constructing L explicitly and answer queries directly.

High-level plan

- The sorted array L is exactly all triples (x, l, y) such that l = LCA(x, y), listed in lexicographic order by (x, l, y). So L consists of n blocks of size n, one block for each x (in increasing x), within each block grouped by l (in increasing l), and within each l-group, y is listed in increasing numeric order.
- Given k, determine x and the in-block rank t:
  x = ((k-1) div n) + 1,  t = ((k-1) mod n) + 1.
  Then the task reduces to: in the block for this x, find the t-th pair (l, y) in the lexicographic order over l ascending and y ascending among pairs satisfying l = LCA(x, y).
- For fixed x, the y with LCA(x, y) = l are precisely the nodes in the subtree of l, excluding the subtree of the child of l that lies on the path from l to x (call that child a1). For l = x, a1 does not exist, and the set is just the whole subtree of x.

This reduces each query to two subproblems:
1) Find the smallest label l such that the total number of pairs with l’ ≤ l (where l’ is an ancestor of x) reaches at least t.
2) Within that l-group, select the b-th y (b = t minus all counts from previous l’), where y ranges over the set subtree(l) minus subtree(a1), ordered by numeric label.

We will solve (1) by parallel binary search (PBS) coupled with a sweep over node labels using a Fenwick tree (Binary Indexed Tree) with range updates and point queries. We will solve (2) by a persistent segment tree to support k-th smallest label in “subtree(l) minus subtree(a1)”.

Preprocessing

Compute once:
- Root, adjacency, parent p[u], depth[u], subtree size sz[u], Euler tour tin[u], tout[u] (1..n) by a DFS.
- Binary lifting table up[u][i] to jump u upward by 2^i (used only to get the child a1 of l on the path to x).
- A persistent segment tree (Chairman Tree) over the label domain [1..n] built along the Euler tour: let euler[1..n] be nodes in DFS order; build version root[i] = root[i-1] with +1 at position label = euler[i]. Then, for any subtree S = [L, R] in Euler order, the frequency of labels in S is represented by the difference of versions (root[R], root[L-1]). You can query counts in any label interval and also select the k-th smallest label in S. We will need the k-th over the set difference of two subtrees, which is handled by subtracting two such differences.

Characterizing counts for fixed x

- For a fixed x and an ancestor l of x:
  count(x, l) = number of y with LCA(x, y) = l =
    sz[l]                  if l = x
    sz[l] - sz[a1]         otherwise (a1: child of l on the path from l to x)
- Let P_x(z) be the number of pairs (l’, y) counted in the block of x among all l’ ≤ z. Then
  P_x(z) = sum over ancestors l’ of x with l’ ≤ z of count(x, l’).
  This is a non-decreasing function of z and P_x(n) = n.

Computing P_x(z) offline via a sweep

We create two types of “events” keyed by the parent label l:
- Type A (node event): for node l, add +sz[l] to every x in subtree(l).
- Type B (edge event): for each edge l -> u (u child of l), add -sz[u] to every x in subtree(u).

If we process all events with key ≤ z, then for any x:
P_x(z) = (sum of A over all ancestors l’≤z) + (sum of B over all edges (l’→u) with l’≤z and x ∈ subtree(u)) = sum ancestors sz[l’] − sum over the a1’s above x sz[a1] = sum count(x, l’).

We can maintain these sums with a single Fenwick tree over Euler indices supporting range add and point query:
- Range add [L, R] by +w is implemented as add(L, +w), add(R+1, -w); point query at tin[x] returns the current sum for x.
- When we insert a Type A for l, we do range add [tin[l], tout[l]] by +sz[l].
- For each Type B on edge l→u, do range add [tin[u], tout[u]] by -sz[u].
- After processing all keys up to z, fenwick.query(tin[x]) equals P_x(z).

Step 1: Find l for each query (PBS)

For each query k:
- Compute x and t as above.
- We need the smallest l ∈ [1..n] with P_x(l) ≥ t.

We run parallel binary search on l over all queries:
- Initialize low_j = 1, high_j = n for each query j.
- Repeat until all low_j = high_j:
  - For every unresolved query j, compute mid_j = floor((low_j + high_j)/2) and bucket it under mid_j.
  - Reset Fenwick to zero.
  - Sweep labels l from 1 to n:
    - Apply Type A for l.
    - Apply all Type B for edges out of l.
    - For each query j in bucket[l], compute val = fenwick.query(tin[x_j]) = P_{x_j}(l).
      If val ≥ t_j: set high_j = l; else low_j = l + 1.
At the end l_j := low_j is the correct l for each query.

Step 2: Get the offset b = t − P_x(l−1) in one sweep

We also need b_j = t_j − P_{x_j}(l_j − 1). Compute all b_j at once:
- Group queries by their final l_j.
- Reset Fenwick to zero.
- Sweep l from 1 to n:
  - Before inserting events for l, Fenwick currently encodes contributions from labels < l.
  - For every query j with l_j = l:
    pref = fenwick.query(tin[x_j]) = P_{x_j}(l − 1).
    b_j = t_j − pref.
  - Apply Type A for l and all Type B for edges out of l.
This is a single O(n log n + q log n) pass.

Step 3: Select y inside the l-group

Given x, l = l_j, and b = b_j:
- If l = x, the allowed set of y is exactly subtree(x).
- If l ≠ x, let a1 be the child of l that lies on the path from l to x. Compute it by binary lifting:
  steps = depth[x] − depth[l] − 1
  a1 = jump_up(x, steps)
- The allowed y-set is S = subtree(l) minus subtree(a1). We need the b-th smallest label in S.

Use the persistent segment tree (Chairman Tree) built on labels:
- For any subtree of a node v, the multiset of labels is represented by (root[tout[v]], root[tin[v]−1]).
- For S = subtree(l) − subtree(a1), the frequency on any label interval is
  F_S = (root[tout[l]] − root[tin[l]−1]) − (root[tout[a1]] − root[tin[a1]−1]).
- To select the b-th smallest label, walk down the segment tree from [1..n], at each node computing leftCount = count in the left child of F_S; if b ≤ leftCount, go left; else b -= leftCount, go right. At a leaf, return its label y.

Finally, the answer number is
ans = (x − 1)·n^2 + (l − 1)·n + (y − 1).

Pseudocode outline (sketch; omit implementation details)

Preprocessing:
- DFS(root):
  - compute parent, depth, sz[u], tin[u], tout[u], euler[].
- Build binary lifting up[u][i].
- Build Chairman Tree versions:
  - root[0] = empty.
  - for i = 1..n: v = euler[i]; root[i] = insert(root[i−1], position = v).

Events:
- For each node l: A-event at key l: (interval [tin[l], tout[l]], +sz[l]).
- For each edge l→u: B-event at key l: (interval [tin[u], tout[u]], −sz[u]).

PBS to find l:
- For each query j:
  - read k; x = (k−1) // n + 1; t = (k−1) % n + 1; low = 1; high = n.
- While exists j with low < high:
  - clear buckets.
  - for all j with low<high: m = (low+high)//2; bucket[m].push(j).
  - Fenwick.clear()
  - for l = 1..n:
    - fenwick.add_range(tin[l], tout[l], +sz[l]).
    - for u child of l: fenwick.add_range(tin[u], tout[u], −sz[u]).
    - for j in bucket[l]:
      - val = fenwick.query(tin[x_j]).
      - if val ≥ t_j: high_j = l else low_j = l+1.
- l_j = low_j.

One pass to get P_x(l−1):
- Group queries by l_j.
- Fenwick.clear()
- for l = 1..n:
  - for j in group[l]:
    - pref = fenwick.query(tin[x_j]).
    - b_j = t_j − pref.
  - fenwick.add_range(tin[l], tout[l], +sz[l]).
  - for u child of l: fenwick.add_range(tin[u], tout[u], −sz[u]).

Select y and output:
- for each query j:
  - if l_j = x_j: a = null else a = jump_up(x_j, depth[x_j] − depth[l_j] − 1).
  - y_j = kth_label(
      root[tout[l_j]], root[tin[l_j]−1],
      a ? root[tout[a]] : null, a ? root[tin[a]−1] : null,
      b_j)
  - ans = (x_j − 1)·n^2 + (l_j − 1)·n + (y_j − 1).
  - print ans.

Why this works

- The numeric key V(x, y) makes the final sort lexicographic by (x, LCA(x, y), y). This partitions the n^2 items into n blocks by x, and within each block groups by l ascending, then y ascending.
- For fixed x and any ancestor l of x, the set of y with LCA(x, y) = l is exactly subtree(l) minus subtree(a1), where a1 is the child of l on the path to x. The size of this set is sz[l] if l = x, else sz[l] − sz[a1].
- The function P_x(z), the cumulative count of items with l ≤ z, can be written as a sum over ancestors minus a sum over the a1-subtrees along the path to x. Our event construction and Euler-based Fenwick with range add + point query make evaluating P_x(z) possible offline during a label sweep.
- P_x(z) is monotone in z, so parallel binary search finds, for each query, the smallest l where the cumulative count reaches t.
- The b-th y in the chosen l-group (sorted by label) is the b-th smallest label in subtree(l) minus subtree(a1). A persistent segment tree over labels indexed along the Euler tour supports k-th selection in O(log n).
- All queries are handled without ever materializing L or computing any LCA online per query.

Complexity

- DFS + binary lifting + Euler tour: O(n log n).
- Build persistent segment tree (Chairman Tree): O(n log n) time and O(n log n) memory.
- Parallel binary search on l:
  - O(log n) rounds.
  - Each round: O(n) range updates and O(q) point queries on a Fenwick (each O(log n)).
  - Total: O((n + q) log^2 n).
- One sweep to compute P_x(l−1): O((n + q) log n).
- k-th selection via persistent segment tree: O(q log n).

Overall: about O((n + q) log^2 n) time and O(n log n) memory, which fits n, q ≤ 1e5 and a 4 s time limit in a compiled language.

Implementation notes and pitfalls

- Use 64-bit integers for the final answer, as (x−1)·n^2 can be up to ~1e15.
- Fenwick must support range add and point query (two point adds per range and one point query).
- The PBS round needs the Fenwick reset. Either zero the array each round or track touched indices to reset them efficiently.
- To get child a1 of l on the path to x when l ≠ x:
  a1 = jump_up(x, depth[x] − depth[l] − 1) using the binary lifting table.
- In the persistent segment tree k-th, you walk with four version pointers:
  (R1, L1) for subtree(l) and (R2, L2) for subtree(a1); the count in a child is (R1.child.sum − L1.child.sum) − (R2.child.sum − L2.child.sum).

This approach carefully decomposes the problem into counting-by-threshold and order-statistics in subtree differences, and uses standard offline and persistent data-structure techniques to keep the total cost near O((n + q) log^2 n).