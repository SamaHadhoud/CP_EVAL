Restatement

You are given a connected simple graph G on n vertices with m edges, where every edge belongs to at most one simple cycle (i.e., G is a cactus). Each vertex v has a positive value a[v]. Over all independent sets S of G, compute the sum of (sum of a[v] over v in S) squared, modulo 998244353:
sum over S independent of (sum_{v in S} a[v])^2.

Constraints: n ≤ 1e5, m ≤ 2n, time 1s. The “at most one simple cycle per edge” is the key structural property.

Key ideas

1) What to aggregate

Let for a subgraph H, over all independent sets of H:

- cnt(H): number of independent sets
- sum1(H): sum over S of sum_{v in S} a[v]
- sum2(H): sum over S of (sum_{v in S} a[v])^2

We want sum2(G).

2) How to combine disjoint parts

If two vertex-disjoint subgraphs A and B are independent of each other (no edges across A and B), then any independent set of A ∪ B is a product choice from A and from B, and for each choice the sum of weights adds. This gives the “product” of triples:

Let A = (cA, s1A, s2A), B = (cB, s1B, s2B). Then for A ⊗ B:

- cnt = cA * cB
- sum1 = s1A * cB + cA * s1B
- sum2 = s2A * cB + cA * s2B + 2 * s1A * s1B

All operations are modulo MOD = 998244353.

When we have a choice between two disjoint families (e.g., “child taken” vs “not taken”), we “add” triples component-wise:

- (c, s1, s2) + (c', s1', s2') = (c + c', s1 + s1', s2 + s2').

Adding one fixed vertex v (with value a[v]) to every set in a triple T multiplies by the atomic triple V(v) = (1, a[v], a[v]^2):
T_with_v = T ⊗ V(v).
Equivalently:
sum1' = sum1 + cnt * a[v]
sum2' = sum2 + 2 * a[v] * sum1 + cnt * a[v]^2.

This small algebra is the engine of the solution.

3) Tree DP (warm-up)

On a tree, the classic independent set DP has two states per node v:

- dp0[v]: v is not chosen
- dp1[v]: v is chosen

We lift each state to a triple (cnt, sum1, sum2).

Transitions over children u:

- If v = 0, u can be 0 or 1:
  dp0[v] = product over u of (dp0[u] + dp1[u]).
- If v = 1, u must be 0:
  dp1[v] = product over u of dp0[u], then add v’s value (multiply by V(v)).

The product is the ⊗ above; + is the component-wise sum.

Finally, the answer for the tree is sum2 of (dp0[root] + dp1[root]).

4) From trees to cacti: handling cycles

In a cactus, each block is either a single edge (part of a tree) or a simple cycle. Cactus graphs are glued by cut vertices, and the “block-cut” structure is a tree. We can therefore still do a bottom-up DP if we handle a cycle as a single “super-child”.

Core idea:
- For every cycle C that touches v, contract C to one component that offers two triples to v:
  - C0: contribution when v is forced to be 0
  - C1: contribution when v is forced to be 1
These triples encode all vertices on the cycle except v (plus their off-cycle subtrees), with the adjacency constraint around the ring respected.

Then v’s combination is uniform:
- dp0[v] = product over all incident components c of comp0[c]
- dp1[v] = product over all incident components c of comp1[c], then multiply by V(v)

Here, a “component” c is either:
- a tree child u (edge (v, u) not in any cycle), with
  comp0[u] = dp0[u] + dp1[u], comp1[u] = dp0[u];
- a cycle C anchored at v (defined below), with comp0[C] = C0 and comp1[C] = C1.

5) Building cycles and anchoring

Run one DFS to detect all cycles. For each back edge (u → v) where v is an ancestor, collect the cycle vertices along the tree path v → … → u plus the back edge. Record the cycle in order as [v = x1, x2, …, xk]. Mark these edges as “cycle edges” so they are not used as tree edges. Anchor this cycle at the ancestor v = x1. In a DFS tree, x2, …, xk are in the subtree of v, so they can be computed before v in a postorder traversal.

After DFS, for each vertex we know:
- its tree children (edges not used in cycles),
- the list of cycles it anchors, each given by the ordered list [v, x2, …, xk].

6) DP on a cycle (to compute C0, C1)

For a cycle anchored at v = x1 with ordered vertices [x1, x2, …, xk]:

- For each i ≥ 2, we already have the two base states:
  base0[xi] = dp0[xi] using only their subtree and incident cycles anchored below xi
  base1[xi] = dp1[xi] using only their subtree and incident cycles anchored below xi
These base states include xi’s own vertex weight in base1[xi] (because xi is chosen in dp1[xi]) and exclude any cycle edge of the current cycle because those are treated here.

We need C0 and C1 which exclude v itself and represent only the contribution of x2..xk under the cycle’s adjacency constraints and the fixed state of v:

- Case v = 0 (C0): v puts no restriction on x2 and xk beyond each other; this is a simple path DP along x2 → x3 → … → xk:
  Initialize
    cur0 = base0[x2], cur1 = base1[x2]   // x2 can be 0 or 1
  For i = 3..k:
    new0 = (cur0 + cur1) ⊗ base0[xi]     // xi = 0; previous can be 0 or 1
    new1 = cur0 ⊗ base1[xi]              // xi = 1; previous must be 0
    cur0 = new0, cur1 = new1
  Result
    C0 = cur0 + cur1                      // xk can be 0 or 1; v = 0 imposes nothing at the end

- Case v = 1 (C1): x2 = 0 and xk = 0 are forced by adjacency to v:
  Initialize
    cur0 = base0[x2], cur1 = zero         // x2 must be 0
  For i = 3..k:
    new0 = (cur0 + cur1) ⊗ base0[xi]
    new1 = cur0 ⊗ base1[xi]
    cur0 = new0, cur1 = new1
  Result
    C1 = cur0                             // enforce xk = 0 at the end

All operations are the triple + and ⊗ defined above.

7) Full algorithm

- Preprocess
  - DFS the graph once.
  - For each back edge to an ancestor, extract its cycle vertices in order and mark cycle edges.
  - For each vertex, store its tree children (non-cycle edges) and list of anchored cycles.

- Postorder DP
  - Recurse on the tree formed by non-cycle edges.
  - At vertex v:
    1) Combine all tree children u:
       P0 = (1, 0, 0), P1 = (1, 0, 0)
       For each u:
         P0 = P0 ⊗ (dp0[u] + dp1[u])
         P1 = P1 ⊗ dp0[u]
    2) For each cycle C anchored at v, compute (C0, C1) by the cycle DP above and multiply them in:
         P0 = P0 ⊗ C0
         P1 = P1 ⊗ C1
    3) Finish v:
         dp0[v] = P0
         dp1[v] = P1 ⊗ (1, a[v], a[v]^2)   // add v when v is chosen
  - After processing the root r:
    total = dp0[r] + dp1[r]
    Answer = total.sum2 mod MOD

Correctness

- The triple algebra is correct for disjoint unions: counts multiply; sums add; second moments follow from (A + B)^2 = A^2 + B^2 + 2AB. Thus + and ⊗ compute exactly the aggregate cnt, sum1, sum2 over independent sets for unions of independent components.
- Tree edges: The two states per vertex enforce the adjacency constraint to children: if v = 1 then each child must be 0; if v = 0, each child is free (0 or 1). The product across children is valid because subtrees are disjoint.
- Cycles: In a cactus, each cycle is a 2-connected component, and different cycles are edge-disjoint; cutting the cycle at the anchor v turns it into a path whose endpoints face a known boundary at v. The DP described enumerates exactly all choices on the cycle that are compatible with v = 0 or v = 1, while incorporating each vertex xi’s off-cycle subgraph via base0[xi], base1[xi]. The wrap-around (xk adjacent to v) is handled by the end condition (for v = 1 we force xk = 0; for v = 0 we don’t).
- At a vertex v with multiple incident components (tree children and/or multiple cycles anchored at v), these components only meet at v and are otherwise disjoint. Conditioning on the state of v and multiplying their contributions is therefore valid. Finally, when v = 1 we include a[v] exactly once (multiplying by V(v)).
- Since the block-cut structure is a tree, a postorder traversal from any root processes every off-cycle subtree and every cycle before combining at its anchor and parent. By induction on this traversal, dp0[v], dp1[v] correctly represent the triples for the entire subgraph below v with v fixed to 0 or 1. The root’s sum gives all independent sets of G.
Therefore, the computed sum2 equals the required sum over S of (sum a[v])^2.

Complexity and limits

- Each tree edge is visited once; each cycle is processed once with a linear DP along its vertices. The sum of cycle lengths is O(m). Total time O(n + m), which is fine for n ≤ 1e5, m ≤ 2n.
- Memory O(n + m) for adjacency and O(n) for DP triples.
- All arithmetic is modulo 998244353.

Pseudocode sketches

Trip = (c, s1, s2) with operations mod MOD.

- add(A, B):
  return (A.c + B.c, A.s1 + B.s1, A.s2 + B.s2)

- mul(A, B):    // disjoint union
  c  = A.c * B.c
  s1 = A.s1 * B.c + A.c * B.s1
  s2 = A.s2 * B.c + A.c * B.s2 + 2 * A.s1 * B.s1
  return (c, s1, s2)

- withVertex(A, a): return mul(A, (1, a, a*a))

DP at vertex v:
- Start P0 = P1 = (1, 0, 0)
- For each tree child u:
  P0 = mul(P0, add(dp0[u], dp1[u]))
  P1 = mul(P1, dp0[u])
- For each anchored cycle C at v (list [v, x2..xk]):
  // compute (C0, C1)
  // v = 0:
  cur0 = base0[x2] = dp0[x2]
  cur1 = base1[x2] = dp1[x2]
  for i = 3..k:
    new0 = mul(add(cur0, cur1), dp0[xi])
    new1 = mul(cur0, dp1[xi])
    cur0 = new0, cur1 = new1
  C0 = add(cur0, cur1)
  // v = 1:
  cur0 = dp0[x2]; cur1 = (0, 0, 0)
  for i = 3..k:
    new0 = mul(add(cur0, cur1), dp0[xi])
    new1 = mul(cur0, dp1[xi])
    cur0 = new0, cur1 = new1
  C1 = cur0
  // multiply into v
  P0 = mul(P0, C0)
  P1 = mul(P1, C1)
- dp0[v] = P0
- dp1[v] = withVertex(P1, a[v])

At the root r:
- total = add(dp0[r], dp1[r])
- output total.s2

Notes and pitfalls

- When extracting cycles by DFS, record the ordered sequence from the ancestor (anchor) down to the current vertex, and mark edges used by cycles. Each such cycle is processed exactly once at its anchor.
- A vertex can belong to several cycles, but each cycle is processed at exactly one anchor (the deepest ancestor on its DFS back edge). Combining multiple cycles at v is handled by the uniform component-multiplying step (multiply all C0/C1 for cycles at v along with children).
- Use 64-bit temporaries when multiplying before taking modulo.

Why this fits the limits

All loops are linear in n + m; the algebra is constant-time per edge/vertex, so it runs in well under 1 second for n ≤ 1e5. Memory usage is linear.