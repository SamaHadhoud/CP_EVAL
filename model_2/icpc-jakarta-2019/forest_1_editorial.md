Restatement

You are given a tree U with N nodes. For a node u, removing u produces exactly deg(u) connected components (trees). Call u a good cutting point if deg(u) ≥ 2 and all these components are pairwise isomorphic as unlabeled trees. You must decide whether there exists a good cutting point and, if so, output the maximum possible number of components produced by removing exactly one such node. Equivalently: output the maximum degree of a node u whose incident components (after removing u) are all identical as unlabeled trees; otherwise output −1.

Key observations

- Removing u splits the tree into exactly deg(u) components, one per neighbor. Thus, for a good cutting point u, all deg(u) components must be isomorphic as unlabeled trees. The answer is the maximum deg(u) over all such u (with deg(u) ≥ 2).
- The nontrivial part is testing whether several trees (these components) are all isomorphic as unlabeled trees.
- Comparing them as rooted trees at the neighbor nodes is not sufficient: two components can be isomorphic as unrooted trees even if the specific attachment vertices (the neighbors of u) occupy non-equivalent positions in the shape. Therefore we must compare unrooted tree isomorphism.

A standard tool: canonical form of an unrooted tree (AHU)

We can assign a canonical encoding to an unrooted tree such that two trees are isomorphic if and only if their encodings are equal. The classical Aho–Hopcroft–Ullman (AHU) method works as follows:

- Centers of a tree:
  - A tree has either 1 center (if its diameter is even) or 2 adjacent centers (if its diameter is odd).
  - Centers can be found in O(size) by repeatedly stripping all leaves until ≤ 2 nodes remain.

- Rooted canonical encoding:
  - For a rooted tree, recursively compute the encodings of all children, sort the multiset of child encodings, and combine them into a parent encoding (e.g., as a string in parentheses or, more efficiently, as a 64-bit hash).
  - Sorting handles sibling permutations.

- Unrooted canonical encoding:
  - If there is one center c: return the rooted encoding with c as root.
  - If there are two centers a and b: encode both sides “anchored at the center edge”. Compute:
    - repA = rooted encoding of a when b is treated as its parent (so this covers the entire side of a excluding b),
    - repB = rooted encoding of b when a is treated as its parent.
    - The unrooted canonical is the symmetric combination of repA and repB (e.g., sort the pair (repA, repB) and combine).

With this, two unrooted trees are isomorphic iff their canonical encodings are equal.

Overall approach

For each node u (with deg(u) ≥ 2), split the tree into its deg(u) components after removing u. For each component, compute its unrooted canonical encoding (via AHU) and check if all encodings are equal. Keep the maximum deg(u) over all such u.

Because N ≤ 4000, we can afford to do this per node u in O(N)–O(N log N) time, which gives an overall O(N^2 log N) solution that easily fits in time.

Detailed steps

1) Build adjacency lists for the tree.

2) For each node u from 1 to N:
   - If deg(u) < 2, skip (cannot be a good cutting point).
   - Mark u as “forbidden” so traversals won’t cross it.
   - For each neighbor v of u, if v is not yet visited in this pass:
     - Collect the component C reached from v without crossing u (DFS/BFS). This will visit disjoint regions; across all neighbors the total size is N−1.
     - Compute the unrooted canonical encoding of C using AHU (see below).
     - Store the encoding in a list encs.
   - If all encodings in encs are equal, u is good; update answer with deg(u).

3) Output the maximum degree found, or −1 if none.

AHU details for a component C

Given the set of nodes in C (collected by DFS/BFS from one neighbor of u, forbidding u), we need only the edges within C; these are the original adjacency edges restricted to C.

- Find centers:
  - Compute degrees within C (count neighbors that are also in C).
  - Push all leaves (deg = 1). Repeatedly remove current leaves, decrement degrees of their neighbors, and push new leaves, until 1 or 2 nodes remain. Those are the centers.

- Rooted encoding function encode_rooted(x, parent):
  - For each neighbor y of x that is inside C and y ≠ parent:
    - h_y = encode_rooted(y, x)
    - Append h_y to a list children
  - Sort children
  - Combine children deterministically into a hash/code.
    - For robustness and speed, prefer a 64-bit hash or a pair of mod hashes over string concatenation. Example pattern:
      - Start from seed S; for each child hash h in order: S = mix(S, h); return finalize(S).
    - The exact formula isn’t important as long as it’s order-sensitive after sorting and has negligible collision chance.

- Unrooted encoding:
  - If one center c: return encode_rooted(c, parent = NONE)
  - If two centers a, b:
    - repA = encode_rooted(a, parent = b)
    - repB = encode_rooted(b, parent = a)
    - Return a symmetric combination of {repA, repB}, e.g., combine(min(repA, repB), max(repA, repB)).

Why we cannot just root at the neighbor of u

If you root the component at the neighbor (the attachment vertex), you may get different rooted encodings for two components that are actually isomorphic as unrooted trees (e.g., a path where u attaches to an endpoint in one component and to an internal node in another). Using the unrooted canonical form via centers removes this dependence on attachment position.

Pseudocode (high level)

- is_good(u):
  - if deg[u] < 2: return false
  - mark all nodes unvisited; mark u as blocked
  - encs = empty list
  - for v in adj[u]:
    - if not visited[v]:
      - comp = DFS(v) that never crosses u; mark visited along the way
      - encs.push_back( unrooted_AHU(comp) )
  - return all encs are equal

- unrooted_AHU(comp):
  - if |comp| == 1: return hash_leaf
  - centers = find_centers(comp)
  - if |centers| == 1:
    - return encode_rooted(centers[0], parent = NONE)
  - else:
    - a, b = centers
    - h1 = encode_rooted(a, parent = b)
    - h2 = encode_rooted(b, parent = a)
    - return glue(min(h1, h2), max(h1, h2))

- encode_rooted(x, parent):
  - child_hashes = []
  - for y in adj[x]:
    - if y in comp and y != parent:
      - child_hashes.push( encode_rooted(y, x) )
  - sort(child_hashes)
  - return combine(child_hashes)  // e.g., 64-bit hashing

Complexity analysis

- For a fixed u:
  - Splitting into components visits each node at most once: O(N).
  - For each component C:
    - Finding centers: O(|C|).
    - Computing rooted encoding(s): sums to O(|C| log deg) due to sorting children at each node; across the component this is O(|C| log |C|) in the worst case.
  - Over all components around u, total work is O(N log N).

- Over all u: O(N^2 log N) time, O(N) memory.
  - With N ≤ 4000 this comfortably fits 1 second in a compiled language if you use integer hashing and avoid building long strings.

Correctness argument

- Removing u yields exactly deg(u) components. The node u is a good cutting point iff deg(u) ≥ 2 and those components are pairwise isomorphic as unlabelled trees.
- The AHU canonical form for unrooted trees is unique up to isomorphism: two unrooted trees are isomorphic iff their canonical encodings are equal. Computing centers and then the rooted encodings at the center(s) and combining them symmetrically is exactly the classic AHU technique for unrooted trees.
- Therefore, for each u, the list of encodings we compute (one per component) are all equal if and only if those components are pairwise isomorphic. Taking the maximum degree among such u thus yields the required answer.

Implementation tips

- Use 64-bit hashing (or double hashing with two independent moduli) rather than strings to keep it fast and memory-light. Make the mixing order-sensitive after sorting child hashes.
- When finding centers, compute degrees only within the component (ignore edges to nodes outside).
- Reuse arrays across iterations over u (e.g., visited flags), or use an integer “timestamp” array to avoid O(N) clearing costs; at N ≤ 4000, a simple clear is fine too.
- Recursion depth up to N may occur; if your language has a small recursion limit, convert encode_rooted to an explicit stack.

Edge cases

- deg(u) = 1: cannot be a good cutting point.
- Star: removing the center yields many single-node components, all identical; answer is N−1.
- Chain: typically only the middle (if any) can be good; your test will correctly detect it.

Summary

- The problem reduces to: for each vertex u with deg(u) ≥ 2, check whether all components obtained by removing u are the same unrooted tree.
- Compare unrooted trees using AHU canonical encodings via centers and sorted child encodings.
- This yields an O(N^2 log N) solution (N ≤ 4000), which fits comfortably in time and memory limits.
- The answer is the maximum degree among good cutting points, or −1 if none exist.