Restatement

- There are n cats, numbered by height 1..n (height i for cat i).
- Some m unordered pairs (x, y) are “dislike” edges (mutual).
- A line is maintained: a dynamic set of cats, always kept sorted by height.
- We process q operations: insert a cat into the line or remove a cat.
- After each operation, all cats currently in the line each have a discontentment that depends only on the two cats directly adjacent to them in the sorted order:
  - 3233 if they dislike both neighbors,
  - 323 if they dislike only the one in front (smaller),
  - 32 if they dislike only the one behind (larger),
  - 3 otherwise.
- Output the sum of discontentment over the cats in the line, after every operation.

Constraints: n up to 1e9 (so we cannot hold arrays indexed by cat id), m up to 1e5, q up to 3e5, time limit 3s.


Key ideas

1) Only adjacency matters
- The line is always sorted. A cat’s discontentment depends only on the two adjacent cats in the line (its predecessor and successor).
- After inserting/removing one cat, only a constant number of adjacencies change: at most the predecessor and successor of the inserted/removed cat, plus the inserted/removed cat itself. Therefore, the total sum changes locally.

2) Dynamic ordered set
- Maintain the current line as an ordered set (balanced BST). It supports:
  - insert(z), erase(z),
  - predecessor(z), successor(z).
- We never need arrays of size n; the line’s size is at most q.

3) Fast “dislike” lookup
- We need to answer “do cats a and b dislike each other?” for pairs that are adjacent in the line. Prestore the m pairs in a hash set keyed by the ordered pair (min(a,b), max(a,b)). Then dislike queries are O(1) expected time.

4) Two equivalent ways to update the answer
There are two clean approaches. Either one easily fits time/memory limits.

A. Local recomputation (most straightforward to implement)
- Define value(x) as the discontentment of cat x using only its current predecessor and successor in the set.
- On insertion of z:
  - Let p = predecessor(z), s = successor(z) in the current set.
  - Subtract value(p) and value(s) (if they exist) from the running total (these use the old neighbors).
  - Insert z.
  - Add value(z) (using neighbors p and s in the new set).
  - Add value(p) and value(s) again (using their new neighbors).
- On deletion of z:
  - Let p = predecessor(z), s = successor(z).
  - Subtract value(z), value(p), value(s).
  - Erase z.
  - Add back value(p) and value(s) with their new neighbors.
- The value(x) function:
  - Let F = (predecessor exists and dislike(predecessor, x) is true).
  - Let B = (successor exists and dislike(x, successor) is true).
  - Return 3233 if F and B; 323 if F; 32 if B; else 3.

B. Aggregate formula (a nice algebraic simplification)
- For any cat, let F = 1 iff they dislike their front neighbor (predecessor), and B = 1 iff they dislike their back neighbor (successor). Then
  discontentment = 3 + 320·F + 29·B + 2881·F·B.
- Summing over all cats in the line:
  total = 3·S + 349·E + 2881·C,
  where
  - S = size of the line,
  - E = number of current adjacent pairs that are dislike edges (each such edge contributes once to “front dislikes” and once to “back dislikes”, hence 320 + 29 = 349),
  - C = number of cats that dislike both neighbors (i.e., are in the middle of two dislike adjacencies simultaneously).
- Maintain S, E, C and update them locally on each insertion/removal:
  - Let p = predecessor(z), s = successor(z).
  - For insertion:
    - E += [p-z is dislike] + [z-s is dislike] − [p-s is dislike].
    - C gains:
      - +1 if both [p-z] and [z-s] are dislike (this is z’s contribution),
      - For p: its “both” status changes because its back neighbor changes from s to z. Let prev2 = predecessor(p):
        delta_p = ([prev2-p] & [p-z]) − ([prev2-p] & [p-s]).
      - For s: its “both” status changes because its front neighbor changes from p to z. Let next2 = successor(s):
        delta_s = ([z-s] & [s-next2]) − ([p-s] & [s-next2]).
    - S += 1.
  - For deletion:
    - E += −[p-z] −[z-s] + [p-s].
    - C loses:
      - −1 if both [p-z] and [z-s] were dislike (removing z’s contribution),
      - For p: back neighbor changes from z to s:
        delta_p = ([prev2-p] & [p-s]) − ([prev2-p] & [p-z]).
      - For s: front neighbor changes from z to p:
        delta_s = ([p-s] & [s-next2]) − ([z-s] & [s-next2]).
    - S −= 1.
  - Finally output total = 3·S + 349·E + 2881·C.
- Here [a-b] is 1 if a and b exist and dislike each other, else 0. Missing neighbors make the corresponding term 0.
- Both approaches need O(log |line|) to find predecessor/successor, and O(1) to check dislike in the hash set.


Why this works

- After an insertion/removal, only adjacencies crossing the location of the changed cat can change. Therefore, only a constant number of cats can see their discontentment change (the inserted/removed one and its two neighbors).
- Approach A recomputes exactly those few cats’ contributions and updates the total.
- Approach B derives an identity that expresses the sum via three global counters (size, number of dislike-adjacent edges, number of “both-sides hated” cats) and updates those counters locally. The identity follows from: for a boolean pair (F, B), the four prescribed values 3, 32, 323, 3233 are exactly
  3 + 320·F + 29·B + 2881·F·B.
- Endpoints are handled naturally: if a neighbor doesn’t exist, the corresponding flag F or B is false.


Pseudocode sketch (Approach A)

Data:
- S: ordered set of active cats.
- H: hash set of dislike pairs, stored as (min(a,b), max(a,b)).
- total: 64-bit running sum.

Helpers:
- dislike(a, b):
  if a == null or b == null: return false
  if a > b: swap(a, b)
  return (a, b) in H
- value(x):
  p = predecessor of x in S (or null)
  s = successor of x in S (or null)
  F = dislike(p, x)
  B = dislike(x, s)
  if F and B: return 3233
  else if F: return 323
  else if B: return 32
  else: return 3

Process queries:
- For each operation (d, z):
  if d == 1: // insert z
    p = predecessor of z in S
    s = successor of z in S
    if p != null: total -= value(p)   // old neighbors
    if s != null: total -= value(s)
    insert z into S
    total += value(z)                 // new
    if p != null: total += value(p)
    if s != null: total += value(s)
    print total
  else: // d == 2, erase z
    p = predecessor of z in S
    s = successor of z in S
    total -= value(z)
    if p != null: total -= value(p)
    if s != null: total -= value(s)
    erase z from S
    if p != null: total += value(p)
    if s != null: total += value(s)
    print total

Notes:
- Each value(.) call performs two neighbor lookups; that’s fine. If you want, you can avoid some lookups by reusing already known neighbors p and s where appropriate, but it’s not necessary.
- Use 64-bit integers for the running sum to be safe (although the maximum possible sum is ≤ 3233 · q ≤ 9.699e8).


Complexity analysis

- Building H: O(m) time and memory.
- Each operation does O(log |S|) ordered-set operations (predecessor/successor/insert/erase) and O(1) dislike lookups for a constant number of cats. Thus overall O(q log q).
- Memory: O(m + |S|) ≤ O(m + q) ≤ 4e5 to 5e5 elements; well within 1024 MB.


Correctness argument

- At any time, each cat’s discontentment depends only on its immediate neighbors in the sorted line. When we insert or delete a cat z, the only cats whose neighbors can change are z itself and the two cats adjacent to its insertion/deletion spot (the predecessor and successor before/after the operation). All other cats keep the same neighbors and therefore the same discontentment.
- Approach A explicitly removes the old contributions of exactly those cats and adds their new contributions computed from the new neighbors. Hence the maintained sum after each operation is exactly the true sum.
- Approach B uses an equivalent global decomposition of the sum into three counters S, E, C, all of which are affected only locally by the same three cats/adjacencies. The local update formulas for E and C correct exactly the edges and nodes that changed; therefore total = 3·S + 349·E + 2881·C remains equal to the true sum.


Implementation tips

- Since n can be up to 1e9, never allocate arrays indexed by cat id.
- Store dislike pairs in a hash set keyed by a 64-bit encoding of (min(a,b), max(a,b)) or by a hash of pair<int,int>.
- Ordered set can be a balanced BST supporting predecessor/successor; operations are O(log |S|).
- Be careful to always compute “old” values before mutating S, and “new” values after mutating S.
- Endpoints (no predecessor or no successor) contribute 3 unless they dislike their single neighbor, in which case it’s 323 (if they dislike the front) or 32 (if they dislike the back).


Sanity check with the sample

- Start with empty S: sum = 0.
- After inserting 1: S={1}, sum=3.
- After inserting 3: S={1,3}, dislike(1,3)=true, sum=355.
- After inserting 5: S={1,3,5}, both edges disliked, middle (3) dislikes both neighbors, sum=3588.
- … and so on, matching the sample outputs.