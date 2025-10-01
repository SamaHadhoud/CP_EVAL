Restatement

You are given an undirected graph with N cities and M roads. Time is discrete and repeats every C seconds (0 to C−1). Each road i can only be entered during a daily window [Li, Ri] (inclusive), and takes Ti seconds to traverse once you enter. You may wait in any city for any amount of time. Starting from city 1 at absolute time 0, find the minimum total time to reach city N.

Key observations

- Time-dependent edges: Whether you can enter a road depends on the current time modulo C. However, once you enter, travel time is fixed (Ti); exit time is unrestricted.

- Waiting only at cities: You may wait at cities to align with a road’s open window; there is no benefit to “waiting on an edge” since entry time is the only constrained moment.

- Next admissible entry is easy to compute: If you arrive at a city at time t, let x = t mod C. For a road with window [L, R]:
  - If L ≤ x ≤ R, you can enter immediately (wait = 0).
  - If x < L, wait = L − x (enter later the same day).
  - If x > R, wait = C − x + L (enter at L on the next day).
  This is the earliest possible entry time for that edge; waiting even longer before entering never helps because travel time is independent of the entry moment and there are no restrictions on exit.

- FIFO property (no overtaking): For any road, the earliest arrival as a function of the arrival time at the city is nondecreasing:
  - Let E(t) be the earliest feasible entry time ≥ t (repeating schedule). E(t) is nondecreasing in t.
  - Arrival time via the edge is A(t) = E(t) + Ti, also nondecreasing.
  Consequently, the whole network is FIFO. In FIFO time-dependent graphs, Dijkstra’s algorithm with time-dependent relaxation is correct.

Algorithm

Use Dijkstra’s algorithm on the cities (not on (city, time) states). Dist[u] stores the earliest absolute time you can be at city u. When relaxing an edge, compute the needed wait from the current time modulo C.

Relaxation rule for an edge (u, v) with parameters (Ti, Li, Ri) from city u at time du:
- x = du mod C
- Compute wait:
  - if L ≤ x ≤ R: wait = 0
  - else if x < L: wait = L − x
  - else: wait = C − x + L
- Candidate time to reach v: nd = du + wait + Ti
- If nd < dist[v], update dist[v].

You may terminate early when city N is extracted from the priority queue, since in Dijkstra the first time a node is popped is its final shortest-time value.

Pseudocode (high level)

- Build adjacency list; for each undirected road (a, b, T, L, R), add entries to both a’s and b’s lists.

- Initialize:
  - dist[1] = 0; dist[u] = +∞ for u ≠ 1
  - Min-heap with (0, 1)

- While heap not empty:
  - Pop (du, u); if du > dist[u], continue
  - If u == N, return du
  - For each edge (u -> v, T, L, R):
    - x = du % C
    - if L ≤ x ≤ R: wait = 0
      else if x < L: wait = L − x
      else: wait = C − x + L
    - nd = du + wait + T
    - If nd < dist[v]: dist[v] = nd; push (nd, v)

- Output dist[N]

Correctness argument

- Per-edge optimal waiting: For a fixed edge and arrival time t, the earliest valid entry time is exactly defined by the three cases above. Entering any later than that cannot lead to an earlier arrival at the neighbor because travel time Ti is fixed and exit is unconstrained. So the relaxation computes the true earliest time you can reach the neighbor via that edge.

- FIFO property: For any edge, define A(t) = earliest arrival at the other endpoint if you reach the current endpoint at time t. As shown, A(t) = E(t) + Ti with E(t) the earliest feasible entry ≥ t. Since E(t) is nondecreasing, A(t) is nondecreasing. Compositions of nondecreasing (FIFO) edge functions remain nondecreasing, so the entire network is FIFO.

- Dijkstra on FIFO time-dependent graphs: In FIFO networks, the earliest-arrival label-setting Dijkstra is valid. When a node u is extracted, dist[u] is final. The relaxation rule uses the correct edge travel-time function at the current time dist[u], so all improvements are sound. Therefore the algorithm returns the minimum possible arrival time at city N.

Complexity

- Time: O((N + M) log N) using a binary heap; each edge is relaxed at most twice (undirected), and computing the wait is O(1).
  - With N ≤ 1e5 and M ≤ 2e5, this easily fits in 1s in a typical compiled language.

- Memory: O(N + M) for the adjacency list and distance array. Use 64-bit integers for times:
  - Each edge contributes at most (C − 1) waiting + Ti, and a path may have many edges; totals are well within 64-bit range (on the order of 1e14–1e15).

Edge cases and implementation notes

- Use 64-bit integers for dist and sums. du % C fits in 64-bit even if du is large.

- Windows are inclusive: if x == Li or x == Ri, wait = 0.

- Single-second window (Li == Ri) and full-day window (Li = 0, Ri = C−1) are handled naturally.

- No need to model states by time modulo C or to build layered graphs; computing the next admissible entry is O(1).

Why this handles all cases

- Arbitrary T up to 1e9 and C up to 1e9 are supported; entry constraints repeat every day and only matter through t mod C, which we compute directly.

- Multiple edges and connectivity are naturally handled by Dijkstra; the earliest arrival function per edge respects FIFO, guaranteeing global optimality.

- The algorithm correctly considers waiting at any city implicitly through the per-edge wait computation; there is no benefit to “strategic extra waiting,” since FIFO ensures leaving later cannot make you arrive earlier along any path.

Illustrative run on Sample 1

- C=10.
- From city 1 at t=0:
  - Edge 1–2: x=0 ∈ [0,5] → wait=0, arrive at t=3
  - Edge 1–3: x=0 ∉ [7,8] → wait=7, arrive at t=8
- Next, from city 2 at t=3:
  - Edge 2–3 has window [3,3], x=3 → wait=0, arrive at t=7
- Minimum time to city 3 is 7.

This matches the sample output.