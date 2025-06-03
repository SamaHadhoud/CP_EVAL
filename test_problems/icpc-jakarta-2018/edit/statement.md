# Problem Statement

A binary string is a non-empty sequence of 0’s and 1’s, e.g., 010110, 1, 11101, etc. The edit distance of two binary strings $S$ and $T$, denoted by $edit(S, T)$, is the minimum number of single-character edit (insert, delete, or substitute) to modify $S$ into $T$. For example, the edit distance of 0011 and 1100 is 4, i.e. 0011 → 011 → 11 → 110 → 1100. The edit distance of 1100101 and 1110100 is 2, i.e. 1100101 → 1110101 → 1110100.

Ayu has a binary string $S$. She wants to find a binary string with the same length as $S$ that maximizes the edit distance with $S$. Formally, she wants to find a binary string $T_{max}$ such that $|S| = |T_{max}|$ and $edit(S, T_{max}) \geq edit(S, T')$ for all binary string $T'$ satisfying $|S| = |T'|$.

She needs your help! However, since she wants to make your task easier, you are allowed to return any binary string $T$ with the same length as $S$ such that the edit distance of $S$ and $T$ is more than half the length of $S$. Formally, you must return a binary string $T$ such that $|S| = |T|$ and $edit(S, T) > \frac{|S|}{2}$.

Of course, you can still return $T_{max}$ if you want, since it can be proven that $edit(S, T_{max}) > \frac{|S|}{2}$ for any binary string $S$. This also proves that there exists a solution for any binary string $S$. If there is more than one valid solution, you can output any of them.

# Input

Input contains a binary string $S$ ($1 \leq |S| \leq 2000$).

# Output

Output in a line a binary string $T$ with the same length as $S$ that satisfies $edit(S, T) > \frac{|S|}{2}$.

# Sample Input #1
```
0011
```
# Sample Output #1
```
1100
```
# Explanation for the sample input/output #1

As illustrated in the example above, the edit distance of `0011` and `1100` is 4. Since $4 > \frac{4}{2}$, `1100` is one of the valid output for this sample.

# Sample Input #2
```
1100101
```
# Sample Output #2
```
0011010
```