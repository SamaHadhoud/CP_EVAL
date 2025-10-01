Restatement

You are given a binary string S of length n (1 ≤ n ≤ 2000). You must output any binary string T of the same length such that the edit distance (Levenshtein distance: insert, delete, substitute; all cost 1) between S and T is strictly greater than n/2. You do not need the T that maximizes the distance; any T satisfying edit(S, T) > n/2 is acceptable.

Key observations

1) A counting lower bound that costs nothing to compute:
- Let ones(X) be the number of ‘1’s in a string X.
- Any single edit changes ones(·) by at most 1.
- Therefore, for any strings X, Y: edit(X, Y) ≥ |ones(X) − ones(Y)|.
This bound is often already > n/2 if we choose Y with a very different number of ones.

2) Why “just flip every bit” is not enough:
- The Hamming distance between S and its bitwise complement is n, but Levenshtein distance can be much smaller due to insertions/deletions realigning the strings. For example, for S = 010101 and T = 101010, the edit distance is only 2 (delete the first character and insert the same at the end).
- So we must reason with lower bounds on edit distance that hold under insertions/deletions too.

3) The “balanced” case is the only tricky one:
- If ones(S) ≠ zeros(S), we can set T to be all the opposite bit of S’s majority. Then |ones(S) − ones(T)| = max(ones(S), zeros(S)) ≥ ⌊n/2⌋ + 1, immediately giving edit(S, T) > n/2.
- The only problematic case is when n is even and S has exactly n/2 ones and n/2 zeros. The counting bound then gives only n/2, not strictly greater.

4) For the balanced case, force two constraints at once:
- Make T have exactly one occurrence of one bit, and force that single occurrence to be at a position that S currently does not have (so one extra operation is unavoidable to place it there).
- Meanwhile, eliminate all occurrences of that bit elsewhere (forcing ≈ n/2 additional edits).
- This yields a guaranteed lower bound of n/2 + 1 edits.

A simple constructive strategy

Case A: S is unbalanced (ones(S) ≠ zeros(S))
- If ones(S) > zeros(S), output T = 0^n (all zeros).
- Else, output T = 1^n (all ones).
Why it works:
- edit(S, 0^n) ≥ ones(S), and edit(S, 1^n) ≥ zeros(S).
- The majority count is at least ⌊n/2⌋ + 1, so edit(S, T) > n/2.

Case B: S is balanced (n even and ones(S) = zeros(S) = n/2)
- Let b = S[1] be the first character of S (1-indexed). Construct:
  - T = (1 − b) followed by (n − 1) copies of b.
  - Concretely, if S starts with ‘0’, output T = 1 0 0 … 0 (one ‘1’, then n−1 zeros).
    If S starts with ‘1’, output T = 0 1 1 … 1 (one ‘0’, then n−1 ones).
Why it works (proof idea):
- Suppose S starts with 0, and T = 1·0^{n−1}. In any edit sequence turning S into T:
  - T has exactly one ‘1’ (at the first position). S has n/2 ones.
  - You must end up with exactly one ‘1’ overall and with the first character being ‘1’.
  - Two possibilities:
    1) You create a ‘1’ at the first position by substituting S[1] (0→1). That costs 1 and increases ones-count by 1. You still must eliminate all n/2 original ones elsewhere (each needs ≥1 edit, either substitution 1→0 or deletion). Total ≥ 1 + n/2 = n/2 + 1.
    2) You avoid substituting at position 1. To make the first character a ‘1’, you must delete some positive-length prefix that contains no ‘1’ and then later insert zeros to keep length n. That costs at least 2 edits beyond the operations needed to reduce the number of ‘1’s to exactly one. Since S initially has n/2 ones and you keep exactly one, at least n/2 − 1 “1-eliminating” edits are needed; plus the 2 positioning edits gives at least (n/2 − 1) + 2 = n/2 + 1.
  - In both subcases, edit(S, T) ≥ n/2 + 1.
- The argument is symmetric if S starts with ‘1’ (use T = 0·1^{n−1}).

Therefore in all cases, this construction yields a T with edit(S, T) > n/2.

Pseudocode

- Let n = |S|, c1 = number of ‘1’s in S, c0 = n − c1.
- If c1 ≠ c0:
  - If c1 > c0: output string T of length n consisting of all ‘0’.
  - Else: output string T of length n consisting of all ‘1’.
- Else (balanced: c1 = c0 = n/2):
  - If S[1] = ‘0’: output T = ‘1’ followed by (n−1) ‘0’.
  - Else (S[1] = ‘1’): output T = ‘0’ followed by (n−1) ‘1’.

Complexity

- Counting ones and constructing T both take O(n) time and O(1) extra memory, comfortably within 1.0 s and 256 MB.

Why this is correct

- For unbalanced S, the edit distance to the uniform opposite string is at least the majority count, which is strictly greater than n/2.
- For balanced S, the “one-opposite-at-front” T forces:
  - Either one substitution at the first position plus eliminating all the original ones (or zeros) elsewhere, or
  - At least two edit operations to move a ‘1’ (or ‘0’) to the front without substitution, plus eliminating the remaining ones (or zeros).
- Either way, the total is at least n/2 + 1 > n/2.
- This covers all possible S and guarantees a valid T always.

Examples

- S = 0011 (n = 4, balanced, S[1] = 0)
  - Our construction outputs T = 1000. The edit distance is 3 (> 2). The sample’s 1100 also works (distance 4).

- S = 1100101 (n = 7, ones = 4, zeros = 3)
  - Unbalanced with more ones. Output T = 0000000. Then edit(S, T) ≥ ones(S) = 4 > 3.5. The sample’s 0011010 also works.

Takeaways

- The naive “flip every bit” does not guarantee a large Levenshtein distance due to insert/delete realignments.
- A robust counting argument handles all unbalanced strings.
- For the balanced case, forcing a unique minority bit to sit at the first position adds one unavoidable extra operation on top of eliminating half the characters, giving a clean n/2 + 1 bound.