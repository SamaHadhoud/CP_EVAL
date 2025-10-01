Restatement

We have an infinite line of icebergs labeled 1, 2, 3, … . Each recorded day i contributes one Piplup trying to claim iceberg Ai, and if it’s already occupied he moves right to the first free iceberg ≥ Ai. Over a range of days [L, R], this produces a set S of occupied icebergs (one distinct iceberg per day). A group of X new Piplups then leaves for iceberg 1 (one at a time with the same “go to first free ≥ start” rule), and we want the furthest iceberg any of them reaches. Equivalently, we want the X-th smallest positive integer not in S.

Additionally, records can be updated: a single A[P] changes to a new value.

Constraints to exploit
- N up to 1e6 (large array).
- M up to 5e3 (few total operations).
- All values Ai and X in queries are small: ≤ 500. This is the key.

Key ideas

1) The final set S depends only on the multiset of starting points, not on the order
The insertion rule “place each item at the smallest free ≥ its starting point” admits an equivalent left-to-right scan:

- For each position p = 1, 2, 3, … keep a backlog b of “waiting” Piplups that have already started but not yet placed.
- At position p, first add arrivals: b += c[p], where c[p] is the number of Piplups starting exactly at p.
- If b > 0, place one here (position p is occupied) and set b = b − 1. Otherwise, position p is empty.

This faithfully reproduces the greedy placement because at position p you can place one Piplup if and only if there exists at least one Piplup whose starting position is ≤ p and who hasn’t been placed yet; if so, the earliest free place for one of them is exactly p.

Consequently, after we aggregate counts c[j] = number of days in [L, R] with A = j, the occupancy pattern is uniquely determined and does not depend on order.

2) Only the first MAXA positions can receive new arrivals
Let MAXA be the maximum possible Ai, here MAXA = 500. Then:
- c[p] = 0 for p > MAXA, so after p > MAXA there are no new arrivals; the backlog simply drains by 1 per step.
- Therefore, the occupancy pattern is:
  - Decide for positions 1..MAXA by the recurrence b ← b + c[p]; if b > 0 then occupy and b ← b − 1 else p is empty.
  - Then positions MAXA+1..MAXA+b are all occupied (continuous tail), and all positions after that are empty forever.

This structure is crucial: once we know the number of occupied positions in [1..MAXA] and the final backlog b after MAXA, we know everything.

3) From occupancy to the X-th missing (the query answer)
Let:
- occPrefix = number of occupied among positions 1..MAXA.
- holes = MAXA − occPrefix = number of empty positions among 1..MAXA.
- tail = backlog after processing up to MAXA (the number of consecutive occupied positions immediately after MAXA).

Then the missing numbers are:
- The holes inside [1..MAXA], at the exact empty p’s found by the scan.
- Then everything after MAXA + tail is missing.

Therefore:
- If X ≤ holes: the answer is the position of the X-th empty among 1..MAXA; we can find it by the same scan.
- Else: answer = MAXA + tail + (X − holes).

That eliminates any need for binary search on positions; a single O(MAXA) scan per query suffices once we have c[1..MAXA].

4) How to get c[1..MAXA] quickly for a window [L, R] with updates
We must support:
- Range count queries: for each value v in 1..MAXA, count how many indices i in [L, R] have A[i] = v.
- Point updates: change A[P] from old to new.

A practical way given M is small (≤ 5000) and MAXA is tiny (≤ 500):
- For each value v in 1..MAXA, maintain a dynamic ordered set (balanced BST with order statistics) of positions where A[i] = v.
- Range count for value v is rank_v(R+1) − rank_v(L).
- Update: remove P from the set of old A[P], insert P into the set of new A[P], and update A[P].

This yields:
- Per query: build c[v] for v = 1..MAXA using two rank queries per v, total O(MAXA log N).
- Then simulate the O(MAXA) placement scan once to get holes, tail and (if needed) the exact position of the X-th hole within [1..MAXA].
- Per update: two balanced-tree operations, O(log N) each.

Algorithm in detail

Preprocessing
- Read N, M and the array A[1..N].
- For each v in 1..MAXA (MAXA=500), build an ordered set S[v] storing all positions i with A[i] = v.

Update operation (0 P X)
- Let old = A[P].
- If old == X: do nothing.
- Else: erase P from S[old], insert P into S[X], set A[P] ← X.

Query operation (1 L R X)
- Build counts c[1..MAXA]:
  - For each v in 1..MAXA:
    - c[v] = number of P in S[v] with L ≤ P ≤ R
    - This is c[v] = rank_v(R+1) − rank_v(L).
- Run the placement scan over positions p = 1..MAXA:
  - b = 0, holes = 0
  - For p from 1 to MAXA:
    - b += c[p]
    - If b > 0:
      - b -= 1  // position p occupied
    - Else:
      - holes += 1  // position p is empty
      - If holes == X: output p and stop
- If we didn’t output inside the loop:
  - tail = b  // contiguous occupied tail after MAXA
  - rem = X − holes
  - Answer = MAXA + tail + rem
  - Output this number.

Why it works

- The equivalence between the greedy insertions and the left-to-right backlog process is the core invariant: at each position p, after adding arrivals c[p], if any item with start ≤ p remains unplaced (backlog > 0), the earliest free place is exactly p, so we must occupy p and reduce the backlog.
- After MAXA, c[p] = 0 so the backlog drains deterministically: we occupy exactly the next tail positions MAXA+1 .. MAXA+tail.
- The X-th missing is either within the holes of the first MAXA positions, or beyond the continuous occupied tail; hence the closed form MAXA + tail + (X − holes) in the latter case.

Complexity and limits

- MAXA = 500, fixed.
- Preprocessing: O(N log N) to build the sets (insert N positions total).
- Each update: O(log N) (one erase, one insert).
- Each query:
  - O(MAXA log N) to compute c[v] across all values.
  - O(MAXA) to run the scan and produce the answer.
  - Total per query about 500 log N + 500 operations; with N up to 1e6, log N ~ 20, so roughly ~10k primitive operations per query.
- With M ≤ 5000, this comfortably fits in 2 seconds and 512 MB.

Notes and edge cases

- The approach relies on the given constraint that Ai, X ≤ 500. If Ai were unbounded, we would need a different structure.
- If any Ai could be 0, you can extend the scan to start at p = 0 and then continue at 1; or shift indices by +1 so that all starts are ≥ 1 and the logic remains identical.
- The upper bound for the answer is trivially (R − L + 1) + X, but with our direct formula no binary search is needed.

Pseudocode sketch

Data structures:
- Ordered sets S[1..MAXA] of integers (positions)
- Array A[1..N]

Update(P, X):
- old = A[P]
- if old == X: return
- S[old].erase(P)
- S[X].insert(P)
- A[P] = X

CountInRange(v, L, R):
- return rank_in_Sv(R+1) − rank_in_Sv(L)

Query(L, R, X):
- for v in 1..MAXA:
    c[v] = CountInRange(v, L, R)
- b = 0; holes = 0
- for p in 1..MAXA:
    b += c[p]
    if b > 0:
        b -= 1
    else:
        holes += 1
        if holes == X:
            return p
- // not found within 1..MAXA
- tail = b
- rem = X − holes
- return MAXA + tail + rem

Worked example (sample)

Initial A = [1, 3, 2, 4, 5]
Query 1: L=2, R=4, X=2
- c[2]=1, c[3]=1, c[4]=1 (others 0)
- Scan:
  p=1: b=0 -> hole #1 at 1
  p=2: b=1 -> occupy, b=0
  p=3: b=1 -> occupy, b=0
  p=4: b=1 -> occupy, b=0
  p=5: b=0 -> hole #2 at 5 → answer 5

Update: P=2, X=1 ⇒ A=[1,1,2,4,5]

Query 2: L=1, R=3, X=2
- c[1]=2, c[2]=1
- Scan:
  p=1: b=2 -> occupy, b=1
  p=2: b=2 -> occupy, b=1
  p=3: b=1 -> occupy, b=0
  p=4: b=0 -> hole #1
  p=5: b=0 -> hole #2 → answer 5

Query 3: L=1, R=5, X=10
- c[1]=1, c[2]=1, c[3]=1, c[4]=1, c[5]=1
- Scan p=1..5: occupy all, b=0; holes in 1..MAXA begin at 6, so hole #10 is 15 → answer 15

This matches the sample output.

Conclusion

The problem becomes easy once we:
- Aggregate by starting positions (counts c[1..MAXA]).
- Use the left-to-right backlog model to compute occupancy, which depends only on c and not on order.
- Exploit that Ai ≤ 500 to limit the “interesting” region to the first 500 positions plus a contiguous tail.
With an ordered set per value to count occurrences in any subarray and support updates, we achieve an efficient and clean solution within the time and memory limits.