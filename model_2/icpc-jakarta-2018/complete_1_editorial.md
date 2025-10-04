Restatement

You are given an undirected simple graph G with N vertices and M edges. All missing edges are pairs of vertices not directly connected in G. A company will process a fixed list L of all missing pairs in some order; when it evaluates a pair (a, b), it adds the edge if the current degree sum δa + δb is at least k (degrees are taken at the moment of evaluation). Each pair is evaluated exactly once, in the order given by L.

We must find the maximum integer k such that there exists at least one ordering L that makes the graph complete by the end of the pass. For any j > k, no ordering should succeed.


Key ideas

1) Work on the complement

Let H be the complement graph of G: same vertex set, and its edges are exactly the missing pairs of G. Let dG(v) be the degree in G at any point in time, and dH(v) the current degree (number of remaining incident missing edges) in H at the corresponding time.

At every moment we have:
dG(v) + dH(v) = (N − 1) for all vertices v

So the rule “add (u, v) if dG(u) + dG(v) ≥ k” is equivalent to, in H,
dH(u) + dH(v) ≤ T, where T = 2(N − 1) − k

In H, “adding a missing edge” in G is the same as “deleting” that edge from H, which decreases dH(u) and dH(v) by 1. Thus the process becomes:

- Start with H (the complement of G).
- Take a list order of all edges of H.
- When visiting edge e = (u, v), delete it if dH(u) + dH(v) ≤ T; otherwise it stays forever.
- We want to delete all edges of H in one pass, under this rule.

Crucially, in H the degrees only go down, so the condition dH(u) + dH(v) ≤ T becomes easier over time.

2) What threshold T is necessary and sufficient?

The best possible outcome is to choose an order of edges of H that always has dH(u) + dH(v) ≤ T when an edge e = (u, v) is removed. Let sH(e) = dH(u) + dH(v) be the current “2-degree” of e in H (sum of degrees of its endpoints in the current residual graph).

Define the parameter:
D2(H) = max over all subgraphs F of H of min over edges e in F of sF(e)

Interpretation: Look at any subgraph F (choose any subset of edges, any subset of vertices); inside F consider the least sum of endpoint-degrees across its edges. D2(H) is the largest such minimum you can force by choosing the worst subgraph.

Claim:
- The smallest T that allows a complete deletion order of H equals D2(H).
- Therefore the answer is k = 2(N − 1) − D2(H).

Why? Necessity: During any successful deletion order, consider the residual subgraph Ft right before removing e_t. By feasibility, sFt(e_t) ≤ T. The minimum s over edges of Ft is ≤ T. Taking the maximum over all encountered residual subgraphs shows T must be at least D2(H). Sufficiency: If at every step you delete an edge with the current minimum s (a “smallest-last” edge order), the maximum s you ever delete equals exactly D2(H) (this is the standard degeneracy argument, but on the line graph; see below). Hence T = D2(H) suffices and is optimal.

Connection to line graph degeneracy: In the line graph L(H), vertices correspond to edges of H, and two vertices are adjacent if the original edges share an endpoint. The degree of an edge e = (u, v) in L(H) is sH(e) − 2. Thus D2(H) = degeneracy(L(H)) + 2. Our threshold T is “degeneracy of the line graph plus 2”, and k = 2(N − 1) − T = 2N − 4 − degeneracy(L(H)).

3) How to compute D2(H) efficiently

We do not need to build the line graph. We can run a “smallest-last” peeling directly on H’s edges:
- Maintain current degrees dH(v).
- For each remaining edge e = (u, v), maintain s(e) = dH(u) + dH(v).
- Repeatedly remove an edge with minimum s(e); after removal, decrement dH(u), dH(v), and for each still-present edge e’ incident to u or v, decrement s(e’) by 1.
- Track the maximum s(e) encountered; that maximum is D2(H).

Because s(e) ∈ [0, 2N − 2], we can implement the priority by simple integer buckets instead of a heap. This keeps each update O(1) amortized.

Algorithm outline

- Build G’s adjacency (N ≤ 500; an N×N boolean matrix is fine).
- Build H implicitly:
  - For every unordered pair {i, j}, if it is not an edge of G, put it as an edge of H.
  - Let Ē be the number of such edges (the number of missing edges).
  - Compute dH(v) = count of non-neighbors of v = (N − 1) − dG(v).
  - For each vertex v, store the list of incident edges of H (edge indices).
- Initialize for each edge e = (u, v) its score s(e) = dH(u) + dH(v) and insert it into bucket s(e).
- Maintain:
  - present[e] = true if e is not yet removed.
  - A current bucket index cur pointing to the smallest non-empty bucket.
  - answer_T = 0.
- While there are edges left:
  - Advance cur until a non-empty bucket is found.
  - Pop some edge e from bucket[cur].
  - If present[e] is false, skip (lazy duplicates).
  - Otherwise remove it: present[e] = false, answer_T = max(answer_T, s(e)).
  - Let e = (u, v). Do:
    - dH(u) -= 1; for each present edge e’ incident to u, do s(e’) -= 1 and push e’ to bucket[s(e’)].
    - dH(v) -= 1; for each present edge e’ incident to v, do the same.
- Finally the result is k = 2(N − 1) − answer_T.

Pseudocode (high level)

- read N, M; build adjacency A[N][N]
- compute dG[v]
- build list Edges = [] and incidence EdgesByVertex[v] for H:
  for 1 ≤ i < j ≤ N:
    if not A[i][j]:
      id = Edges.push((i, j))
      EdgesByVertex[i].push(id)
      EdgesByVertex[j].push(id)
- if Edges is empty: k can be anything, but constraints guarantee at least one missing edge
- compute dH[v] = (N − 1) − dG[v]
- for each edge id with endpoints (u, v):
    s[id] = dH[u] + dH[v]
    put id into bucket[s[id]]
- cur = 0, answer_T = 0
- present[id] = true for all edges
- removed = 0
- while removed < |Edges|:
    while cur ≤ 2N − 2 and bucket[cur] empty: cur++
    take some id from bucket[cur]
    if not present[id]: continue
    present[id] = false; removed += 1; answer_T = max(answer_T, s[id])
    (u, v) = endpoints[id]
    dH[u] -= 1
    for each e’ in EdgesByVertex[u]:
        if present[e’]:
            s[e’] -= 1
            push e’ to bucket[s[e’]]
    dH[v] -= 1
    for each e’ in EdgesByVertex[v]:
        if present[e’]:
            s[e’] -= 1
            push e’ to bucket[s[e’]]
- output k = 2(N − 1) − answer_T

Why this is correct

- Complement translation is exact: dG(u) + dG(v) ≥ k at a moment is equivalent to dH(u) + dH(v) ≤ 2(N − 1) − k at that moment, since the two sums add to 2(N − 1).
- In H, degrees only go down as we delete edges, so the edge-removal feasibility for a threshold T is monotone: once an edge is removable, it stays removable later.
- The minimal T that permits full deletion equals D2(H) defined as max over subgraphs F of min over edges e in F of sF(e). Necessity follows because every residual subgraph encountered must have at least one removable edge; sufficiency is achieved by removing edges in nondecreasing order of current s(e) (“smallest-last”), which is the standard degeneracy algorithm applied to the line graph. The maximum s(e) observed in this process equals D2(H).
- Therefore the best k is 2(N − 1) − D2(H).

Complexity analysis

- Building G and H: O(N^2) time; memory O(N^2) bits for adjacency, which is fine for N ≤ 500.
- Let Ē be the number of missing edges (edges of H), Ē ≤ N(N − 1)/2 ≤ 124,750.
- The peeling does O(Ē) deletions. Each deletion decreases degrees of two endpoints and causes exactly one s(e) decrement for every still-present edge adjacent to those endpoints. Over the whole algorithm the number of such updates is Σv dH(v)^2 − 2Ē, which is at most about N^3 in the worst case. With integer buckets, every update is O(1) amortized. Overall time is O(N^2 + Ē + Σv dH(v)^2), which is comfortably fast for N ≤ 500. Memory is O(N^2 + Ē).

Sanity checks on samples

- Sample 1 (N=4, path 1-2-3-4 in G): H has edges (1,3), (1,4), (2,4), dH = [2,1,1,2]. Peeling smallest-last yields maximum s(e) = 3, so k = 2(N − 1) − 3 = 6 − 3 = 3.
- Sample 2 (empty G): H = K5, every edge has s = 8 throughout, so D2(H) = 8, k = 8 − 8 = 0.
- Sample 3: The computation yields D2(H) = 6, so k = 8 − 6 = 2.

Final remarks

- The answer always lies in 0 ≤ k ≤ 2N − 4. If only one edge is missing, D2(H) = 2, giving k = 2N − 4 (the maximum possible).
- This approach avoids backtracking over orders: the existence of a successful order is encoded by a single numeric parameter of the complement graph, computable by a linear-time “smallest-last” edge peeling when using integer buckets.