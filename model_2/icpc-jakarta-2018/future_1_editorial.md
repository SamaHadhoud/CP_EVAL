Restatement

You are given N strings S1, S2, …, SN (1 ≤ N ≤ 15, 1 ≤ |Si| ≤ 15). For each i you must choose a non-empty subsequence Ti of Si. These chosen names must be strictly increasing in lexicographic order:
T1 < T2 < … < TN,
and your goal is to maximize the total length |T1| + … + |TN|. If no such sequence exists, output -1.

Key observations

- The lexicographic order is a total order and is transitive. So insisting on T1 < T2 < … < TN automatically enforces “each child’s name is larger than all older siblings.”
- For each Si, there are at most 2|Si| − 1 non-empty subsequences (≤ 32767 for |Si| = 15). This is small enough to enumerate.
- If we sort all distinct subsequences of a string in lexicographic order, then the best predecessor for any current subsequence is the maximum DP value over all previous subsequences strictly smaller than it. This can be obtained with a prefix maximum + binary search or a two-pointer linear sweep.

High-level plan: layered DAG + DP

Think of a layered DAG with N layers. Layer i contains all distinct non-empty subsequences of Si, sorted lexicographically. We put a directed edge from any node u in layer i − 1 to any node v in layer i if u < v (lex order). Each node has weight equal to its length. We want the longest (maximum total weight) path that picks exactly one node per layer in increasing order.

Instead of explicitly building all edges (which would be too many), we use the sorted order to compress transitions:

- Let A = the sorted list of distinct subsequences of Si−1, and B = the sorted list of distinct subsequences of Si.
- Let dpA[k] = the best total length up to layer i−1 if we end at A[k].
- For each B[j], the valid predecessors are exactly those A[k] with A[k] < B[j]. If we precompute prefMaxA[k] = max(dpA[0..k]), then
  dpB[j] = |B[j]| + max{ dpA[k] | A[k] < B[j] } = |B[j]| + prefMaxA[upper_bound(A, B[j]) − 1],
  where upper_bound(A, B[j]) is the first index in A where A[idx] ≥ B[j] (equivalently, lower_bound for “≥” since we need strictly smaller).

Because A and B are sorted, we can compute all dpB in O(|A| + |B|) using a two-pointer sweep (or in O(|B| log |A|) with binary search). We only need to keep one previous layer in memory.

Generating distinct subsequences per layer

- Enumerate all masks from 1 to (1 << |Si|) − 1.
- Build the subsequence corresponding to the set bits of the mask.
- Sort the generated list and deduplicate to obtain all distinct non-empty subsequences in lexicographic order.

Deduplication is safe and beneficial: if two masks produce the same subsequence, their lengths are equal and their transition sets are identical, so we only need one copy.

DP details

- Layer 1: dp1[j] = |A1[j]| for each subsequence A1[j] of S1.
- For i = 2..N:
  - Let A = layer i−1 strings, dpA their values.
  - Let B = layer i strings.
  - Build prefMaxA where prefMaxA[k] = max(prefMaxA[k−1], dpA[k]).
  - For each B[j]:
    - Find pos = lower_bound(A, B[j]) // first index where A[pos] ≥ B[j].
    - If pos = 0, there is no valid predecessor (everything in A is ≥ B[j]); mark dpB[j] as invalid.
    - Else dpB[j] = |B[j]| + prefMaxA[pos − 1].
- After processing layer N, the answer is max over dpN; if all are invalid, answer is −1.

Pseudocode sketch

- buildLayer(S):
  - V = []
  - for mask in 1..(1<<|S|)−1:
      t = subsequence of S using set bits of mask in increasing position order
      V.push_back(t)
  - sort(V), remove duplicates
  - return V

- main DP:
  - A = buildLayer(S1)
  - dp = [ |A[j]| for all j ]
  - for i = 2..N:
      B = buildLayer(Si)
      pref = prefixMax(dp) aligned with A
      dp2 = array of size |B| filled with invalid (e.g., −inf)
      For each j in [0..|B|−1]:
          pos = lower_bound(A, B[j]) // first index with A[pos] ≥ B[j]
          if pos > 0 and pref[pos − 1] is valid:
              dp2[j] = pref[pos − 1] + |B[j]|
      A = B, dp = dp2
  - ans = max(dp); print ans if valid; else print −1

Why it’s correct

- The problem is equivalent to finding a maximum-weight path through N layers where each chosen node (string) must strictly increase in lex order from one layer to the next.
- Sorting each layer puts strings in total order, and the set of valid predecessors for any current string is exactly the prefix of the previous layer strictly less than it.
- The DP value dp_i[s] is defined as the optimal total length of an increasing chain ending at s in layer i. The recurrence
  dp_i[s] = |s| + max{ dp_{i−1}[t] | t < s }
  follows directly from optimal substructure of the longest path on this layered DAG.
- Using prefix maxima over the sorted previous layer computes the above max exactly.
- By induction over layers, the DP computes the true optimum. If no value is valid in the last layer, no chain of length N exists.

Complexity

- Distinct non-empty subsequences per layer: at most 2|Si| − 1 ≤ 32767.
- Building a layer: O(2|Si| · |Si|) to enumerate + O(Ki log Ki · L) to sort and deduplicate, with Ki ≤ 2|Si| − 1 and L ≤ 15 (string compare is O(L)).
- Transition i−1 → i:
  - Either O(Ki−1 + Ki) with a two-pointer sweep, or
  - O(Ki log Ki−1) using lower_bound per element.
- Memory: store at most two layers at a time. Even in the worst case this is well within 256 MB.

With N ≤ 15 and |Si| ≤ 15, the total number of generated subsequences across all layers is at most about 15 × 32767 ≈ 5×10^5, and string lengths are tiny (≤ 15), so the approach runs comfortably within 1 second.

Implementation tips and pitfalls

- Do not include the empty subsequence: names must be non-empty.
- Remove duplicate subsequences within each layer before DP.
- Ensure the string comparison follows the given definition (standard lex on uppercase strings already matches it, including the rule that a proper prefix is smaller).
- Use a sentinel “invalid” DP value (e.g., −1 or a very negative number) and ignore it in prefix maxima and transitions.
- You can unify base case by imagining a virtual layer 0 with a single empty string of DP = 0; because empty is smaller than any non-empty string, layer 1 then becomes dp1[s] = |s| automatically. But do not allow empty strings in real layers.

Examples

- For the sample with KARIM, PARBUDI, CHANDRA:
  - Build and sort subsequences per layer, DP as above. The algorithm finds a chain of total length 16 (e.g., ARIM < ARUDI < CHANDRA).
- For ZORO, ANDI:
  - Every subsequence of ANDI starts with A.. or is lex ≤ something starting with Z.. from ZORO, so no valid transition to the second layer exists; the DP remains invalid in the last layer, so the answer is −1.

This solution is simple, robust, and fits easily in both time and memory limits.