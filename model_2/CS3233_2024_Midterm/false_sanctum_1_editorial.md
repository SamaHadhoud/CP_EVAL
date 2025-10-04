Restatement

We have a mutable string S of length N. For any substring S[l, r], its density is defined by grouping consecutive equal characters (runs) and summing the square of each run’s length. For example, maakkkki splits into m | aa | kkkk | i and has density 1^2 + 2^2 + 4^2 + 1^2 = 22.

We must process Q operations:
- Type 1: set S[i] = c.
- Type 2: output the density of S[l, r].

Constraints are large (N, Q up to 2e5), so both updates and queries must be around O(log N).


Key observations

- The density of a segment equals the sum, over its runs of identical characters, of (run length)^2.
- If we concatenate two adjacent segments A and B, their densities simply add, except possibly at the boundary where the last character of A and the first character of B may be the same. If they are the same, the two boundary runs merge:
  - Originally counted: a^2 (A’s suffix run) + b^2 (B’s prefix run).
  - After concatenation: (a + b)^2.
  - Net change: (a + b)^2 − a^2 − b^2.
- Therefore, to combine two segments correctly, we only need:
  - The density inside each segment.
  - The last character and the length of the suffix run in the left segment.
  - The first character and the length of the prefix run in the right segment.

This strongly suggests a segment tree where each node stores just enough boundary information and the precomputed density for its interval.


Data kept per segment tree node

For an interval [L, R], store:
- len: length of the interval, R − L + 1.
- first: the first character in S[L, R].
- last: the last character in S[L, R].
- pref: length of the prefix run of character first within [L, R].
- suff: length of the suffix run of character last within [L, R].
- sum: the density of [L, R], i.e., sum of squared run lengths inside this interval.

For a single-character leaf:
- len = 1, first = last = S[i], pref = suff = 1, sum = 1.

All sums must use 64-bit integers, because the answer can be up to N^2 (≈ 4e10).


Merging two nodes (left A, right B)

We construct the parent node C = merge(A, B) as follows:
- C.len = A.len + B.len
- C.first = A.first
- C.last = B.last
- C.sum = A.sum + B.sum
- If A.last == B.first, then:
  - Let a = A.suff, b = B.pref.
  - Adjust sum to merge boundary runs:
    C.sum += (a + b)^2 − a^2 − b^2
- C.pref = A.pref
  - If A.pref == A.len and A.last == B.first, then C.pref = A.len + B.pref
- C.suff = B.suff
  - If B.suff == B.len and A.last == B.first, then C.suff = B.len + A.suff

Intuition:
- sum: Replace the two boundary squares by the single merged square when the boundary characters match.
- pref: The combined prefix spans all of A and also B’s prefix if and only if A is one single run and matches B’s first char.
- suff: Symmetric to pref.


Operations

- Build:
  - Build the segment tree bottom-up with the leaf initialization and the merge rule above. Time O(N).

- Point update (type 1: set S[i] = c):
  - Update the leaf at position i: first = last = c, pref = suff = 1, sum = 1.
  - Recompute all ancestors by merging. Time O(log N).
  - Optional micro-optimization: if S[i] is already c, you can skip updating.

- Range query (type 2: density of S[l, r]):
  - Query the segment tree for [l, r], returning a node with all six fields computed for that interval using the same merge logic while traversing.
  - The answer is the node.sum. Time O(log N).


Why this works (correctness)

- Base case: For a leaf, the density is 1, which matches the definition (one run of length 1).
- Inductive step: Assume for two disjoint adjacent segments A and B we have correct nodes. When forming C = [A|B]:
  - The only potential change in run structure when concatenating A and B is at the junction between A’s last character and B’s first character. All runs entirely inside A and entirely inside B remain unchanged.
  - If the boundary characters differ, runs do not merge; thus C.sum = A.sum + B.sum.
  - If the boundary characters are equal, exactly two runs merge into one; the change in density is replacing a^2 + b^2 by (a + b)^2, which is precisely the adjustment applied. No other runs are affected.
  - pref and suff definitions ensure that when the whole left or right side is a single run matching the boundary character, the prefix/suffix lengths extend across the boundary properly.
- By induction, all nodes compute the correct sum and boundary run data. A range query merges O(log N) nodes and thus yields the correct density for any substring.


Complexity and limits

- Build: O(N).
- Each update: O(log N).
- Each query: O(log N).
- Total over Q up to 2 × 10^5: well within 1 second in a compiled language.
- Memory: A typical segment tree with about 4N nodes; each node stores a few integers and chars. This easily fits within the given memory limit. Use 64-bit integers for sum.


Pseudocode sketch

- Node fields: len, first, last, pref, suff, sum (sum is 64-bit).
- merge(A, B):
  - res.len = A.len + B.len
  - res.first = A.first
  - res.last = B.last
  - res.sum = A.sum + B.sum
  - if A.last == B.first:
      res.sum += (A.suff + B.pref)^2 − A.suff^2 − B.pref^2
  - res.pref = A.pref
  - if A.pref == A.len and A.last == B.first:
      res.pref = A.len + B.pref
  - res.suff = B.suff
  - if B.suff == B.len and A.last == B.first:
      res.suff = B.len + A.suff
  - return res
- build(node, L, R):
  - if L == R: initialize leaf from S[L]
  - else:
      mid = (L + R) / 2
      build(leftChild, L, mid)
      build(rightChild, mid+1, R)
      tree[node] = merge(tree[leftChild], tree[rightChild])
- update(node, L, R, i, c):
  - if L == R: set leaf to c (len=1, first=last=c, pref=suff=1, sum=1)
  - else:
      mid = (L + R) / 2
      recurse into the child that contains i
      tree[node] = merge(tree[leftChild], tree[rightChild])
- query(node, L, R, ql, qr):
  - if [L, R] fully inside [ql, qr]: return tree[node]
  - if disjoint: return a neutral node that merges correctly (handle via standard segment tree range query technique by tracking whether a partial result exists)
  - else:
      leftRes = query(leftChild, L, mid, ql, qr) if overlaps
      rightRes = query(rightChild, mid+1, R, ql, qr) if overlaps
      combine present partial results with merge and return

Note: For the neutral element, a common practical approach is to keep a boolean that indicates “no node yet” and then set result = current node on first overlap; subsequently merge in others. This avoids having to invent a true neutral node.


Correct handling of all cases

- Single position ranges (l == r) correctly return 1.
- Ranges containing only one run (all equal chars) return (length)^2, because for such a node pref = suff = len and sum = len^2 by construction/merging.
- Merging across any boundary works correctly due to the explicit boundary character check and the precise sum adjustment.
- Point updates that split or merge runs are handled naturally because the update recomputes along the path and the merge function re-evaluates the only affected boundaries.
- Large answers fit in 64-bit integers.


Sanity check with the sample

S = aabcccacc
Query 1: 2 1 9
- Runs: aa | b | ccc | a | cc → 2^2 + 1^2 + 3^2 + 1^2 + 2^2 = 4 + 1 + 9 + 1 + 4 = 19

Update: 1 7 c → S becomes aabcc c ccc (positions 7 turns to c)
- Now substring 2..8 later will be computed by the segment tree; the logic merges runs on the fly.

Query 2: 2 2 8
- S[2..8] = abcccc c → runs: a | b | ccccc | c? Specifically: indices 2..8 = a b c c c a c c; runs: a | b | ccc | a | cc → 1 + 1 + 9 + 1 + 4 = 16? After update (i=7→c), indices 2..8 = a b c c c c c c → runs: a | b | cccccc → 1 + 1 + 36 = 38? Wait, careful:
  Original S: a a b c c c a c c
  After update i=7:=c: a a b c c c c c c
  Now S[2..8] = a b c c c c c c → runs: a(1), b(1), c(6) → 1 + 1 + 36 = 38, but sample says 27. The query in the sample is 2 2 8 before re-checking text:
  Sample Input:
    9 3
    aabcccacc
    2 1 9
    1 7 c
    2 2 8
  After changing position 7 to c, the full string is aabcc c c c: a a b c c c c c c
  However, the sample output says 27. The reason: the query asks density of S[2, 8] in the new string:
  positions 2..8 = a b c c c c c → 7 characters: a(1), b(1), c(5) → 1 + 1 + 25 = 27, matching the sample.

The segment tree returns exactly these values with the merging rule.

This approach is optimal and robust under the given constraints.