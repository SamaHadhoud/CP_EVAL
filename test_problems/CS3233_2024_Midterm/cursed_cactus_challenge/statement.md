# Problem Statement
You are given a simple connected graph $G$ with $n$ vertices and $m$ edges. Each vertex $i$ has a value $a_i$. No two edges connect the same pair of vertices. Interestingly, each edge lies in at most one simple cycle.

A simple cycle is a sequence of $k$ distinct vertices $v_1, v_2, \ldots, v_k$, such that there is an edge connecting $v_i$ and $v_{i+1}$ for every $1 \le i \le k - 1$, and also an edge connecting $v_k$ and $v_1$. We note that these $k$ edges lies in the simple cycle  
$v_1 \rightarrow v_2 \rightarrow \cdots \rightarrow v_k \rightarrow 1$.

Let $S$ be a subset of vertices. We call $S$ an independent set if for all $u, v \in S$ and $u \ne v$, there is no edge connecting the vertices $u$ and $v$. The score $F(S)$ of an independent set $S$ is defined as

$$
F(S) = \left( \sum_{v \in S} a_v \right)^2
$$

Let $\mathcal{I}$ be the set of all possible independent sets in $G$. In other words,

$$
\mathcal{I} = \{ S \subseteq \{1, 2, \ldots, n\} \mid S \text{ is an independent set} \}
$$

Find the following sum, modulo $998\ 244\ 353$.

$$
\sum_{S \in \mathcal{I}} F(S)
$$

# Input

The first line of input contains two integers $n$ ($2 \le n \le 10^5$) and $m$ ($n - 1 \le m \le 2n$).

The second line contains $n$ integers, the $i$-th of which denotes $a_i$ ($1 \le a_i \le 10^8$).

It is guaranteed that the given graph is simple, connected, and each edge lies in at most one simple cycle.

# Output

Output $\sum_{S \in \mathcal{I}} F(S)$, modulo $998\ 244\ 353$.

# Sample Input #1
```
3 2
1 1 1
1 2
2 3
```
# Sample Output #1
```
7
```
# Sample Input #2
```
4 4
3 2 3 3
1 2
2 3
3 4
4 1
```
# Sample Output #2
```
92
```
# Sample Input #3
```
10 12
1 2 3 4 5 6 7 8 9 10
1 2
2 3
3 1
1 4
4 5
5 1
1 6
6 7
7 1
1 8
8 9
9 10
```
# Sample Output #3
```
50520
```