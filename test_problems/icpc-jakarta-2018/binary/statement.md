# Problem Statement

A binary string is a non-empty sequence of 0’s and 1’s, e.g., 010110, 1, 11101, etc. Ayu has a favorite binary string $S$ which contains no leading zeroes. She wants to convert $S$ into its **decimal** representation with her calculator.

Unfortunately, her calculator cannot work on any integer larger than $K$ and it will crash. Therefore, Ayu may need to remove zero or more bits from $S$ while maintaining the order of the remaining bits such that its decimal representation is no larger than $K$. The resulting binary string also must not contain any leading zeroes.

Your task is to help Ayu to determine the minimum number of bits to be removed from $S$ to satisfy Ayu’s need.

For example, let $S = 1100101$ and $K = 13$. Note that $1100101$ is 101 in decimal representation, thus, we need to remove several bits from $S$ to make it no larger than $K$. We can remove the $3^{rd}$, $5^{th}$, and $6^{th}$ most significant bits, i.e. $1100101 \rightarrow 1101$. The decimal representation of $1101$ is 13, which is no larger than $K = 13$. In this example, we removed 3 bits, and this is the minimum possible (If we remove only 2 bits, then we will have a binary string of length 5 bits; notice that any binary string of length 5 bits has a value of at least 16 in decimal representation).

# Input

Input begins with a line containing an integer $K$ ($1 \le K \le 2^{60}$) representing the limit of Ayu’s calculator. The second line contains a binary string $S$ ($1 \le |S| \le 60$) representing Ayu’s favorite binary string. You may safely assume $S$ contains no leading zeroes.

# Output

Output contains an integer in a line representing the minimum number of bits to be removed from $S$.

# Sample Input #1
```
13
1100101
```
# Sample Output #1
```
3
```
# Explanation for the sample input/output #1

This sample is illustrated by the example given in the problem description above.

# Sample Input #2
```
13
1111111
```
# Sample Output #2
```
4
```
# Explanation for the sample input/output #2

Ayu must remove 4 bits to get 111, which is 7 in its decimal representation.
