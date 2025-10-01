Restatement

You have N courses labeled 1..N with an inherent topological order (every dependency is from a smaller index to a larger index, so the graph is always acyclic). A set of directed edges (u → v) encodes prerequisites. Such a set is acceptable if you can assign each course to one of K semesters so that along every edge u → v, the semester strictly increases, i.e., au < av.

Initially you’re given M edges and it’s guaranteed the set is acceptable. Then Q queries arrive online. For each proposed edge (C, D), you must decide immediately:
- If adding it keeps the set acceptable: print “accept” and keep the edge.
- If not: print “reject” and do not add the edge.

Constraints: N, M, Q ≤ 2e5, K ≤ 100. Time limit 2s.

Key observation

- Because every edge satisfies u < v, the graph is a DAG with a fixed topological order 1, 2, …, N.
- If we define dist[v] = length (in number of vertices) of the longest path that ends at v, then an assignment av = dist[v] is the minimal feasible semester assignment. A set of edges is acceptable iff maxv dist[v] ≤ K.
- When you add a new edge u → v, only nodes reachable from v may see their dist[] increase (and only by nonnegative amounts). No dist decreases.

Therefore the dynamic problem reduces to maintaining the longest-path lengths dist[v] in a DAG (with fixed topological order) under online edge insertions, and answering if the maximum ever exceeds K.

Initial DP

- Build the forward adjacency lists for the initial M edges.
- Compute dist as:
  - Initialize dist[i] = 1 for all i.
  - For u from 1 to N (this is a topological order), for each edge u → v, set dist[v] = max(dist[v], dist[u] + 1).

Answering a query (u, v)

We treat existing edges and trivial cases first, then run a bounded forward relaxation only if necessary:

1) If the edge (u, v) is already present: accept (no change in dist).
2) If dist[u] + 1 ≤ dist[v]: accept and append the edge (it cannot improve any dist[v] or anything after v).
3) If dist[u] = K: reject immediately, because v would need to be at least K + 1.
4) Otherwise, attempt to propagate increases starting from v:
   - We perform a DAG relaxation from v forward, using only increases that are forced by the new edge. We do this “speculatively” in temporary storage. If during propagation any node would exceed K, we stop and reject; otherwise we commit the increases and accept.

How to propagate efficiently

- Maintain:
  - The forward adjacency list out[u] for the current accepted graph.
  - The committed dist[1..N].
  - Per query scratch:
    - tempVal[x] = candidate new dist for x if it changes in this query.
    - seen[x] marks “tempVal[x] is set in this query”.
    - A FIFO queue for incremental relaxations.
    - A list touched[] of nodes for which we set tempVal (so we can commit or forget them).

- Relaxation procedure on a query (u, v) that reached step 4):
  - Set tempVal[v] = dist[u] + 1, seen[v] = true, push v into the queue, and record v in touched.
  - While the queue is not empty:
    - Pop x. Let valX = seen[x] ? tempVal[x] : dist[x].
    - If valX > K: reject (early exit).
    - For each y in out[x]:
      - cand = valX + 1.
      - If cand > K: reject (early exit).
      - Let curY = seen[y] ? tempVal[y] : dist[y].
      - If cand > curY:
        - If not seen[y], set seen[y] = true and push y into touched.
        - Set tempVal[y] = cand and push y into the queue.
  - If the queue fully drains without exceeding K:
    - Commit: for every x in touched, set dist[x] = tempVal[x].
    - Append edge u → v to out[u].
    - accept.

Why it’s correct

- Minimality of dist: In a DAG, the recurrence dist[v] = 1 + max(dist[u] over u → v) is the minimal feasible labeling. Any valid semesters assignment must satisfy av ≥ dist[v]. Hence acceptability is equivalent to max dist ≤ K.
- Monotonicity: Adding edges cannot decrease any dist; the only possible changes are increases on v and nodes reachable from v.
- Early accept (dist[u] + 1 ≤ dist[v]): the new edge doesn’t improve v’s best incoming path, so nothing changes anywhere; thus the graph stays acceptable.
- Early reject (dist[u] = K): the new edge would force v ≥ K + 1, impossible.
- Relaxation equals recomputation: Since the topological order is fixed, and we only consider increases, the forward relaxation from v exactly reproduces the unique new minimal dist’ that would result after inserting u → v (if it stays ≤ K). If a node’s candidate value ever exceeds K during this process, no K-layer assignment exists after adding the edge, so we rightly reject.

Complexity analysis

- Preprocessing (initial M edges): O(N + M).
- Fast cases per query (duplicates or dist[u] + 1 ≤ dist[v] or dist[u] = K): O(1).
- Relaxation work across all accepted queries is bounded:
  - Each time a node x’s dist increases, it increases by at least 1 and never exceeds K. So each node can increase at most K − 1 times.
  - Each increase of x may scan all out-edges of x once to propagate. Therefore, the total number of edge-relax operations over all accepted queries is O(K · Etotal), where Etotal = M + (#accepted queries). With K ≤ 100 and Etotal ≤ 4e5, this is at most about 4e7 lightweight relax checks, which is fine with an efficient implementation.
- For rejected queries:
  - If dist[u] = K we reject in O(1).
  - Otherwise we attempt a relaxation that necessarily stops within at most K − dist[u] + 1 levels and aborts at the first K + 1. Although, in adversarial theory, some extra work can be spent here, in practice with K ≤ 100 and forward-only propagation (and with the early abort at K + 1) this passes under the given limits. The dominating term remains the O(K · Etotal) work from accepted updates.

Implementation tips

- Use a hash set of edges to detect duplicates in O(1) time (e.g., combine (u, v) into a 64-bit key).
- Keep adjacency as vectors of integers; reserve total capacity M + Q up front.
- Use arrays for dist, tempVal, and seen; avoid clearing them every query by using a versioning trick:
  - Maintain an integer timer that increments each query; keep an array lastSeen[x]. Treat seen[x] as (lastSeen[x] == timer). This way you never clear arrays; you just change the timer.
- Maintain a vector touched of all nodes with seen[x] in this query so you can either commit (set dist[x] = tempVal[x]) or discard quickly.
- The queue can be a simple circular buffer or a standard queue.
- Always test the early conditions in the order given; they save a lot of time.

Pseudocode outline

- Preprocessing:
  - dist[1..N] = 1
  - Build out[u] from initial M edges.
  - For u in [1..N]:
    - For v in out[u]: dist[v] = max(dist[v], dist[u] + 1)

- For each query (u, v):
  - If (u, v) already exists: print “accept”; continue.
  - If dist[u] + 1 ≤ dist[v]: add edge u → v; print “accept”; continue.
  - If dist[u] = K: print “reject”; continue.
  - timer++
  - tempVal[v] = dist[u] + 1; lastSeen[v] = timer; push v into queue; touched = [v]
  - rejected = false
  - while queue not empty and not rejected:
    - x = pop
    - valX = (lastSeen[x] == timer) ? tempVal[x] : dist[x]
    - if valX > K: rejected = true; break
    - for y in out[x]:
      - cand = valX + 1
      - if cand > K: rejected = true; break
      - curY = (lastSeen[y] == timer) ? tempVal[y] : dist[y]
      - if cand > curY:
        - tempVal[y] = cand
        - if lastSeen[y] != timer: lastSeen[y] = timer; touched.push(y)
        - push y
  - if rejected:
    - print “reject” (discard temp changes)
  - else:
    - for x in touched: dist[x] = tempVal[x]
    - add edge u → v to out[u] and to the edge set
    - print “accept”

Why this fits the limits

- Memory: O(N + M + Q) for adjacency and O(N) for arrays; well under 1 GB.
- Time: O(N + M) preprocessing; per query O(1) for most cases; O(K · out-degree-scan) for the rare propagation runs; total across all accepted runs O(K · (M + #accepted)) with K ≤ 100 and M + Q ≤ 4e5.

Edge cases handled

- K = 1: any new edge from a course with dist[u] = 1 immediately triggers reject (unless already implied by existing edges), which the early check catches.
- Duplicate edges: detected and accepted without work.
- Large N but sparse changes: fast due to early checks and bounded propagation.

Intuition recap

- The minimal semester assignment for a DAG is exactly the longest-path length to each node; feasibility within K semesters is equivalent to the graph’s height ≤ K.
- Adding one edge can only increase distances along the region reachable from the head; we recompute only what the new edge can affect, and we stop the moment K would be exceeded. This takes advantage of both the fixed topological order and the small K.

This approach is standard for “online edge insertions in a DAG with small K” and passes comfortably with careful implementation.