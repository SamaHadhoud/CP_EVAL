Restating the problem
We are given N distinct country names S1, S2, …, SN (uppercase A–Z), and an integer K. We must assign each country a code Ci (a string of length K) such that:
- Ci is a subsequence of Si,
- Ci[1] equals the first letter of Si,
- and the codes preserve the lexicographic order: for any i, j, Si < Sj if and only if Ci < Cj.

We must either output one valid assignment or report that it is impossible.

Key observations
- Sort the countries by name: S(1) < S(2) < … < S(N). The “iff” condition is equivalent to requiring C(1) < C(2) < … < C(N) strictly. (Si are all different.)
- For each sorted country, we must pick a code that is strictly larger than the previously chosen code, and still a valid subsequence with the same first letter. If we always pick the lexicographically smallest feasible code for each country, this never hurts the future (smaller lower bound is easier for the next country), so a greedy strategy works: at step i, choose the smallest valid Ci > C(i−1). If at some step there is no valid choice, then no solution exists.

This reduces the problem to the subroutine:
Given a name S and a lower bound P (a K-length string), find the lexicographically smallest K-length subsequence C of S that starts with S[1] and is strictly greater than P. For the first country, P is “no bound”; we just need the lexicographically smallest valid code.

Efficient subsequence navigation: next-occurrence table
To quickly navigate subsequences, precompute for the current string S (length m) the array next[pos][ch]:
- next[pos][ch] = the smallest index ≥ pos where S[index] = ch (or “infinite” if none).
- Build it in O(26·m) by scanning S from right to left while maintaining last-seen positions for each letter.

This lets us:
- Check whether there is any occurrence of a letter after a given position in O(1).
- Jump to the next occurrence index in O(1).
- Check feasibility to finish: after placing a character at index idx, we can complete the remaining r letters iff m − idx ≥ r (enough characters remain).

Greedy construction without a lower bound (first country or when first letters already force Ci > P)
We want the lexicographically smallest K-length subsequence starting with S[1] (denote f = S[1]):

- If m < K, impossible.
- Take the first code letter at position 1 (the earliest f; this always maximizes room for later picks).
- For j = 2..K:
  - Among letters 'A'..'Z', pick the smallest ch such that next[pos+1][ch] = idx exists and m − idx ≥ K − j.
  - Set pos = idx and append ch.

This builds the smallest possible code for S respecting the constraints.

Handling the lower bound P
Let P be the previous code of length K. Let f = S[1]:

- If f < P[1]: impossible. Any code for S must start with f, hence is lexicographically smaller than P immediately.
- If f > P[1]: any valid code for S is already > P; choose the greedy smallest code (as above).
- If f = P[1]: we must find the smallest code strictly greater than P but still starting with f. The only way is to match the longest possible prefix of P and increase at the first position t ≥ 2.

How to find the smallest code strictly greater than P (when f = P[1])
1) Compute earliest positions of P’s prefix in S:
   - pos[0] = 0.
   - For j from 1 to K: pos[j] = next[pos[j−1] + 1][P[j]] (or ∞ if not presente). These are the earliest indices where the prefix P[1..j] can be matched as a subsequence.
   - If pos[j] becomes ∞, longer prefixes cannot be matched (break out when needed).

2) Try to “bump” at the earliest possible position t ≥ 2:
   - We must keep C[1..t−1] = P[1..t−1] (so pos[t−1] must be finite).
   - At position t, choose the smallest letter ch > P[t] such that there is an occurrence after pos[t−1]: idx = next[pos[t−1] + 1][ch], and we can still complete K − t letters after idx, i.e., m − idx ≥ K − t.
   - Iterate t from 2 to K. For each t, iterate ch from P[t]+1 to 'Z' and test existence as above. Stop at the first success; this yields the lex smallest C > P.

3) Construct the code:
   - Copy C[1..t−1] = P[1..t−1], placed at the precomputed earliest positions pos[1], …, pos[t−1]. (Using earliest positions is always best: it leaves more suffix to finish and does not affect lex order because the letters are fixed.)
   - Set C[t] = ch at index idx.
   - Greedily fill the remaining positions j = t+1..K exactly like in the “no lower bound” case, starting from pos = idx.

4) If no bump is possible for any t, then there is no valid C > P for S, and the whole assignment is impossible.

Why earliest positions for the matched prefix suffice
Suppose we try to match P[1..t−1]. If we can’t find a valid ch > P[t] after the earliest placement pos[t−1], then placing the prefix later only reduces the remaining suffix (makes it strictly harder to place a larger letter and finish). Hence failure with earliest placement implies failure with any placement.

Overall algorithm
- Pair each name with its original index and sort all names lexicographically.
- prev = empty (no bound).
- For each name S in sorted order:
  - If |S| < K, print NO.
  - Build next occurrence table for S.
  - If prev is empty: build the smallest code for S.
  - Else compare f = S[1] with prev[1]:
    - f < prev[1] → NO.
    - f > prev[1] → build the smallest code for S.
    - f = prev[1] → run the “bump” procedure above to obtain the smallest code strictly greater than prev; if impossible → NO.
  - Save this code and set prev = this code.
- If all succeed, print YES and then output the assigned codes in the original input order.

Pseudocode sketch
- Sort countries by name.
- For each S (with original index idx) in sorted order:
  - if |S| < K: return NO.
  - Build next[pos][ch] for pos = 1..|S|+1, ch in 'A'..'Z'.
  - f = S[1].
  - if prev is empty or f > prev[1]:
    - code = smallest subsequence of length K starting with f (greedy).
  - else if f < prev[1]:
    - return NO.
  - else:
    - Compute earliest positions pos[j] matching prefix of prev using next.
    - For t from 2 to K:
      - if pos[t−1] is ∞: break.
      - For ch from prev[t]+1 to 'Z':
        - idx = next[pos[t−1]+1][ch]
        - if idx exists and |S| − idx ≥ K − t: choose this (t, ch, idx) and stop.
    - If nothing chosen: return NO.
    - Build code:
      - C[1..t−1] = prev[1..t−1] (placed at pos[1..t−1]).
      - C[t] = ch at idx.
      - Fill C[t+1..K] greedily using next with feasibility check |S| − idx ≥ remaining.
  - Store code for original idx; prev = code.
- Print YES and stored codes in input order.

Correctness argument
- Necessity: If there is a valid assignment, in sorted order the codes must be strictly increasing (otherwise the “iff” condition fails).
- Greedy optimality: At step i, we choose the lexicographically smallest feasible Ci > C(i−1). Any larger choice only raises the lower bound for step i+1, never helps. Thus if our greedy choice leads to failure later, any alternative (larger) choice would also fail. Therefore the greedy algorithm succeeds if and only if a solution exists.
- Minimal “> P” construction correctness: The bump-at-first-possible-position approach is the standard way to produce the smallest string strictly larger than a given one under subsequence constraints. Matching the prefix at the earliest possible positions maximizes the remaining suffix and thus maximizes feasibility; if no bump exists with earliest placements, none exists at all.

Complexity analysis
- Sorting N ≤ 1000 names: O(N log N · average name length), easily within limits.
- For each name S (sum |S| ≤ 200,000):
  - Build next table: O(26·|S|).
  - Construct code: O(K·26) time (filling greedily and at most one “bump” search).
- Total time: O(26·sum|S| + N·K·26) ≈ a few million primitive operations.
- Memory: next table per string uses about 26·(|S|+2) integers; process strings one by one to keep memory well under 256 MB. Storing N codes costs at most N·K ≤ 200,000 characters.

Edge cases and notes
- If any |S| < K, immediate NO.
- If K = 1, the codes are just the first letters. The sorted first letters must be strictly increasing; otherwise NO. The algorithm above naturally detects this (when f = prev[1], we cannot “bump” at any t ≥ 2 because K = 1).
- The first code letter always uses S[1] (the earliest occurrence of the required first letter); this is never harmful and often simplifies implementation.
- Be careful to output codes in the original input order after processing in sorted order.

Walkthrough on the samples
1) N=2, K=2, names: INDIA < INDONESIA.
- For INDIA: smallest code starting with 'I' is "IA".
- For INDONESIA with lower bound "IA" and same first letter:
  - Match 'I'; at position 2, bump 'A' to the smallest larger letter that appears, which is 'D'; code "ID".
- Codes: "IA", "ID" (also valid is "ID", "IN" as in the sample). Output YES.

2) N=3, K=2, names sorted: IAAA, IAAC, IBAA.
- IAAA → "IA".
- IAAC must be > "IA" with first letter 'I': bump second letter 'A' to 'C' → "IC".
- IBAA must be > "IC" with first letter 'I': second letter must be > 'C', but only 'B' and 'A' are available → impossible. Output NO.

Why this passes the limits
All operations are linear or near-linear in the total input size with small constants (alphabet 26, K ≤ 200). The memory footprint is modest when processing strings one at a time. Thus it fits 1.0s time and 256 MB memory constraints.

This completes the approach and its justification.