Restatement

- There are N cities and N proposed undirected roads. Proposal i connects city i to A[i], and comes with a set of allowed materials B[i]. No two proposals form a 2-cycle (A[i] = j and A[j] = i never happens). If all N proposals are built, the undirected graph is connected.
- There are K workers; worker t can build at most one road and only if the material used equals C[t].
- Assign each worker to at most one proposal so that the built roads connect all N cities (unused workers print 0 0). If impossible, print -1.

Key observations

1) Structure of the proposal graph

- Direct each proposal i as i → A[i]. Every node has outdegree 1, and the whole undirected graph is connected. In any weakly connected functional digraph, there is exactly one directed cycle in the component. Because self-loops and 2-cycles are forbidden, the unique cycle has length L ≥ 3.
- The undirected graph therefore is a unicyclic graph: one simple cycle of length L, and every other edge is in a tree hanging off this cycle.

2) Which proposals must be built?

- Any edge not on the unique cycle is a bridge in the undirected graph. If you omit such a bridge, its subtree disconnects forever (there is no other edge crossing that cut), so all non-cycle proposals are mandatory.
- On the cycle of length L, you need at least L−1 of those L proposals to keep the graph connected (dropping one cycle edge breaks the cycle into a tree; dropping two or more disconnects the graph).
- Therefore any feasible connected assignment must:
  - build every non-cycle (tree) proposal,
  - and build at least L−1 cycle proposals.
- Consequently, at least N−1 workers are required. If K < N−1, the answer is immediately -1.

3) Resources vs proposals is a matching problem

- Each proposal i needs exactly one worker whose material is in B[i].
- Each worker can be used once.
- This is a bipartite matching between proposals and workers, with an edge if C[t] ∈ B[i].
- However, to stay within time and memory, it is better to group workers by material: treat each material value m as a right-node with capacity equal to the number of workers having C[t] = m. Then proposal i connects to material m if m ∈ B[i]. This replaces up to K identical worker nodes by a single capacity node and keeps the edge count bounded by sum M[i] (≤ 10^4).

Algorithm

Outline

- Find the unique cycle.
- Build a flow network proposals → materials with capacities.
- Phase 1: force-match all mandatory (non-cycle) proposals. If you cannot, answer -1.
- Phase 2: with all non-cycle proposals kept matched, extend the flow to match as many cycle proposals as possible. If you can match at least L−1 of them, you are done; otherwise output -1.
- Finally, decode the aggregated material assignment back into concrete workers.

Details

A) Identify the unique cycle

- Let indeg[v] be the number of proposals i such that A[i] = v.
- Push all nodes v with indeg[v] = 0 into a queue.
- Repeatedly pop u, mark u as off-cycle, and decrement indeg[A[u]]; if indeg[A[u]] becomes 0, push it.
- After this elimination, the remaining nodes (indeg[v] > 0) form exactly the unique directed cycle. Let C be this set, with size L. The non-cycle proposals are the indices i ∉ C; cycle proposals are i ∈ C.

B) Preprocess materials

- Build a map from material value to list of worker indices having that material. Let count[m] be the size of that list.
- When building edges from proposals to materials, ignore any material in B[i] that has count[m] = 0 (no worker knows it). If a non-cycle proposal i ends up with an empty list, you can immediately return -1.

C) Flow graph (aggregated by material)

- Source S connects to each proposal node i with capacity 1.
- Proposal i connects to material-node m with capacity 1 if m ∈ B[i] and count[m] > 0.
- Each material-node m connects to sink T with capacity count[m].

D) Two-phase max flow (to respect “mandatory vs optional”)

- Phase 1 (mandatory only): Temporarily enable S→i edges only for non-cycle proposals i ∉ C (capacity 1). Run max flow.
  - If the total flow f1 < number of non-cycle proposals (i.e., N−L), it is impossible to cover all mandatory proposals; print -1.
- Phase 2 (add cycle proposals): Now also enable S→i edges for cycle proposals i ∈ C (capacity 1). Continue running the flow, starting from the Phase 1 solution (or recompute but it’s more efficient to reuse).
  - Let f2 be the final max flow value. The number of matched cycle proposals is f2 − f1.
  - If f2 − f1 ≥ L−1, we can build all mandatory proposals and at least L−1 cycle proposals, thus connect the cities. Otherwise, print -1.

This two-phase approach ensures all non-cycle proposals stay covered while we maximize the number of cycle proposals. Augmenting paths may rematch which worker builds which mandatory proposal, but mandatory proposals remain covered because augmentations originate from unmatched cycle proposals only.

E) Constructing the output assignment

- From the flow, for each proposal i, find the unique material m such that 1 unit of flow goes i → m. For each material m, you have count[m] workers; assign a distinct unused worker with that material to proposal i. This is always possible because we obey the material-node capacities.
- For each worker not used by the flow, print “0 0”.
- For a used worker who builds proposal i, print “i A[i]” (cities can be in any order).

Why this is correct

- The functional graph on N nodes with N edges and connected underlying undirected graph has exactly one directed cycle (hence one undirected simple cycle). All non-cycle edges are bridges in the undirected graph, so any connected spanning subgraph must include all of them. Among L cycle edges, at least L−1 are necessary and sufficient to keep that part connected.
- The aggregated flow network faithfully models the resource constraints:
  - S→proposal enforces that every built proposal consumes exactly one worker.
  - proposal→material enforces that the worker’s material is allowed by that proposal.
  - material→T with capacity count[m] enforces that you cannot use more workers of a material than available.
- Phase 1 computes whether there exists a feasible assignment covering all mandatory proposals. If not, no solution exists at all.
- Phase 2 maximizes the count of additionally covered cycle proposals while maintaining coverage of mandatory proposals (standard augmenting path behavior). If this maximum is less than L−1, then even the best reassignments cannot reach connectivity; if it’s at least L−1, selecting those matched proposals yields a connected built graph.
- The final mapping from material units to concrete worker indices is always possible because we never exceed the per-material capacities.

Complexity

- Cycle detection via indegree peeling: O(N).
- Let U be the number of materials that appear among the workers and somewhere in some B[i], and let E’ = sum over i of |B[i] ∩ workers’ materials| ≤ 10^4.
- Flow graph has O(N + U) nodes and O(N + E’ + U) edges. Running a max flow (e.g., Dinic) twice on this small graph is easily within 2 seconds and 256 MB. This avoids the potential blow-up of building edges to every individual worker.

Pseudocode (sketch)

- Read N, K; read A[i], B[i]; read C[t].
- Build map M: material value → list of worker indices; count[m] = |M[m]|.
- Filter each B[i] to keep only materials m with count[m] > 0.
- Find cycle set C using indegree peeling; L = |C|.
- If K < N−1: print -1 and exit.
- Build flow network with nodes: S, proposals 1..N, material nodes for all m with count[m] > 0, T.
- Add arcs:
  - For i ∉ C: S→i cap 1.
  - For each i and each m ∈ filtered B[i]: i→m cap 1.
  - For each material m: m→T cap count[m].
- Run max flow; let f1 be the flow. If f1 < N−L: print -1.
- Enable S→i cap 1 for i ∈ C (add these arcs) and continue the max flow; let f2 be the new flow.
- If f2 − f1 < L−1: print -1.
- Otherwise, for each saturated edge i→m (flow 1), pop one unused worker index t from M[m] and assign t to build (i, A[i]).
- For each worker t:
  - If assigned to some i: print “i A[i]”.
  - Else: print “0 0”.

Notes and pitfalls

- Materials are up to 1e9, so always compress/map them with a hash map.
- Do not add proposal→material edges for materials not present among workers.
- If any non-cycle proposal ends with no usable materials after filtering, answer is immediately -1.
- It’s allowed to build all L cycle edges (if K permits); the resulting graph is still connected. The algorithm naturally does this when possible.

This approach meets the time and memory limits and is robust because it isolates the only combinatorial freedom (choosing which cycle edge to possibly skip) and handles the resource constraints with a single small max flow.