Restatement

You are given a connected undirected graph with N cities and M roads. Each city i has a nonnegative economic value S[i]. There are Q online operations:
- Type 0: point update S[B] ← C
- Type 1: query on two starts (B, C): two people choose any nonnegative integer X (same X for both), then each walks exactly X steps (one edge per day, no staying still, revisits allowed). After X steps they end at cities u and v to minimize |S[u] − S[v]|. Output that minimum.

Key observations

1) “Exactly X steps” + “revisits allowed” means padding by 2:
- If there is any walk from s to t of length d, then for any X ≥ d with X ≡ d mod 2 there is also a walk of length X (just insert back-and-forths along edges). So for a fixed start s and parity p ∈ {0,1}, by choosing X large enough with X mod 2 = p you can reach any node whose distance parity from s equals p.

2) Bipartiteness determines what parities are possible:
- If the graph is non-bipartite (has an odd cycle), you can also change parity by looping through an odd cycle once. Consequently, for any s and any target t, and for all sufficiently large X, there exists a length-X walk from s to t. Therefore both walkers can always synchronize to reach the same city for some large X, so the answer to every Type 1 query is 0.
- If the graph is bipartite with parts A and B, distance parity equals the bipartition parity. From a start in A, after an even X you can be in A, after an odd X you can be in B (and similarly for starts in B). Because X is chosen freely, only parity matters.

3) What does that imply for queries in a bipartite graph?
- If the two starts B and C are in the same part (both in A or both in B), pick any city w in that same part and take an even X large enough; both can reach w at the same time. Hence the answer is always 0.
- If they are in different parts (one in A, the other in B), then:
  - For even X: the first must end in its own part, the second in its own part, i.e., one ends in A and the other in B.
  - For odd X: both parts swap; again one ends in A and the other in B.
  In both cases we are choosing one city from A and one city from B. Since we can make X large, all such pairs are feasible. So the answer is the minimum over all pairs (a ∈ A, b ∈ B) of |S[a] − S[b]|, independent of the particular starting cities.

Thus the whole problem collapses to:
- If the graph is non-bipartite: always output 0 for Type 1 queries.
- Else the graph is bipartite:
  - If query starts are in the same side: answer 0.
  - Else: answer is min |S[a] − S[b]| over a ∈ A, b ∈ B, under online point updates S[i] ← new.

The remaining task is to maintain, under point updates, the minimum absolute difference between one value from A and one value from B.

Data structure for the bipartite case

Let’s maintain the values of both parts in one globally sorted multiset U of pairs (value, id), with id as a unique tie-breaker so equals are totally ordered. Also keep the color (part) of each id: col[id] ∈ {A,B}.

Crucial fact:
- The minimum over a ∈ A, b ∈ B of |S[a] − S[b]| equals the minimum difference between two consecutive elements in the sorted order that belong to different parts.
Proof: If the minimizer (x from A, y from B) are not consecutive in the merged sorted order, there is some z between them. Then either |x−z| ≤ |x−y| or |z−y| ≤ |x−y|. If z is from the same part as x, look further toward y; if z is from the opposite part, you already found a pair of different parts with no larger difference. Repeating shows the optimum occurs across a boundary between adjacent elements of different colors.

Therefore it suffices to maintain all boundaries between consecutive elements of different colors. Let E be a multiset of gaps:
- For every adjacent pair (prev, next) in U with col[prev] ≠ col[next], insert |value[next] − value[prev]| into E (we also need to be able to delete a specific boundary; in practice store it identified by the ordered pair of ids plus its gap).
- Then the answer (when starts are in different parts) is simply min(E).

Updates (changing S[x] to newS)
Only local adjacencies around x can change. We must update U and E accordingly in O(log N).

- Before changing, locate x in U and its current predecessor p and successor q (if any).
- Remove boundaries touching x:
  - If p exists and col[p] ≠ col[x], erase boundary (p,x) from E.
  - If q exists and col[q] ≠ col[x], erase boundary (x,q) from E.
- Remove (value[x], x) from U.
- After removal, p and q become adjacent. If both exist and col[p] ≠ col[q], insert boundary (p,q) with gap |value[q] − value[p]| into E.
- Now insert (newS, x) into U and find its new neighbors p’ and q’.
- Before x’s insertion, p’ and q’ were adjacent. If both exist and col[p’] ≠ col[q’], erase boundary (p’,q’) from E (they stop being adjacent).
- Insert new boundaries touching x:
  - If p’ exists and col[p’] ≠ col[x], insert (p’,x).
  - If q’ exists and col[q’] ≠ col[x], insert (x,q’).

All set/multiset operations are O(log N). The multiset E’s minimum can be read in O(1).

Preprocessing

- Run a BFS/DFS to 2-color the graph and detect bipartiteness. If you ever discover an edge (u,v) with col[u] = col[v], the graph is non-bipartite.
- If non-bipartite: every Type 1 answer is 0; Type 0 updates can be ignored for answering.
- If bipartite:
  - Build U by inserting all (S[i], i).
  - Sweep U once and for every adjacent pair with different colors, insert their gap into E.
  - Thereafter, process updates as above.

Answering queries

- If the graph is non-bipartite: print 0.
- Else bipartite:
  - If col[B] = col[C]: print 0 (they can meet at some city after an even X).
  - Else: print min(E).

Why this is correct

- Reachability in exactly X steps: Because staying still is disallowed but back-and-forth along an edge is allowed, any path can be lengthened by 2 freely, so for fixed start and parity, all nodes with the correct parity are attainable for sufficiently large X. In a non-bipartite connected graph, odd cycles allow changing parity as well, so for sufficiently large X all pairs (start, destination) are feasible with that same X (cofinite sets of feasible lengths intersect), hence both can meet at a same city, giving difference 0.
- In a bipartite graph, parity is the only restriction. If starts are in the same part, both can reach any city of that part with the same even X, so difference 0. If starts are in different parts, the two endpoints must lie one in A and one in B regardless of parity choice, and every such pair is attainable for some sufficiently large X, so the problem reduces to minimizing |a − b| over a ∈ A, b ∈ B.
- The boundary-argument shows that the global minimum across A×B equals the minimum gap between two consecutive elements of different colors in the merged sorted order. Updates only change adjacencies around the updated element’s position, so maintaining E captures the true minimum at all times.

Complexity

- Preprocessing (coloring + building U and E): O((N + M) + N log N).
- Each update: O(log N) for predecessor/successor lookups and a constant number of insert/erase operations in ordered sets.
- Each query: O(1) (after one color check); output 0 if same part, else the minimum of E.
- Memory: O(N + M) for the graph and O(N) for U and E.

Pseudocode outline

- Color and check bipartite:
  - BFS from any node, col[src] = 0; for each edge (u,v):
    - if col[v] = −1, set col[v] = 1 − col[u]
    - else if col[v] = col[u], mark nonBipartite = true
- If nonBipartite:
  - For each query:
    - if type = 0: update S but it won’t matter to answers
    - else: print 0
- Else:
  - U ← ordered set of (S[i], i)
  - E ← empty multiset
  - For consecutive pairs (x,y) in U:
    - if col[x] ≠ col[y], insert gap |S[y] − S[x]| keyed by pair (x,y) into E
  - For each query:
    - If type = 0:
      - UpdateValue(i, newS):
        - p ← predecessor of (S[i], i) in U (if any)
        - q ← successor of (S[i], i) in U (if any)
        - if p and col[p] ≠ col[i], erase boundary (p,i) from E
        - if q and col[q] ≠ col[i], erase boundary (i,q) from E
        - erase (S[i], i) from U
        - if p and q and col[p] ≠ col[q], insert boundary (p,q) into E
        - Find p’, q’ around insertion point of (newS, i) in U
        - if p’ and q’ and col[p’] ≠ col[q’], erase boundary (p’,q’) from E
        - insert (newS, i) into U
        - if p’ and col[p’] ≠ col[i], insert (p’, i) into E
        - if q’ and col[q’] ≠ col[i], insert (i, q’) into E
        - S[i] ← newS
    - Else (type = 1, query B, C):
      - if col[B] = col[C], print 0
      - else print min(E)

Sanity check on the sample

The sample graph is bipartite. Initially all S = 0, so min gap across parts is 0.
- After S1=10, S3=20, and parts A={1,3}, B={2,4,5,6}: min over A×B is min{|10−0|,|20−0|}=10, matching the sample.
- After S4=11, min over A×B is min{|10−11|,|20−11|, plus differences vs other zeros} = 1, again matching.
Queries with starts in the same part output 0; with different parts output min(E).

This approach fits comfortably in 2 seconds and 256 MB: total O((N+M) + (N+Q) log N) time and O(N+M) memory.