# Problem Statement
Kivotos is in deep trouble. Familiar yet different monsters had made their appearance all over Kivotos, guarding what seems to be False Sanctums. The students plan to destroy these sanctums as fast as possible to stop the monster raid.

The hacker group of the Millenium Science School, Veritas, is trying to hack through the fifth False Sanctum’s defenses. The fifth False Sanctum is guarded by Hod, a large robot which was originally the communications unit AI of the Millennium, but was hacked by an unidentified AI and now claims itself to be the eighth prophet of the Decagrammaton.

The documentation states that Hod has a weak spot in its security system, which is represented by a 1-indexed string $S$ of length $N$. The document states that it is possible to project the _density_ of a continuous substring $S[l, r]$ in the following way:

- Partition the substring $S[l, r]$ to multiple substrings such that each of the smaller substrings have the same characters. For example, the string `maakkkki` is partitioned to 4 smaller substrings, which are `m`, `aa`, `kkkk`, and `i`.

- The _density_ is estimated to be the sum of squares of the lengths of these partitioned substrings. Following the previous example, the estimated _density_ would be $1^2 + 2^2 + 4^2 + 1^2 = 22$.

As the beloved Sensei of Kivotos students, you decide to help Veritas to do operations on the string $S$. Given the initial state of $S$, you have to process $Q$ queries in either of the following form:

- `1 i c`: Change the $i^{th}$ character of $S$ to $c$.
- `2 l r`: Report the projected _density_ of $S[l, r]$.

# Constraints

- $1 \leq N, Q \leq 2 \times 10^5$
- $S$ is a string consisting of lowercase alphabets at all times
- $1 \leq l \leq r \leq N$
- $1 \leq i \leq N$

# Input

The first line contains a two space-separated integers $N$ and $Q$, representing the length of the string $S$ and the number of queries respectively  
The second line contains a string of $N$ characters representing $S$  
$Q$ lines follows, the $i^{th}$ line contains 3 space-separated integers which represents the query type to be processed.

# Output

For every query of the second type, print a single integer representing the projected _density_ of the queried substring.

# Sample Input #1
```
9 3
aabcccacc
2 1 9
1 7 c
2 2 8
```
# Sample Output #1
```
19
27
```