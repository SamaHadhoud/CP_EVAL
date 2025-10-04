Restatement

- We have N gears arranged as a tree. Each gear has an arrow angle in [0, 359], initially 0. If you rotate a gear x by α clockwise, every gear y connected to x (in the current on-board forest) rotates by α with alternating direction along the unique path: clockwise if dist(x, y) is even, counter-clockwise if dist(x, y) is odd.
- Three operations:
  1) remove x (it disappears and no longer transmits rotation),
  2) insert x back at the same absolute angle it had upon removal (no rotation is induced onto others),
  3) rotate x by α (0 ≤ α ≤ 359) while x is on-board.
- For every type-3 operation output the energy = α × (#gears that actually rotate in this operation) = α × (size of the current connected component of x within the on-board gears).
- Finally, output the sum of all final angles δu (each δu is kept modulo 360; for the final sum we sum these residues as integers in [0, 359]).

Two key observations

1) Bipartite parity controls the direction
Fix an arbitrary root of the tree and define parity p(u) = 0 for even depth and 1 for odd depth. Then, for any x and any y in the same current on-board component:
- dist(x, y) is even iff p(x) = p(y).
- Therefore, a rotation at x by α:
  - adds +α to every y with p(y) = p(x),
  - adds −α to every y with p(y) ≠ p(x).

This is independent of where x,y sit in the component beyond their global parity.

2) What we need to output
- For each type-3:
  - Energy = α × (size of x’s current on-board component).
- For the final sum of δ:
  - When we apply +α to some node with current angle a ∈ [0, 359], its new angle is (a + α) mod 360. The contribution to the total sum increases by:
    - α if a + α < 360,
    - α − 360 if a + α ≥ 360 (a “wrap” occurred).
  - When we apply −α, the increase is:
    - −α if a ≥ α,
    - 360 − α if a < α (an “underflow” wrap).
  - So to update the total sum correctly, for each operation we must know:
    - how many of the affected nodes (per side of the bipartition) will wrap/underflow relative to α,
    - and we must then add the exact change to the global total.
  - This makes it clear that merely knowing the component size or color counts is not enough to keep the final sum unless we also know the angle distribution modulo 360 among those nodes.

Main difficulty

- The on-board graph changes because vertices are removed/inserted; components split/merge arbitrarily. We need to:
  - quickly find the on-board component of x (for energy and to know who rotates),
  - quickly update the total sum of angles considering wrap-arounds,
  - do all of this under 1 s and 256 MB for N, Q up to 1e5.

Core idea: time blocking (sqrt-decomposition over time) + static components per block + small overlay graph

We break the Q operations into blocks of size B ≈ 700–1000 (tune in implementation).

Within one block we do three things:

- Freeze who can change: Let S be the set of gears that are toggled (removed or inserted) during the current block. Note |S| ≤ B.
- Build the “clean” static forest: At the start of the block, some gears are already off-board. Among the on-board gears, we remove S completely and compute the connected components of the remaining nodes; call them clean components. These clean components are static for the whole block (because only vertices in S will toggle).
- Build an overlay graph between S and clean components:
  - Nodes of this overlay: all s ∈ S plus all clean components.
  - Edges:
    - Between two s, if they are adjacent in the original tree.
    - Between s and a clean component C if s has at least one neighbor in C.
  - During the block, only on/off flags of s ∈ S change. Clean components remain “on” for the whole block (they were on-board at block start and are not toggled in this block). Connectivity for rotations is now connectivity in this small overlay graph subject to “only traverse S-nodes that are currently on”.

This overlay graph is small enough to explore per rotation—even in worst cases the number of S nodes is ≪ N, and you only connect to clean components adjacent to S nodes.

How we keep angles and update the final sum inside a block

We exploit two facts:
- Rotations inside a component add a uniform +α to one parity and a uniform −α to the other parity (Observation 1).
- Whether a node wraps when you add ±α is determined entirely by its current angle modulo 360 compared against a threshold (≥ 360 − α for +α or < α for −α).

For each clean component C:
- We split its nodes by parity color (p=0 and p=1).
- For each parity, we maintain:
  - A 360-bucket histogram of angles modulo 360 at the start of the block: histC[p][0..359] (counts of nodes with that residue).
  - A cyclic shift offset offC[p] that holds the net rotation applied to that parity in this component during the current block. The current residue of a node initially in bucket b is (b + offC[p]) mod 360.
- We also maintain counts cntC[p] (number of nodes in that parity in C).

With this, for a rotation at x by α:
- Identify parity px = p(x).
- Every visited clean component C receives:
  - For parity px: a +α shift. The number of wraps in parity px equals the count of buckets whose current residue ≥ 360 − α. Because we keep a fixed histogram in the initial frame and a running offset offC[px], this count is just a range sum over histC[px] shifted by offC[px]. We can precompute prefix sums of histC[px] to answer it in O(1).
  - For parity 1 − px: a −α shift. The number of underflows equals the count of buckets whose current residue < α; similarly computed in O(1).
- The contribution to the global final-sum accumulator T for this component is then:
  - T += α × cntC[px] − 360 × wraps_plus
  - T += −α × cntC[1−px] + 360 × wraps_minus
- After updating T, we update offC[px] = (offC[px] + α) mod 360 and offC[1−px] = (offC[1−px] − α) mod 360.

For nodes in S:
- We store their angles explicitly (a[s] in [0,359]) and their parity p(s).
- When a rotation visits an s that is currently on, we update T individually:
  - If p(s) = px: add +α
    - If a[s] + α ≥ 360: T += α − 360 and set a[s] = a[s] + α − 360
    - Else: T += α and a[s] += α
  - If p(s) ≠ px: add −α
    - If a[s] ≥ α: T += −α and a[s] −= α
    - Else: T += 360 − α and a[s] = a[s] − α + 360
- This is O(1) per visited S-node.

Energy:
- Energy for the rotation is α times the number of visited overlay nodes expanded to original nodes:
  - Count 1 per visited S-node that is on.
  - For each visited clean component C, add its size |C| (cntC[0] + cntC[1]).
- We print energy immediately.

How we find “visited” nodes per rotation

- Use BFS/DFS in the overlay graph starting from:
  - If x ∈ S: start at x (provided x is currently on; type-3 guarantees x is on).
  - Else: start at the clean component that contains x.
- Traverse edges:
  - From a clean component to adjacent S-nodes only if that S-node is currently on.
  - From an S-node to adjacent clean components always (clean components are always on in this block).
  - From an S-node to adjacent S-nodes only if the neighbor S-node is on.
- The visited set exactly corresponds to the on-board component of x in the original graph restricted to this block’s rules. That is because clean components represent maximal regions of on-board nodes that do not toggle in this block, and S-nodes are the only “gates” that can open/close connectivity during the block.

Block boundaries (rebuild)

At the end of a block:
- We must materialize the per-component offsets into per-node angles so that the next block starts with correct base angles:
  - For every clean component C and each parity p:
    - For each node u of parity p in C, set a[u] = (a[u] + offC[p]) mod 360.
  - Then reset offC[*] to 0 and discard histC; next block will rebuild from current per-node angles a[u].
- Nodes in S already have explicit a[u].

At the start of a new block:
- Determine S = nodes that will be toggled in this block (by looking ahead B operations).
- From the current alive set (after processing all previous blocks), form the clean forest by removing S.
- Compute clean components by a simple DFS/BFS over the remaining nodes.
- For every clean component C:
  - Count cntC[0], cntC[1].
  - Build histC[0][0..359], histC[1][0..359] from per-node a[u].
  - Compute prefix sums for both hist arrays to answer range sums in O(1).
  - Initialize offC[*] = 0.
- Build overlay adjacency:
  - For each s ∈ S, scan its neighbors in the original tree:
    - If neighbor ∈ S, add an S–S edge.
    - Else if neighbor ∈ a clean component C, link s ↔ C.
- Initialize isOn[s] from the state at the block’s beginning.

Correctness argument

- Connectivity within a block: Removing S-nodes from the graph partitions the on-board nodes into clean components; the only way to traverse between distinct clean components is through an S-node, and such traversal is possible if and only if the S-node is on. Also, S–S traversal corresponds to the original tree adjacency and is permitted only when both endpoints are on. This reproduces exactly the connectivity dynamics of the original operations restricted to the nodes that may toggle in this block.

- Direction and parity: For any rotation at x, the sign of rotation for y is determined by dist(x, y) parity, which equals p(x) XOR p(y), independent of the history. Hence, within any visited clean component C we can apply a uniform +α to parity p(x) and a uniform −α to the other parity.

- Sum updates and wrap counting:
  - Shifting by +α on a multiset of residues increases the total sum by α per element, except for those elements crossing 360, which reduce the increase by 360. Counting wraps as “how many residues ≥ 360 − α” is exactly correct. Similarly for −α we count underflows as “how many residues < α”.
  - Because we store histograms in the component’s initial frame and keep only a cyclic offset offC[p], we can answer “how many current residues fall into a given threshold range” in O(1) from the fixed histogram via the shifted index.
  - S-nodes are updated explicitly and contribute exact changes.

- Insert/remove operations: Removing a gear (type-1) and inserting it back (type-2) within a block only toggles isOn[s] for s ∈ S; no other node is rotated and our model does not change any angle at toggle times, matching the statement (“place back … arrow points to the same degree as when it was removed”). Between blocks we materialize offsets so angles entering the next block are correct.

Complexity

Let B be the block size (≈ 700–1000):

- Per block rebuild:
  - Materialize offsets: O(#clean nodes) ≤ O(N).
  - Build clean components: O(#clean nodes) ≤ O(N).
  - Build histograms and prefix sums: O(#clean nodes + 360 × #clean components). The number of clean components is at most (#removed at block start + |S| + 1). In practice, the 360 factor is OK because the number of clean components is limited by how many nodes are off-board at block start and by S (≤ B).
  - Build overlay adjacency: O(sum of degrees of nodes in S). This is usually much smaller than N and bounded by O(B × average degree). Even in worst shapes, this happens per block, not per operation.

- Per operation inside the block:
  - Type-1/2: O(1) (just toggle isOn).
  - Type-3:
    - BFS/DFS in overlay graph touching only currently reachable overlay nodes. In the worst case it can visit |S| S-nodes plus the clean components adjacent to them; empirically this is small compared to N, and the block approach keeps it manageable.
    - For each visited clean component, all work is O(1): computing wrap counts via histogram prefix sums and offset arithmetic.
    - For each visited S-node, O(1) updates.

Overall, with B ≈ √Q, this yields roughly O( (N + Q) √Q ) work with low constants, which fits in practice for N, Q ≤ 1e5 in a fast language with careful implementation and 256 MB memory. Memory use is dominated by histograms: each clean component keeps 2×360 counters; use 16-bit counters where possible to keep memory modest (components are disjoint; if there are many components then each is small, so 16-bit suffices for their bucket counts).

Pseudocode outline

Preprocessing:
- Root tree at 1; compute parity p[u].
- Set all angles a[u] = 0.
- currentOn[u] = true for all u.

Process in blocks of size B:
- Determine S = set of nodes that appear in a type-1 or type-2 in this block.
- Rebuild:
  - Materialize from previous block: for all nodes not in S and on-board, just keep their a[u] (offset already embedded at previous block end).
  - Build clean components on nodes that are on-board at block start and not in S.
  - For each component C:
    - cntC[p] = number of nodes with parity p in C.
    - histC[p][0..359] from a[u] for nodes in C with parity p.
    - build prefix sums prefC[p][0..359].
    - offC[p] = 0.
  - Build overlay graph among S and clean components.

- Initialize isOn[s] for s ∈ S according to currentOn[s].

- For each op in this block:
  - If op is “1 x”: isOn[x] = false; currentOn[x] = false.
  - If op is “2 x”: isOn[x] = true; currentOn[x] = true.
  - If op is “3 x α”:
    - Identify start overlay node start:
      - If x ∈ S: start = x.
      - Else: start = compID(x) of clean component containing x.
    - BFS over overlay using a queue:
      - Visit comp nodes freely; visit s nodes only if isOn[s] = true.
      - Maintain visited flags to avoid double counting.
      - Collect:
        - set VisComp of visited clean components,
        - set VisS of visited S nodes (these are necessarily on).
    - Compute energy:
      - energy = α × ( |VisS| + sum over C∈VisComp of (cntC[0] + cntC[1]) ).
      - Print energy.
    - Update total sum T:
      - Let px = p[x].
      - For each C in VisComp:
        - wrapsPlus = count of residues ≥ 360 − α in parity px under shift offC[px]; compute via prefC and offC.
        - wrapsMinus = count of residues < α in parity 1−px under shift offC[1−px].
        - T += α × cntC[px] − 360 × wrapsPlus
        - T += −α × cntC[1−px] + 360 × wrapsMinus
        - offC[px] = (offC[px] + α) mod 360
        - offC[1−px] = (offC[1−px] − α + 360) mod 360
      - For each s in VisS:
        - If p[s] = px: apply +α to a[s] with wrap handling; update T accordingly.
        - Else: apply −α similarly.
    - Continue.

- End of block:
  - For each C and p:
    - For each node u of parity p in C: a[u] = (a[u] + offC[p]) mod 360.
  - Clear structures; proceed to next block.

- After all blocks, print T as the final sum of δu.

Why this is correct

- The block decomposition preserves exact on/off dynamics of nodes that can change within the block (S), and treats the rest as “static clean components”; the overlay BFS exactly enumerates the on-board component of the rotated gear x at that moment.
- Within a rotation, directions depend only on parity, so per component and parity, a uniform shift of ±α is applied. Our offset+histogram representation produces the exact count of wraps/underflows, hence the exact change to the total sum T.
- Removing/inserting a gear does not rotate any other gear and does not change T; we model this as a pure toggle of isOn for S-nodes and leave clean components unchanged, exactly matching the statement that insertion is “always possible” without rotating neighbors.
- Between blocks we materialize offsets into per-node angles, preserving state across blocks.

Corner cases and notes

- α = 0: energy is 0; T unchanged. Our formulas handle this (wrap counts zero; offsets unchanged).
- x ∈ S: start BFS from the S node; the component may consist of S only or include clean components adjacent through on S-nodes.
- If many gears are off-board at block start, the number of clean components might be large, but each is small; storing 16-bit bucket counters per component keeps memory within 256 MB.
- Pick B to balance rebuild time and per-operation time. Values around 700–1000 work well in practice for 1e5 operations.

Summary of the approach

- Use a sqrt-decomposition over time to isolate the few nodes that toggle in the current block.
- Replace the big dynamic forest by a small overlay graph (S-nodes + static clean components).
- Within each clean component, maintain (per parity) a 360-bucket histogram and a cyclic offset. This lets us:
  - update energy in O(1) per visited overlay node,
  - update the global final sum T exactly (accounting for wrap-around) in O(1) per visited clean component and O(1) per visited S-node.
- Rebuild at block boundaries, materializing offsets to per-node angles.

This satisfies both outputs: per-rotation energy (by counting reached nodes) and the final sum T (by maintaining angle changes exactly with wrap handling), within time and memory limits.