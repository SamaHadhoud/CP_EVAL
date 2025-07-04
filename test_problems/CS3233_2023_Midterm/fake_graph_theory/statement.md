# Problem Statement
> SoCCat the Combinatorician is a very combinatoricky cat. Currently, SoCCat is challenging you to solve a combinatorics problem!

SoCCat challenges you to solve the following combinatorics problem:

Given a graph $G$ with $4n$ vertices, along with two permutations $p$ and $q$, each with size $2n$ on the set $\{1, 2, \ldots, 2n\}$.

Add the following $4n$ edges:

- For each $i \in \{1, 2, 3, \ldots, 2n\}$, add an edge between vertices $i$ and $i + 2n$.
- For each $i \in \{1, 2, 3, \ldots, n\}$, add an edge between vertices $p_i$ and $p_{i+n}$.
- For each $i \in \{1, 2, 3, \ldots, n\}$, add an edge between vertices $q_i + 2n$ and $q_{i+n} + 2n$.

Suppose the permutations $p$ and $q$ are chosen uniformly at random among the $(2n)!$ possible permutations. Note that the permutations $p$ and $q$ are independent of each other. What is the expected number of connected components in the resulting graph?

Suppose the expected number of connected components in the resulting graph is $E$. Then, let $E' = E \times (2n)! \times (2n)!$. It can be proven that $E'$ is always an integer. You are required to output the value of $E' \mod 1\ 000\ 003\ 233$.

In other words, you should output the sum of the number of connected components in the resulting graph, over all $(2n)! \times (2n)!$ possible permutations $p$ and $q$.

# Input

The first line of input contains an integer $n$, as described in the problem statement ($1 \le n \le 3233$).

# Output

Output a single integer, the value of $E' \bmod 1\ 000\ 003\ 233$.

# Notes

In the sample test case, there’s a $\frac{1}{3}$ chance that there are $2$ connected components, and $\frac{2}{3}$ chance that there is $1$ connected component.

Therefore, $E = \frac{1}{3} \times 2 + \frac{2}{3} \times 1 = \frac{4}{3}$, and the output should be $\frac{4}{3} \times (4!)^2 = 768$.

# Sample Input #1
```
2
```
# Sample Output #1
```
768
```