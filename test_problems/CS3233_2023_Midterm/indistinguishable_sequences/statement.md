# Problem Statement
> SoCCat the Group Theorist is a very detail-oriented cat. Currently, SoCCat is researching a unique equivalence relation on sequences!

SoCCat defines a map $L : \bigcup_{i \in \mathbb{N}} \mathbb{Z}^i \rightarrow \mathbb{N}$ which takes in a sequence $\mathbf{s} = (s_1, s_2, \ldots, s_k)$ of integers and outputs the length of a longest (strictly) increasing subsequence of $\mathbf{s}$.

Two sequences $\mathbf{s} = (s_1, s_2, \ldots, s_k)$ and $\mathbf{t} = (t_1, t_2, \ldots, t_l)$ are said to be _indistinguishable_ if and only if they belong to the same fiber of the map $L$ – in other words, if $L(s_1, s_2, \ldots, s_k) = L(t_1, t_2, \ldots, t_l)$. That is, two sequences are indistinguishable if and only if the lengths of their longest increasing subsequences are the same.

SoCCat considers a very natural question: given a sequence $\mathbf{a} = (a_1, a_2, \ldots, a_n)$, how many _subsequences_ of $\mathbf{a}$ are indistinguishable from the original sequence $\mathbf{a}$ itself?

SoCCat doesn’t believe that you can do it, so they simply gave you a **random permutation** $\mathbf{a}$ for you to tinker with.

Surprise SoCCat by outputting the answer modulo $1\ 000\ 003\ 233$!

# Input

The first line of input contains an integer $n$, the length of the sequence $(1 \leq n \leq 100\ 000)$.

The second line of input contains $n$ integers $a_1, a_2, \ldots, a_n$, the integers in the permutation $(1 \leq a_i \leq n;\ a_i \neq a_j\ \text{if}\ i \neq j)$. The permutation $\mathbf{a}$ is uniformly randomly generated from all $n!$ possible permutations.

# Output

Output a single integer, the number of subsequences of $\mathbf{a}$ that are indistinguishable from $\mathbf{a}$. In other words, output the number of subsequences of $\mathbf{a}$ such that the length of their longest increasing subsequence is equal to the length of the longest increasing subsequence of $\mathbf{a}$.

As the answer can be very large, output the answer modulo $1\ 000\ 003\ 233$.

# Notes

A sequence $\mathbf{b}$ is called a subsequence of another sequence $\mathbf{a} = (a_1, \ldots, a_n)$ if there is a strictly increasing sequence of indices $i_1, \ldots, i_p$ such that $\mathbf{b} = (a_{i_1}, \ldots, a_{i_p})$.

In the sample test case, we have $L(\mathbf{a}) = 2$, and any subsequences of $\mathbf{a}$ that belongs to the same fiber of $L$ as $\mathbf{a}$ must contain $a_2 = 1$, as well as either of $a_3$ or $a_4$ (or both).

# Sample Input #1
```
4
4 1 3 2
```
# Sample Output #1
```
6
```
# Sample Input #2
```
10
2 4 8 7 3 9 5 6 1 10
```
# Sample Output #2
```
84
```