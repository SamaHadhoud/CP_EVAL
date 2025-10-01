Restatement

You are given M directed slides between 8 buttons laid in a circle. Each slide i goes from Xi to Yi and has speed Vi (slides can repeat in type—same Xi, Yi, Vi—but they are distinct items and can be used at most once). Salt wants to build a level consisting only of “chain slides” that are closed (each chain starts and ends at the same button), and the union of all chosen slides must satisfy:

- Every button appears as a starting button of at least one chosen slide.
- No slide type is used more than once.
- The total number of chosen slides is at least K.

Among all such valid choices, minimize the speed range max(V) − min(V) over the chosen slides. If impossible, print -1.

Key observations

1) What does “only chain slides that end where they start” mean combinatorially?

- Every chosen slide must belong to a directed cycle (possibly a 1-cycle if Xi = Yi).
- Equivalently: If we pick a subset S of slides (as directed edges), the union is a disjoint union of directed cycles if and only if each vertex v has indegreeS(v) = outdegreeS(v). This condition alone already guarantees that the chosen edges decompose into directed cycles.

2) “Every button is a starting button of at least one slide”

- That is exactly outdegreeS(v) ≥ 1 for every vertex v. Combined with indegreeS(v) = outdegreeS(v), it also implies indegreeS(v) ≥ 1 for all v. So at least 8 edges must be chosen (one per button), regardless of K.

3) Optimization over speeds

- We are free to choose any subset of slides, but all chosen slides must lie inside some speed window [L, R]. We want to minimize R − L.
- Therefore, sort all slides by speed and use a two-pointer (sliding window) technique over the sorted list to try candidate windows.

4) Feasibility inside a fixed window

- Let E be the multiset of slides with speeds in the current window [L, R]. Let c[u][v] be the number of available slides from u to v within the window.
- We need to choose integers f[u][v] with 0 ≤ f[u][v] ≤ c[u][v] such that:
  - For each button v: sum_w f[v][w] = sum_w f[w][v] ≥ 1 (balance and at least one outgoing).
  - Total slides used S = sum_{u,v} f[u][v] ≥ K.

This is a “circulation with capacities” problem on a tiny graph (8 nodes, at most 64 directed pairs). We want to know if there exists an Eulerian multiset of edges (a circulation) that also covers all vertices (each out-degree at least 1) and uses at least K edges in total.

The core idea: min-cost circulation on a tiny network

We can model and solve the per-window feasibility with a min-cost circulation (or min-cost flow) on a fixed-size network. The trick makes the structural constraints automatic, and we only have to check if we can reach at least K edges.

Network construction for a fixed [L, R]:

- Node split per button:
  - For each button v, create two nodes: vin[v] and vout[v].
  - Add an internal edge A_v: vin[v] → vout[v] with lower bound 1 and very large capacity (INF). This enforces “use vertex v at least once” (at least one unit of flow must pass through v).

- Slide edges:
  - For each ordered pair (u, v), add an edge E_{u→v}: vout[u] → vin[v] with capacity c[u][v] and cost −1 per unit flow. Think of one unit of flow on this arc as “select one slide u→v”. Using cost −1 means the solver will try to use as many slides as allowed/possible.

- Objective and constraints:
  - We seek a feasible circulation that respects all capacities and the internal edges’ lower bounds and minimizes total cost. Because internal edges have cost 0 and slide edges have cost −1, the algorithm will use as many slide edges as it possibly can (the more it chooses, the more negative the total cost).

- Handling lower bounds (standard trick):
  - For an edge with lower bound l and upper bound u, temporarily replace it by an edge with bounds [0, u − l], and store the lower bound in node demands: b(from) −= l and b(to) += l.
  - After processing all low bounds, add a super-source SS and a super-sink TT:
    - For each node x with b[x] > 0, add edge SS → x with capacity b[x].
    - For each node x with b[x] < 0, add edge x → TT with capacity −b[x].
  - Add an extra edge TT → SS with very large capacity (enables arbitrary circulation beyond just satisfying the lower bounds).
  - Run min-cost max-flow from SS to TT.
  - If not all edges out of SS are saturated, there is no feasible circulation that satisfies the per-vertex “≥ 1” requirement → window infeasible.
  - Otherwise, you now have a feasible circulation respecting lower bounds. Because slide edges have cost −1, by further pushing flow around cycles (which the TT → SS trick permits), the solver minimizes total cost, i.e., maximizes the number X of chosen slides. The achieved minimal cost equals −X.

- Decision:
  - If X ≥ K, then this window is feasible (we can choose at least K slides).
  - Else, infeasible.

Why this works

- Circulation = indegree = outdegree at each vertex, automatically enforced by flow conservation on vin/vout with only inter-vertex movement allowed via the slide arcs (vout[u] → vin[v]) and per-vertex throughput via the internal arc vin[v] → vout[v].
- Lower bound 1 on each internal arc forces at least one unit of flow to pass through each vertex. In terms of chosen edges, that means each vertex must have at least one outgoing and one incoming selection (since every unit that passes vin → vout must be “fed” and “drained” via slide edges).
- Each unit of flow on a slide edge directly corresponds to selecting one slide of that type. “At most once per slide type” is enforced because capacities are integer and bounded by the count c[u][v] inside the window.
- Minimizing cost with cost −1 per slide simply pushes as many slides as possible while respecting all constraints. Thus the minimum cost equals −(max #slides). So feasibility “at least K” is just checking −cost ≥ K.

Two-pointer outer loop (minimizing the speed range)

- Sort all slides by speed. Let the sorted array be edges[0..M−1] with speeds V[0..M−1].
- Maintain a window [l, r] (inclusive) over the sorted array.
- Maintain counts c[u][v] of slides currently in the window; also track per-vertex totals quickly for early pruning.
- Procedure:
  - Initialize l = 0, r = −1, answer = +∞.
  - For l from 0 to M−1:
    - While r + 1 < M and the current window is infeasible, increment r by 1 and insert edges[r] into c[u][v].
      - Optional early prunes before running flow:
        - If there exists a button v with out_cap[v] = 0 (no outgoing slide in window), then infeasible.
        - If total slides in window < max(K, 8), then infeasible.
    - If the window is feasible, update answer = min(answer, V[r] − V[l]).
    - Remove edges[l] from c[u][v] (slide window’s left end).
  - If answer stayed +∞, print −1, else print answer.

Because r only increases and l increases once per iteration, the number of feasibility checks is O(M). The per-check network is tiny (16 nodes, at most 64 capacity-collapsed slide edges plus 8 internal edges), and min-cost flow on such a graph is very fast in practice.

Pseudocode (high level)

- BuildFeasibleMaxSlides(c, K):
  - Create graph nodes vin[1..8], vout[1..8].
  - Add internal edges vin[v] → vout[v] with lower bound 1, capacity INF, cost 0.
  - For each u, v:
    - If c[u][v] > 0, add edge vout[u] → vin[v] with lower bound 0, capacity c[u][v], cost −1.
  - Apply lower-bound transformation to get demands b[node].
  - Add SS, TT; arcs from SS/TT according to b; add TT → SS with capacity INF, cost 0.
  - Run min-cost max-flow from SS to TT; if not all SS outgoing arcs are saturated → return “infeasible”.
  - The min total cost equals −X where X is the maximum number of slides selectable in this window.
  - Return “feasible” if X ≥ K else “infeasible”.

- Main:
  - Sort slides by speed.
  - Sliding window with c[u][v] maintained incrementally.
  - For each expansion step where early prunes don’t rule out feasibility, call BuildFeasibleMaxSlides(c, K).
  - Track minimal range.

Correctness argument

- Any valid level is a multiset of slides forming a disjoint union of directed cycles. This is equivalent to indegree = outdegree at every vertex. The extra requirement “each button is a starting button of at least one slide” is exactly outdegree ≥ 1 at every vertex; combined with equality, also indegree ≥ 1. Our internal edges with lower bound 1 enforce exactly that.
- Selecting an edge at most once is modeled by unit capacities (or aggregated capacities per pair).
- The circulation model guarantees that any feasible flow corresponds to a valid selection, and conversely, any valid selection yields a feasible circulation (send one unit along each selected slide edge and pass through each vertex’s internal edge accordingly).
- With cost −1 on each slide edge and 0 on internal edges, the minimum-cost circulation over a feasible network uses as many slides as possible. Therefore, if the minimum cost is at most −K, we can pick at least K slides; otherwise we cannot.
- The two-pointer method considers only O(M) windows (since r and l each move at most M times), and among those it picks the narrowest speed interval that is feasible, which is by definition the optimal solution to minimize max(V) − min(V).

Complexity

- Sorting: O(M log M).
- Sliding window: O(M) feasibility checks.
- For each feasibility check:
  - Network size is fixed (16 nodes). You can collapse parallel slides per pair (u, v) into a single edge with capacity c[u][v], so at most 64 slide edges plus 8 internal edges.
  - Min-cost flow on such a small graph is fast; with successive shortest augmenting paths (SPFA or potentials + Dijkstra), it runs in well under the time limit for M ≤ 1000.
- Memory is small (a few hundred edges at most).

Practical tips

- Early prunes are effective:
  - If any vertex has no outgoing slides in the current window, infeasible.
  - If current window has fewer than max(K, 8) slides overall, infeasible.
- INF can safely be set to the current total number of slides in the window.
- Self-loops (u = v) are allowed and naturally handled; they are 1-cycles.
- Duplicate slides are just additional capacity on the edge (u, v).

Why sample cases behave as shown

- Sample #1: The narrowest window that still allows an Eulerian subgraph covering all 8 buttons with at least 9 edges has range 7.
- Sample #2: With K = 1 but “every button starts at least once” forces at least 8 edges; the graph does not admit such a cover → −1.
- Sample #3/#4: They illustrate when two 4-cycles exist versus when a button cannot be covered; the model captures both perfectly.
- Sample #5/#6: Show that adding a couple of extra slides (even at extreme speeds) can change feasibility and the minimal range.

In summary

- Reduce to selecting an Eulerian multiset of directed edges covering all 8 vertices and using at least K edges.
- For each speed window, check feasibility by a min-cost circulation on a tiny network:
  - Per-vertex internal arc with lower bound 1,
  - Slide arcs with capacity equal to counts and cost −1,
  - Lower-bound handling via standard SS/TT trick.
  - The minimum cost equals −(max number of usable slides).
- Use a two-pointer over sorted speeds to minimize the range.

This approach is correct, handles all edge cases (self loops, duplicates), and easily fits within the given 1s / 1024MB limits.