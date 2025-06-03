# Problem Statement

A permutation $P$ of size $N$ is defined as an array $[P_1, P_2, \cdots, P_N]$ where $1 \le P_i \le N$ and $P_i \ne P_j$ for $i \ne j$.

We also define an order of a permutation. If $A$ and $B$ are permutations of size $N$, then $A$ is less than $B$ if and only if there exists an index $i$ ($1 \le i \le N$) where:

- $A_i < B_i$, and  
- $A_j = B_j$ for all $1 \le j < i$

We also define the multiplication of two permutations. If $A$ and $B$ are permutations of size $N$, then $A \times B$ is a permutation of size $N$, where the $i$-th element is $A_{B_i}$.

We also define the exponentiation of a permutation and a positive integer. If $P$ is permutation and $z$ is a positive integer, then $P^z$ is defined as follow:

- $P^z = P$, for $z = 1$  
- $P^z = P^{z-1} \times P$, for $z > 1$

You are given a permutation $P$ of size $N$. Let $M$ be the smallest integer greater than $1$ such that $P = P^M$. We define $A$ (index starts from $1$) as an array consisting of $P^i$ for all $1 \le i < M$ sorted in increasing order (of permutation). In other words, $A_i < A_j$ for all $1 \le i < j < M$.

For example, suppose $P = [2,3,1,5,4]$. Therefore:

- $P^1 = [2,3,1,5,4]$  
- $P^2 = [3,1,2,4,5]$  
- $P^3 = [1,2,3,5,4]$  
- $P^4 = [2,3,1,4,5]$  
- $P^5 = [3,1,2,5,4]$  
- $P^6 = [1,2,3,4,5]$  
- $P^7 = [2,3,1,5,4]$

Thus, the value of $M$ in this case is $7$, and $A = [P^6, P^3, P^4, P^1, P^2, P^5]$.

You are also given $Q$ queries. The $i$-th query contains an integer $K_i$. The answer for the $i$-th query is an integer $T_i$ such that $1 \le T_i < M$ and $P^{T_i} = A_{K_i}$. Can you answer all of the queries?

# Input

The first line contains two integers: $N \ Q$ ($1 \le N \le 100$; $1 \le Q \le 300,000$) in a line denoting the size of the permutation and the number of queries.  
The second line contains $N$ integers: $P_1\ P_2\ \cdots\ P_N$ ($1 \le P_i \le N$) in a line denoting the permutation.  
It is guaranteed that $P_i \ne P_j$ for all $i \ne j$.  
The next $Q$ lines, each contains an integer; the integer on the $i$-th line is $K_i$ ($1 \le K_i < M$, where $M$ is the smallest integer greater than $1$ such that $P = P^M$ as explained above. Note that $M$ is not explicitly given in the problem) denoting the query.

# Output

Q lines, each contains an integer: T_i in a line denoting the answer of the i-th query.

# Sample Input
```
5 6
2 3 1 5 4
1
2
3
4
5
6
```
# Output for Sample Input
```
6
3
4
1
2
5
```
# Explanation for the 1st sample case

The permutation given in the first sample is the same as the permutation given in the problem description.
