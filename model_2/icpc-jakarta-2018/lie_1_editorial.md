Restatement

You are given N outputs of lie detectors S1..SN, each being the string TRUTH or LIE. The 1st detector reports whether the eyewitness testimony is true. For i ≥ 2, the i-th detector reports whether the (i−1)-th detector’s result is truthful. The N-th detector is guaranteed to be correct. Using all outputs, determine whether the eyewitness testimony itself is TRUTH or LIE.

Key ideas

1) Think in terms of correctness propagation
- Let Ci be a boolean meaning “the i-th detector’s statement is correct” (i.e., it truthfully describes its subject).
- We know CN = true (the last detector is not tampered).
- For i ≥ 2, the i-th detector’s statement Si says whether the (i−1)-th detector is TRUTH or LIE.
  - If Ci is true and Si = TRUTH, then C(i−1) = true.
  - If Ci is true and Si = LIE, then C(i−1) = false.
  - If Ci is false (the i-th detector lies), the above is inverted:
    - Si = TRUTH actually means C(i−1) = false,
    - Si = LIE actually means C(i−1) = true.
- Equivalently: C(i−1) = Ci XOR [Si == LIE].
  This is a simple “flip if Si is LIE” rule while moving backward.

- Finally, the testimony’s truth T depends on the first detector’s correctness:
  - If C1 is true, then T = S1.
  - If C1 is false, then T = not S1.
  Equivalently: T = C1 XOR [S1 == LIE].

2) Parity (XOR) simplification
Combine the two XOR relations above:
- Start with CN = true.
- Repeatedly apply C(i−1) = Ci XOR [Si == LIE] for i = N down to 2.
- Then T = C1 XOR [S1 == LIE].
By associativity/commutativity of XOR,
C1 = CN XOR ([S2 == LIE] XOR [S3 == LIE] XOR ... XOR [SN == LIE]),
so
T = CN XOR ([S1 == LIE] XOR [S2 == LIE] XOR ... XOR [SN == LIE]).
Since CN = true, this becomes:
T is TRUTH if and only if the number of LIE among all N outputs is even.
T is LIE if and only if that number is odd.

This is the main insight: the entire chain collapses to the parity of the count of LIE in S1..SN.

Algorithm

Two equally simple ways to implement:

A) Backward toggle method
- trust = true  // represents current Ci while walking backward; starts at CN
- For i from N down to 2:
  - If Si == LIE: trust = not trust
- After the loop, trust equals C1.
- If trust is true: the testimony equals S1
- Else: the testimony is the opposite of S1

B) Parity method (simplest)
- Count how many of S1..SN equal LIE.
- If the count is even, output TRUTH; else output LIE.

Pseudocode

- Parity method:
  - read N
  - cnt = 0
  - repeat N times:
    - read s
    - if s == "LIE": cnt++
  - if cnt % 2 == 0: print "TRUTH" else print "LIE"

Correctness proof

- Let b(Si) be 1 if Si == LIE, else 0.
- From the detector semantics, C(i−1) = Ci XOR b(Si), with CN = 1 (true).
- Unrolling,
  C1 = CN XOR b(SN) XOR b(SN−1) XOR ... XOR b(S2).
- The testimony T equals S1 if C1 is true, and the opposite otherwise, i.e.
  T = C1 XOR b(S1).
- Therefore
  T = CN XOR [b(S1) XOR b(S2) XOR ... XOR b(SN)].
- Because CN = 1, T is TRUTH iff the XOR of all b(Si) is 0, i.e., the total number of LIE strings is even.
- This matches both the backward-toggle reasoning and the provided examples. Hence the algorithm is correct.

Complexity analysis

- Time: O(N), a single pass through the inputs.
- Memory: O(1), only a counter or a boolean toggle is maintained.

Why this handles all cases

- The last detector being correct seeds the process; each LIE flips the correctness of the item it talks about, so flipping accumulates with parity.
- The algorithm does not rely on any special patterns beyond counting LIEs, so it works for all N within the limits, including all TRUTH, all LIE, or arbitrary mixes.
- No storage of the entire sequence is needed; you can process inputs on the fly, which fits the 256 MB memory limit easily and is well within the 1 s time limit.