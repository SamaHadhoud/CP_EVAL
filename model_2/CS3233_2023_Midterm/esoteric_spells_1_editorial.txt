Restatement

You have n spellcasters. The i-th spellcaster may choose any integer xi in the inclusive range [0, ai]. The combined damage is the bitwise XOR of all chosen xi. For each test case, find:
- The maximum possible value of x1 ⊕ x2 ⊕ … ⊕ xn.
- One specific choice of x1, …, xn achieving that maximum.

Key ideas

1) Upper bound: XOR cannot exceed OR bitwise
- For any bit position b, the XOR result has a 1 at b if and only if an odd number of xi have that bit set.
- If the XOR has a 1 at bit b, at least one xi must have bit b = 1. That implies at least one ai ≥ 2^b, so the b-th bit of a1 | a2 | … | an (bitwise OR) must be 1.
- Therefore, for every bit b, XOR’s b-th bit ≤ OR’s b-th bit. Thus the maximum possible XOR is at most A = a1 | a2 | … | an.

2) Achievability: We can always reach A
- For each bit b that is 1 in A, at least one ai has bit b = 1. Assign this bit b to exactly one such i and add 2^b to xi. Do this for each 1-bit of A.
- For each i, we only collect bits that are already 1 in ai. Therefore xi is formed by a subset of the 1-bits of ai, which guarantees xi ≤ ai.
- Since each target bit b is assigned exactly once (to exactly one xi), in the final XOR that bit appears exactly once, so the XOR equals A.
- Hence the maximum XOR is exactly A, and the simple construction above yields a valid solution.

This settles both optimality and feasibility.

Algorithm

For each test case:
1) Compute A = a1 | a2 | … | an. This is the maximum damage.
2) Initialize x1 = x2 = … = xn = 0.
3) For each bit position b from 0 up to 60 (since ai ≤ 1e18):
   - If the b-th bit of A is 0, continue.
   - Otherwise, find any index i such that the b-th bit of ai is 1.
   - Set xi := xi + 2^b and move to the next bit.
4) Output A and the constructed x1, …, xn.

Why step 3 always succeeds: If A’s b-th bit is 1, there exists at least one ai with bit b = 1, so we can always find such an i. Adding 2^b to xi keeps xi ≤ ai because we only ever add bits that ai itself has.

Pseudocode (high level)

for each test case:
    read n and array a[1..n]
    A = 0
    for i in 1..n:
        A = A OR a[i]
    x[1..n] = all zeros
    for b in 0..60:
        if ((A >> b) & 1) == 1:
            find some i in 1..n with ((a[i] >> b) & 1) == 1
            x[i] += (1 << b)
    print A
    print x[1], x[2], ..., x[n]

Complexity

- Time: O(n + n · B) per test case, where B ≤ 60 (number of bits up to log2(1e18)). With n ≤ 100 and up to 100 test cases, this is easily within 1 second.
- Memory: O(n).

Correctness proof

- Upper bound: As argued, the XOR result cannot have a 1 in any bit where all ai have 0, so XOR ≤ OR bitwise. Thus max XOR ≤ A.
- Construction feasibility: For every 1-bit b of A, we select exactly one i with ai’s b-th bit = 1 and add 2^b to xi. Since each xi is a bitwise subset of ai, xi ≤ ai. Hence 0 ≤ xi ≤ ai for all i.
- Achieving A: Each bit b of A appears in exactly one xi, hence in the XOR the b-th bit is 1 (odd count). Bits not in A appear in none of the xi, hence 0 (even count). Thus x1 ⊕ x2 ⊕ … ⊕ xn = A.
- Optimality: Since no XOR can exceed A, and our construction achieves A, A is the maximum.

Handling all cases

- If all ai are 0, then A = 0 and the algorithm leaves all xi = 0.
- If n = 1, then A = a1 and we assign all bits of a1 to x1, i.e., x1 = a1.
- If many ai share the same bit b = 1, picking any one of them is fine; multiple answers are possible and acceptable.

Implementation notes

- Use 64-bit integers for ai, A, and xi since ai can be up to 1e18.
- Iterate bits up to 60 (inclusive is safe; 2^60 > 1e18).