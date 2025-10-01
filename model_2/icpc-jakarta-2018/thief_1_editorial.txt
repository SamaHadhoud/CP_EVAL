Restatement

- You are given a PIN length N and a set of M distinct digits A = {A1, …, AM} (subset of 0..9).
- When you type a long string S, the box tests every length-N substring of S (sliding window).
- Ayu wants to test at least K different PINs (distinct length-N substrings) on day one.
- Your job is to output the shortest possible string S over digits in A that contains at least K distinct length-N substrings. If multiple shortest strings exist, any is fine.

Key observations

1) How many windows can a string of length L produce?
- Exactly L − N + 1 length-N windows (counting duplicates).
- Therefore, to have at least K distinct windows, we need L − N + 1 ≥ K ⇒ L ≥ N + K − 1.
This is a hard lower bound on the length of S.

2) Can we always achieve this lower bound?
- Yes. Interpret the problem using the de Bruijn graph of order N over the alphabet A:
  - Vertices are all strings of length N−1 using digits from A.
  - For each length-N string x1x2...xN, there is a directed edge from x1...x(N−1) to x2...xN labeled xN.
  - Each edge corresponds to exactly one length-N substring, and a walk corresponds to a string S.
- The de Bruijn graph is Eulerian: every vertex has outdegree M and indegree M, and it is strongly connected.
- Hence it has an Eulerian cycle that uses all M^N edges once; its linearization gives a string of length M^N + N − 1 containing all length-N substrings exactly once (a de Bruijn sequence).
- Crucially, any prefix of length K of that Eulerian edge sequence yields:
  - a walk of K edges with no repeated edges,
  - which corresponds to a string of length N + K − 1 with exactly K distinct length-N substrings.

Therefore the optimal answer length is exactly N + K − 1, and such a string always exists for any K ≤ M^N.

How to construct a shortest string

We need a way to produce a trail (a path with no repeated edges) of length K in the de Bruijn graph, then write the corresponding string: initial (N−1)-prefix + K edge labels.

Two standard approaches work. Both run easily within constraints.

A. Hierholzer on the de Bruijn graph, early stop

- Concept:
  - Fix an order of digits in A (e.g., ascending).
  - Run Hierholzer’s algorithm to build an Eulerian tour, but stop as soon as you’ve output K edges.
  - The edges you output (in the order Hierholzer emits them) form a contiguous segment of the Eulerian cycle; prepend an initial vertex label (N−1 digits) to get the required string.

- Data you need:
  - The current node is the last N−1 digits of the current DFS path; maintain this “path string” P.
  - For each visited node v (i.e., for some (N−1)-string), store how many outgoing edges have been tried so far (an index in [0..M)).
  - You must be able to update the current node efficiently when you move forward (append a digit) or backtrack (pop a digit).
  - You do not need to materialize the entire graph; you only touch O(K) edges and O(K) nodes.

- Efficient state representation:
  - Keep P (current path) as a dynamic array of digits. The current node label is simply the last N−1 digits of P.
  - Use a rolling representation (e.g., a 64-bit or double-64-bit hash) of the last N−1 digits so you can map a node to its “next-outgoing-edge index” in O(1) via an unordered_map keyed by the hash. This avoids storing O(N) strings per node. Maintaining the hash as you append/pop from P is standard and O(1).
  - Since K ≤ 1e5 and M ≤ 10, the map stores at most O(K) entries.

- Pseudocode sketch:
  - Let D be the digits A sorted (any fixed order).
  - Start node v0 = the smallest digit repeated (N−1) times. Initialize P with v0.
  - Maintain:
    - next[v]: the next outgoing edge index for node v (0 initially).
    - ans: empty list to store the edge labels as they are produced by backtracking.
  - While ans.size < K:
    - v = hash(last N−1 digits of P)
    - If next[v] < M:
      - d = D[next[v]]; next[v]++
      - Append d to P (move forward one edge)
    - Else:
      - You must backtrack: pop the last digit d from P and append d to ans.
  - Output S = (initial N−1 digits of v0) + first K digits from ans. This has length N + K − 1 and K distinct windows.

- Complexity:
  - Time: O(N + K), since you process each of the K edges a constant number of times; M ≤ 10.
  - Memory: O(N + K), dominated by P, ans, and the per-node pointer map of size O(K).

B. Greedy “prefer-largest/lexicographically last unused PIN” (no backtracking)

There is a classic linear, no-backtracking construction for de Bruijn sequences: at each step, append the largest digit that creates a new length-N substring. This is known as the “prefer-one” (binary) or “prefer-maximum” (k-ary) algorithm. It can be stopped after K new substrings to meet our requirement.

- Idea:
  - Maintain the set Seen of length-N substrings already used.
  - Maintain the hash of the last N−1 digits of S.
  - Repeatedly try digits in a fixed order (e.g., descending); pick the first digit d for which the new N-length window (last N−1 suffix + d) is not in Seen.
  - Append d, insert that N-gram in Seen, and continue.
  - After K steps you have K distinct windows; length is N + K − 1.

- Why it works:
  - This greedy construction is exactly a walk that never repeats an edge in the de Bruijn graph.
  - It is a well-known fact that this greedy algorithm generates a de Bruijn sequence (i.e., traverses all M^N edges) if run to completion; equivalently, it will not get stuck before exhausting all edges.
  - Therefore, for any K ≤ M^N, it certainly won’t get stuck before producing K distinct windows.

- How to implement efficiently:
  - Let D be the digits in a fixed order (commonly descending order for the standard proof; any fixed order yields some de Bruijn sequence).
  - Start S with N−1 copies of some digit in A (e.g., the smallest) as the initial suffix.
  - Maintain:
    - h: rolling hash of the last N−1 digits of S.
    - Seen: hash set of window hashes of length N already used.
    - powB = B^(N−1) for your base B (e.g., B = 1315423911 or any 64-bit base; you can use double 64-bit hashes for negligible collision probability).
  - For step = 1..K:
    - For d in D:
      - w = h * B + d  // hash of the candidate N-gram
      - If w not in Seen:
        - Seen.insert(w)
        - Append d to S
        - Update h = w − firstSuffixDigit * powB  // in unsigned 64-bit arithmetic, this is “natural modulo 2^64”
        - Break
  - Output S.

- Complexity:
  - Each step tries at most M ≤ 10 digits, performs O(1) hash operations:
    - Time O(N + M·K) ≤ O(N + 10K)
    - Memory O(N + K) for S and Seen.

Why the solution is correct and optimal

- Optimal length: Any string with K distinct length-N substrings must have length at least N + K − 1. Our constructions produce a string of exactly that length. Hence they are optimal.

- Correctness (distinctness): Both constructions ensure each length-N window you create is new:
  - Approach A explicitly traverses distinct edges (no repetition) in the de Bruijn graph.
  - Approach B only appends a digit if the resulting window hasn’t appeared before.

- Existence/Continuation: The de Bruijn graph is Eulerian. Approach A is a prefix of Hierholzer’s Eulerian cycle construction; it cannot get stuck before M^N edges. Approach B is the classical greedy de Bruijn construction; it also cannot get stuck before M^N edges. Therefore either method can always produce the first K edges for any K ≤ M^N.

- Edge cases:
  - N = 1: The graph has a single node and one loop per digit. The optimal string has length K, consisting of K distinct digits from A. Both methods degenerate to “output any K distinct digits from A,” which is correct since K ≤ M.
  - Leading zeros are allowed; the string may start with 0 if 0 ∈ A.
  - The input guarantees the final string length ≤ 100000, so the memory/time bounds are safe.

Pseudocode outline (prefer-greedy, simplest to implement)

- Input N, M, K, and digits A; sort A; define D as A in descending order.
- If N = 1:
  - Output the first K digits of D in any order (or ascending) and stop.
- Else:
  - Initialize S with N−1 copies of the smallest digit in A.
  - Precompute powB = B^(N−1).
  - Compute h = hash of the current suffix (length N−1).
  - Seen = empty set
  - Repeat K times:
    - For d in D:
      - w = h * B + d
      - If w not in Seen:
        - Seen.insert(w)
        - Append d to S
        - Update h = w − firstSuffixDigit(S) * powB
        - Break
- Output S.

Notes on hashing and implementation details

- Use a 64-bit unsigned rolling hash; in practice, natural overflow mod 2^64 is fine. For extra safety, use two independent 64-bit hashes (store a pair in Seen).
- To update h efficiently, keep track of the first digit of the current length-(N−1) suffix; it is S[|S| − (N − 1)] before appending.
- Base B can be any large odd 64-bit constant, e.g., 1315423911; powB is then B^(N−1) under mod 2^64 arithmetic.
- The alphabet A consists of digits 0..9; you can use their values directly as “digits” in the hash.

Why the greedy rule must succeed (sketch)

- The greedy rule “always take the largest digit that makes a new N-gram” is equivalent to a depth-first traversal of the de Bruijn graph where edges are tried in a fixed order and never reused. The set of used edges is always a union of directed cycles and trails in an Eulerian graph.
- If the greedy process could get stuck at some vertex before using all edges, the remaining unused edges would form a nonempty Eulerian subgraph disconnected from the current end, which contradicts the known fact that trying edges in a fixed order with the “use it if unused” rule yields an Eulerian cycle (this is a classical de Bruijn construction). Therefore, the greedy process can continue until all edges are used; hence, it can certainly produce the first K ≤ M^N edges.

Putting it all together

- The shortest possible length is exactly N + K − 1.
- Build a trail of K edges in the order-N de Bruijn graph over A:
  - Either via Hierholzer with early stop,
  - Or via the no-backtracking greedy “prefer-largest-new-window” rule with a Seen set of N-grams.
- Prepend any (N−1)-length start (e.g., the smallest digit repeated N−1 times). Append the K chosen edge labels.
- The result uses only digits from A, has length N + K − 1, and contains at least K distinct PINs (in fact, exactly K).

These methods fit well within 1 second and 256 MB:
- K ≤ 1e5, M ≤ 10 ⇒ O(MK) ~ 1e6 operations.
- Memory O(|S| + K) ≤ 2e5 scale.