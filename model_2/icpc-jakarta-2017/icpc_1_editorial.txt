Restatement

- There are N teams and M problems. Problem j has Tj testcases. Each team i solved S[i][j] testcases on problem j.
- A team solves problem j if and only if S[i][j] = Tj (i.e., all testcases on that problem are solved).
- The winner is the team that solved the most problems; ties are broken by choosing the smallest team index.
- Output the index (1-based) of the winning team.

Key observations

- For each problem j, whether team i solved it depends only on whether S[i][j] equals Tj. There is no partial credit beyond that.
- Problems with zero testcases (Tj = 0) are automatically solved by all teams because S[i][j] ≤ Tj implies S[i][j] = 0 = Tj. This is consistent with “solved all testcases.”
- We only need to count, for each team, how many problems they solved, and keep track of the best count and corresponding index with the given tie-break rule.

Algorithm

1) Read N and M.
2) Read array T[1..M].
3) Initialize:
   - bestCount = −1
   - bestIndex = 1
4) For each team i from 1 to N:
   - count = 0
   - Read the M integers S[i][1..M].
   - For each problem j from 1 to M:
     - If S[i][j] == T[j], increment count.
   - If count > bestCount, set bestCount = count and bestIndex = i.
   - Else if count == bestCount and i < bestIndex, set bestIndex = i (explicit tie-break, though scanning in increasing i and only updating on strictly greater would also work).
5) Output bestIndex.

Pseudocode

- read N, M
- read T[1..M]
- bestCount = −1
- bestIndex = 1
- for i in 1..N:
  - read row S[1..M]
  - count = 0
  - for j in 1..M:
    - if S[j] == T[j]:
      - count += 1
  - if count > bestCount or (count == bestCount and i < bestIndex):
    - bestCount = count
    - bestIndex = i
- print bestIndex

Complexity analysis

- Time: O(N × M), at most 10^4 comparisons for N, M ≤ 100, easily within 1 second.
- Memory: O(M) to store T (and O(1) extra to track counts and best team); you can process each team’s row on the fly without storing all S.

Correctness argument

- A team solves problem j if and only if S[i][j] = T[j] by definition. The inner loop counts exactly these solved problems per team, so count equals the number of solved problems for that team.
- The algorithm maintains bestCount as the maximum count seen so far. If a team’s count exceeds bestCount, we update to that team. If it equals bestCount, we update only when i is smaller than the current bestIndex, implementing the tie-break “smallest index among those with the maximum.”
- Edge cases:
  - Tj = 0: then S[i][j] = 0 for all teams, so every team gets this problem counted; the algorithm handles this naturally.
  - All teams solve the same number of problems (possibly zero): bestIndex remains the smallest index among them due to the tie-break logic.
  - N = 1 or M = 1: handled directly by the same counting and comparison.
- Therefore, the algorithm outputs exactly the index required by the problem for all valid inputs.

Implementation tips

- Use integer types for counts and comparisons; values fit in small ranges (0..100).
- Reading and processing each team’s row immediately avoids storing the full N×M matrix.