Restatement

- You have an array P[1..N] of species IDs (one Pokemon per zone).
- Two operations happen M times:
  1) Update: set P[A] = B.
  2) Query: for a route [A, B], Pichuu catches all Pokemon in that range. For any species whose count in the route exceeds K, the ranger confiscates all of them. The result of the query is the number of species whose frequency in [A, B] is between 1 and K (inclusive).
- After updates, all future queries must use the modified array.

Goal: For each query operation, output the number of species whose frequency in the given subarray is at most K.

Observations

- For a fixed subarray [L, R], the desired answer is:
  answer = count of species x with 1 ≤ freq[x] ≤ K in P[L..R].
- Updates are point assignments that change one position’s species.
- We need to support interleaved updates and range queries efficiently, with N, M up to 1e5.

A direct data structure that supports “count of distinct values with frequency ≤ K” under point updates is non-trivial. However, an offline approach using Mo’s algorithm extended with modifications (time dimension) works well and meets the constraints.

Core idea: Mo’s algorithm with updates (3D Mo)

We will process all queries offline and reorder them to minimize how much the current interval [L, R] and the update-time state need to change between consecutive queries.

- Space of movement dimensions:
  - L and R (array interval), as in standard Mo.
  - T (how many updates have been applied to the array so far).

At any step, we maintain:
- arr[i]: the species currently at position i after applying T updates.
- cnt[x]: how many times species x appears in the current [L, R].
- ans: number of species x with 1 ≤ cnt[x] ≤ K (this is exactly what the query asks).

How ans is maintained
- When including a position i into the current interval (moving R right or L left), we “add” arr[i]:
  - Increase cnt[arr[i]] by 1.
  - If cnt becomes 1, the species newly appears. It counts only if K ≥ 1.
  - If cnt becomes K+1, the species just exceeded K, so its whole contribution is confiscated: decrement ans.
- When excluding a position i (moving R left or L right), we “remove” arr[i]:
  - If cnt is 1, removing it makes it 0: if K ≥ 1, decrement ans.
  - If cnt is K+1, removing it makes it K: it becomes allowed again, increment ans.
  - Decrease cnt[arr[i]] by 1.

These two threshold checks (crossing 1 and crossing K+1) are the only times the species’ contribution toggles. This logic also handles K = 0 correctly:
- Then “1” is exactly K+1, so the two conditions cancel out and ans stays 0, as expected.

Handling updates while answering queries
- Read all operations once and separate them into:
  - updates: a list of (pos, oldValue, newValue) in chronological order.
  - queries: a list of (L, R, T, id), where T is the number of updates that occurred before this query in the input order.
- While reading, keep a working copy cur[] to know each update’s old value:
  - When you see “set P[a] = b”, record old = cur[a], then set cur[a] = b, and append (a, old, b) to updates.
  - When you see a query, append (L, R, T=current number of updates seen so far, id).
- After parsing, reset arr = original P.

During processing:
- Maintain current pointers L, R and current applied-time T:
  - To move T forward (apply next update u = (pos, old, new)):
    - If pos ∈ [L, R], first remove the occurrence at pos (species old), then set arr[pos] = new, then add pos (species new).
    - Else, just set arr[pos] = new.
    - Increment T.
  - To move T backward (undo last update u = (pos, old, new)):
    - If pos ∈ [L, R], remove pos (species new), set arr[pos] = old, add pos (species old).
    - Else, set arr[pos] = old.
    - Decrement T.
- Move L and R as in standard Mo, adding/removing endpoints as needed.

Query ordering (sorting)
- Use block decomposition on three dimensions: L, R, T.
- Let B ≈ N^(2/3). Then sort queries by:
  - primary key: L // B,
  - secondary: R // B,
  - tertiary: T (in alternating order for parity of the first two blocks to reduce movement; or keep all ascending for simplicity).
- Alternatively, a Hilbert-order on (L, R, T) often yields smaller constant factors. Either is fine; N, M ≤ 1e5 fits comfortably with N^(2/3).

Pseudocode outline

Parse:
- Read N, M, K
- Read P[1..N]
- cur = P (copy)
- updates = []
- queries = []
- t = 0
- For i in 1..M:
  - read q, a, b
  - if q == 1:
    - t++
    - updates.append({pos=a, old=cur[a], new=b})
    - cur[a] = b
  - else:
    - queries.append({l=a, r=b, time=t, id=nextID})

Prepare to answer:
- Reset arr = P
- cnt[x] = 0 for all species x
- ans = 0
- L = 1, R = 0, T = 0
- Sort queries by 3D Mo order with block size B ≈ N^(2/3)

Helper functions:
- add(i):
  - x = arr[i]
  - cnt[x]++
  - if cnt[x] == 1 and K >= 1: ans++
  - if cnt[x] == K+1: ans--
- remove(i):
  - x = arr[i]
  - if cnt[x] == 1 and K >= 1: ans--
  - if cnt[x] == K+1: ans++
  - cnt[x]--
- applyUpdate(u): // move T -> T+1
  - pos = u.pos, old = u.old, new = u.new
  - if L <= pos <= R: remove(pos)
  - arr[pos] = new
  - if L <= pos <= R: add(pos)
- undoUpdate(u): // move T -> T-1
  - pos = u.pos, old = u.old, new = u.new
  - if L <= pos <= R: remove(pos)
  - arr[pos] = old
  - if L <= pos <= R: add(pos)

Process queries:
- For each query q in sorted order:
  - while T < q.time: applyUpdate(updates[T+1]); T++
  - while T > q.time: undoUpdate(updates[T]); T--
  - while R < q.r: R++; add(R)
  - while L > q.l: L--; add(L)
  - while R > q.r: remove(R); R--
  - while L < q.l: remove(L); L++
  - ansFor[q.id] = ans

Output answers in id order.

Complexity

- Sorting queries: O(Q log Q), dominated by processing step.
- Processing movements:
  - With block size B ≈ N^(2/3), the total number of pointer/time movements across all queries is O((N + Q) · N^(2/3)). Each movement is O(1).
  - For N, M ≤ 1e5, N^(2/3) ≈ 464, so the total is on the order of a few times 10^7 primitive operations, which is fine in optimized languages within 2 seconds.
- Memory:
  - Arrays of size O(N + M): arr, cnt, updates, queries; well within 1 GB.

Correctness argument

- Invariant: For the current state (arr after exactly T updates applied and current interval [L, R]), cnt[x] equals the count of species x in arr[L..R], and ans equals the number of species with 1 ≤ cnt[x] ≤ K.
- The only times a species’ eligibility changes is when its count crosses 0↔1 or K↔K+1. The add/remove routines adjust ans exactly at these thresholds, keeping the invariant.
- Moving L and R only adds/removes individual positions; moving T only changes one array position; in both cases, we update cnt and ans consistently with the invariant.
- Because each query (L, R, T) is processed exactly at the array state after T updates, the recorded ans equals the desired number for that query.

Edge cases and notes

- K = 0: Every non-empty interval results in 0; the threshold logic above yields ans = 0 automatically.
- K ≥ length of the interval: The answer is simply the number of distinct species in the interval; the algorithm also handles this naturally.
- If an update sets a position to the same species as before, it has no effect. You can skip it or keep it; the logic remains correct either way.
- Species IDs are bounded by N, so you can allocate cnt of size N+1; coordinate compression is optional.

Why this approach fits the limits

- 3D Mo’s algorithm is designed exactly for offline range queries with point updates. It avoids heavy data structures, relies on simple O(1) add/remove/update operations, and scales to N, M ≈ 1e5 within 2 seconds in typical contest environments.
- Memory is linear in input size.

Summary

- Reformulate the query as counting species with frequency in [1..K] in a subarray.
- Solve offline using Mo’s algorithm with a time dimension:
  - Timestamp each query by the number of prior updates.
  - Sort queries in 3D Mo order (L, R, T).
  - Maintain counts per species and a running answer that toggles only when a species’ count crosses 1 or K+1.
- This yields correct answers for all queries, including after updates, within the required time and memory limits.