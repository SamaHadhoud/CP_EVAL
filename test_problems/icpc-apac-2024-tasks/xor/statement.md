# Problem Statement

You are given $n$ integers $a_1, a_2, \ldots, a_n$. You have a sequence of $n$ integers $B = (b_1, b_2, \ldots, b_n)$ which initially are all zeroes.

In one operation, you choose two different indices $i$ and $j$, then simultaneously

- replace $b_i$ with $b_i \oplus a_i \oplus a_j$, and
- replace $b_j$ with $b_j \oplus a_i \oplus a_j$.

Note that $\oplus$ represents the bitwise XOR operation, which returns an integer whose binary representation has a $1$ in each bit position for which the corresponding bits of either but not both operands are $1$. For example, $3 \oplus 10 = 9$ because $(0011)_2 \oplus (1010)_2 = (1001)_2$.

You want to compute the number of different possible sequences $B$ you can obtain after performing zero or more operations. Since this number might be huge, calculate this number modulo $998244353$.

Two sequences of length $n$ are considered different if and only if there exists an index $i$ ($1 \le i \le n$) such that the $i$-th element of one sequence differs from the $i$-th element of the other sequence.

# Input

The first line of input contains one integer $n$ ($2 \le n \le 200000$). The second line contains $n$ integers $a_1, a_2, \ldots, a_n$ ($0 \le a_i < 2^{30}$ for all $i$).

# Output

Output an integer representing the number of different possible sequences $B$ you can obtain after performing zero or more operations modulo $998244353$.

# Sample Input #1
```
3
1 2 1
```
# Sample Output #1
```
4
```
### Explanation for the sample input/output #1

Starting from $B = (0, 0, 0)$, we can obtain the following two sequences $B$:

- Perform the operation with $i = 1$ and $j = 2$. We will have $B = (3, 3, 0)$.
- After that, perform the operation with $i = 2$ and $j = 3$. We will have $B = (3, 0, 3)$.

Starting from $B = (0, 0, 0)$, we can also obtain the following sequence $B$:

- Perform the operation with $i = 2$ and $j = 3$. We will have $B = (0, 3, 3)$.

It can be shown that $(0, 0, 0)$, $(3, 3, 0)$, $(3, 0, 3)$, and $(0, 3, 3)$ are the only possible sequences $B$ you can obtain.  
Therefore, the answer is $4$.

# Sample Input #2
```
4
852415 852415 852415 852415
```
# Sample Output #2
```
1
```