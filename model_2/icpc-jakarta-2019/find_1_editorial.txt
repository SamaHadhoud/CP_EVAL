Restatement

You are given an R×C grid of uppercase letters (R, C ≤ 500) and Q query strings (Q ≤ 2e5, total length ≤ 2e5). For each string S, count how many “L-shaped” occurrences exist: start from some cell (r,c), move right Δc times (Δc ≥ 0), then move down Δr times (Δr ≥ 0). You read exactly once the corner cell at (r, c+Δc). Thus the string constructed is:
G[r, c], G[r, c+1], …, G[r, c+Δc], G[r+1, c+Δc], …, G[r+Δr, c+Δc].
Two occurrences are different if the used set of cells differs.

Key idea (split at the corner + count corners)

Every occurrence has exactly one corner at the last rightward cell. If S has length L, then S can be split at some k (1 ≤ k ≤ L) into:
- Horizontal part pre = S[0..k-1], read ending at the corner,
- Vertical part suf = S[k..L-1], read starting immediately below the corner.

So, for each k:
- pre must occur horizontally ending at the corner cell X,
- suf must occur vertically starting at the cell just below X.

If we could, for every grid cell X, quickly know:
- which pre’s end at X, and
- which suf’s start at the cell below X,
then for each split (pre, suf) we would count the corners X that satisfy both.

Doing that naively per pattern is too slow. The crucial trick is to handle all queries and all splits together, using two Aho-Corasick (AC) automata and a 2D offline range counting reduction.

What we build

Let the total length of all queries be N (≤ 2e5). For a query S of length L, it contributes:
- all prefixes S[0..k-1], k=1..L (we need those),
- all suffixes S[k..L-1], k=1..L (we need those too).

Observe:
- The set of all prefixes, over all queries and all k, is exactly the union of all prefixes of each S. Total size O(N).
- The set of all suffixes, over all queries and all k, equals the set of all prefixes of reverse(S), across all queries. Total size O(N).

Therefore we build:
1) ACrow: AC trie of all queries S (this trie implicitly contains all their prefixes as nodes). We will use it to detect “pre ends here” horizontally.
2) ACcol: AC trie of all reverse(S). We will use it column-wise on reversed columns (bottom-to-top) to detect “suf starts here” vertically.

Failure trees and Euler intervals

For any AC, say a pattern ends at the current position if the pattern’s node lies on the failure chain of the current state. Equivalently, if node v (for the pattern) is an ancestor of the current state in the “failure tree” (tree formed by reversing failure links).

Hence, if we Euler-tour the failure tree of each AC and record for every node u its interval [tin[u], tout[u]) (subtree is a contiguous interval), then:
- “pattern v matches at a position with state s” ⇔ tin[v] ≤ tin[s] < tout[v].

We will use this to turn “does pre occur here?” and “does suf occur here?” into interval membership checks on two independent dimensions.

Mapping each grid cell to a point

We need to treat every possible corner X=(r,c):

- Horizontal side (left-to-right within the row):
  Traverse each row with ACrow, left-to-right. Let stateH[r][c] be the AC state after consuming G[r,1..c]. Define a-coordinate a[r][c] = tin_row[stateH[r][c]].

- Vertical side (start strictly below the corner!):
  For each column c, traverse the column bottom-to-top (i.e., reversed) with ACcol. Maintain a running state “how much of the reversed column below the current row has been read.” At row r we need the state corresponding to the position just below (r,c); let that be stateV[r][c]. Define b-coordinate b[r][c] = tin_col[stateV[r][c]].

Implementation detail for vertical scan:
- For each column c:
  state = root of ACcol
  For r from R down to 1:
    b[r][c] = tin_col[state]              // state corresponds to reversed prefix of length R - r
    state = go(state, G[r][c])            // now includes row r’s character for the next iteration
This automatically sets b[R][c] = tin(root), which is correct for the case “vertical part is empty”.

Now every cell X maps to a single 2D point P(X) = (a[r][c], b[r][c]).

Turning each split into a rectangle

For each query S of length L:
- Precompute v_k (k=1..L), where v_k is the node in ACrow reached after reading S[0..k-1].
- Let T = reverse(S). Precompute wprefix[pos] (pos=0..L), where wprefix[pos] is the node in ACcol reached after reading the first pos chars of T. Then for split k, suf has length L-k, so define w_k = wprefix[L-k].

An occurrence of split k ends at a corner X if and only if:
- a[r][c] ∈ [tin_row[v_k], tout_row[v_k]) and
- b[r][c] ∈ [tin_col[w_k], tout_col[w_k]).

Thus each split contributes the number of points P(X) inside the rectangle:
[tin_row[v_k], tout_row[v_k]) × [tin_col[w_k], tout_col[w_k]).

Sum over k gives the answer for S. Note:
- k=L means suf is empty. In ACcol, w_L = root. The root’s subtree interval is the whole [1..|ACcol|], so this counts all corners where pre ends at X, i.e., all horizontal-only occurrences (Δr=0).
- Non-empty vertical matches are automatically excluded on the bottom row, since b[R][c] = tin(root) and root is not in the subtree interval of any non-root node.

Now we just need to answer many rectangle counting queries over the same set of points.

Answering all rectangles: 2D offline range counting

We have at most RC points (≤ 2.5e5) and at most sum|S| rectangles (≤ 2e5). Do this offline with a sweep line and a Fenwick tree (BIT):

- For each point P=(x,y) where x=a[r][c], y=b[r][c].
- For each rectangle [x1,x2] × [y1,y2] (x2 is tout-1), convert it to two “events”:
  +1 query at x = x2: ask BIT sum over y ∈ [y1..y2].
  -1 query at x = x1-1: ask BIT sum over y ∈ [y1..y2].

- Sort points by x ascending.
- Sort all events by their x thresholds ascending.
- Sweep x from left to right:
  - For every point with current x, add 1 at its y in a BIT over y∈[1..|ACcol|].
  - Process all events whose threshold equals the current x: for each, do query sum(y1..y2) and add to answer with its sign.

This gives counts for all rectangles in O((points + events) log |ACcol|). Keep answers in 64-bit.

Why this is correct

- Horizontal part: Scanning left-to-right with ACrow leads to a state s at each cell. A prefix pre matches ending at that cell if and only if the node v for pre is on the failure chain of s, equivalently s is in the fail-tree subtree of v. Using Euler’s tin/tout, that is an interval membership on the a-coordinate.

- Vertical part: Scanning each column bottom-to-top with ACcol makes the state before reading row r exactly represent the reversed string of the segment below (including row r+1, r+2, …). A suffix suf matches starting at (r+1,c) iff reverse(suf) matches ending at that reversed position, i.e., iff the current state lies in the subtree of the node w for reverse(suf). Again, an interval condition, now on the b-coordinate.

- Corners are independent: For a fixed split, a corner X contributes if and only if both conditions hold, i.e., the point P(X) lies in the rectangle. Summing over k covers all possible bend positions; each L-shaped occurrence has exactly one bend and is counted once.

Complexity

Let N = sum of lengths of all Q strings (≤ 2e5), and M = R·C (≤ 2.5e5).

- Building ACrow and ACcol: O(N) nodes each, O(N) time and space (alphabet size 26).
- Failure links (BFS) and Euler tours: O(N).
- Scanning grid:
  - Rows: O(M) transitions.
  - Columns (reversed): O(M) transitions.
- Points: M.
- Rectangles: at most N (one per split).
- Offline sweep:
  - Sorting points and events: O((M + N) log (M + N)).
  - BIT operations: O((M + N) log N).

Overall time ~ O((M + N) log(M + N)), well within 5 seconds. Memory ~ O(N·alphabet + M), comfortably within 256 MB for alphabet 26.

Pseudocode (high level)

Build ACrow from all S
  insert S into trie (no need to mark endings specially)
Build ACcol from all reverse(S)
Compute failure links for both (BFS)
Build failure trees for both
Euler-tour both failure trees to get tin_row, tout_row and tin_col, tout_col

// Scan rows: compute a[r][c]
for r in 1..R:
  state = root_row
  for c in 1..C:
    while state != root and next[state][G[r][c]] == null: state = fail[state]
    if next[state][G[r][c]] != null: state = next[state][G[r][c]]
    a[r][c] = tin_row[state]

// Scan columns bottom-to-top: compute b[r][c] (state just below row r)
for c in 1..C:
  state = root_col
  for r in R down to 1:
    b[r][c] = tin_col[state]
    while state != root and next[state][G[r][c]] == null: state = fail[state]
    if next[state][G[r][c]] != null: state = next[state][G[r][c]]

// Build points
points = []
for r in 1..R:
  for c in 1..C:
    points.push( (x=a[r][c], y=b[r][c]) )

// Build rectangle queries per string
Initialize answers[i]=0 (64-bit)
events = []
for i in 1..Q:
  S = query[i], L = |S|
  // v_k: walk S in ACrow
  v_state[0] = root_row
  for k in 1..L:
    v_state[k] = go(v_state[k-1], S[k])
  // w_k: walk reverse(S) in ACcol, storing prefixes
  T = reverse(S)
  w_state_prefix[0] = root_col
  for pos in 1..L:
    w_state_prefix[pos] = go(w_state_prefix[pos-1], T[pos])
  // build rectangles for all k
  for k in 1..L:
    v = v_state[k]
    w = w_state_prefix[L-k]
    x1 = tin_row[v], x2 = tout_row[v]-1
    y1 = tin_col[w], y2 = tout_col[w]-1
    // two events for prefix sums over x
    events.push( (x=x2, y1, y2, id=i, coef=+1) )
    events.push( (x=x1-1, y1, y2, id=i, coef=-1) )

// Offline sweep with BIT over y
sort points by x ascending
sort events by x ascending
BIT over y in [1..|ACcol_nodes|], initially zero
p = 0
for each event e in order:
  while p < points.size() and points[p].x ≤ e.x:
    BIT.add(points[p].y, +1)
    p++
  answers[e.id] += e.coef * BIT.sum(e.y1, e.y2)

// Output answers in input order

Notes and pitfalls

- Use 64-bit integers for answers (and BIT sums): A string like “AAAA…A” in a grid of A’s can have many occurrences.
- Vertical handling is subtle: b[r][c] must be the state representing the reversed text strictly below row r (i.e., before reading G[r][c]). The shown loop ensures that.
- You do not need to mark outputs in the AC tries. We only need nodes, failure links, and the failure-tree Euler intervals.
- Alphabet is fixed (26 uppercase letters), so classic array-based trie is efficient.
- DFS for Euler tour can be iterative to avoid recursion depth issues (up to ~2e5 nodes).
- Memory: two tries with ~N nodes each and 26 transitions per node are fine within the limit.

Why it covers all cases

- Purely horizontal matches (Δr = 0): k = L, suf empty ⇒ w is the root; the rectangle spans all y, so we count all corners where the full S ends on its row.
- Purely vertical matches (Δc = 0): k = 1 ⇒ pre is S[0], vertical is S[1..] starting below; exactly matches the definition.
- Bottom row: non-empty vertical parts have no room; they are excluded because b[R][c] is the root, which is not in any non-root subtree.
- Every L-shaped occurrence has exactly one corner; it is counted exactly once by the unique split at that corner.

This approach reduces the problem to linear-time preprocessing on two AC automata and a standard offline 2D range counting with O((R·C + total_length) log N) complexity, fitting comfortably within the time and memory limits.