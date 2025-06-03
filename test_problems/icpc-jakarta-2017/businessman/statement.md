# Problem Statement

There are $N$ cities (numbered from 1 to $N$) connected by $M$ bidirectional roads such that from any city, it is possible to reach any other cities with one or more roads. The $i$-th city has an economic value of $S_i$, and each road directly connects two different cities.

There are $Q$ queries, which should be executed one by one, each represented by a tuple $(A_i, B_i, C_i)$.

1. If $A_i = 0$, then you need to change the economic value of city $B_i$ to $C_i$.
2. If $A_i = 1$, then you need to output the answer to the following question: Suppose there are two businessmen, one of them is in city $B_i$, while the other is in city $C_i$. They both agree to each other on a non-negative integer $X$, where $X$ is the number of days in which they are moving around. Each day, both businessmen move to any city adjacent to the current city they are in, and they repeat this routine for $X$ days. They cannot stay in the same city for two consecutive days, but they may revisit cities which have been visited before. After $X$ days, they should be in the cities such that the difference of the economic value between those two cities are minimum. Output the minimum difference. Note that the two cities can be the same city.

# Input

The first line contains two integers: $N\ M$ ($1 \le N \le 100{,}000;\ 1 \le M \le 200{,}000$) in a line denoting the number of cities and roads. The second line contains $N$ integers: $S_1\ S_2\ \cdots\ S_N$ ($0 \le S_i \le 1{,}000{,}000{,}000$) in a line denoting the economic value of each city. The next $M$ following lines, each contains two integers: $u_i\ v_i$ ($1 \le u_i, v_i \le N;\ u_i \ne v_i$) in a line, which means the $i$-th road connects city $u_i$ and city $v_i$. It is guaranteed that from any city, it is possible to reach any other cities by using one or more roads. The next line contains an integer: $Q$ ($1 \le Q \le 100{,}000$) denoting the number of queries. The next $Q$ lines, each contains three integers: $A_i\ B_i\ C_i$ ($0 \le A_i \le 1$) in a line denoting the queries. For each query, if $A_i = 0$, then $1 \le B_i \le N$ and $0 \le C_i \le 1{,}000{,}000{,}000$; otherwise $1 \le B_i, C_i \le N$. There will be at least one query where $A_i = 1$.

# Output

For each query where $A_i = 1$, output the minimum difference of the economic value of the two cities that can be reached by the businessmen after $X$ days, in a line. Note that the value of $X$ is any non-negative integer and independent between different queries.

# Sample Input
```
6 6
0 0 0 0 0 0
1 2
1 6
5 1
2 3
3 4
3 5
7
1 1 2
0 1 10
0 3 20
1 1 2
0 4 11
1 1 3
1 1 6
```
# Sample Output
```
0
10
0
1
```
# Explanation for the 1st sample case

The following are the road configuration and the queries for the first sample:

![IMAGE](icpc-jakarta-2017-businessman-1.png)


- Originally all cities have an economic value of $0$.
- 1$^\text{st}$ query: $1\ 1\ 2$. As the economic value of all cities is the same (i.e. $0$), then they can decide on any $X$ and move to any cities they want; the difference will be $0$.
- 4$^\text{th}$ query: $1\ 1\ 2$. This query is the same as the 1$^\text{st}$ query, but now city 1 has an economic value of $10$ (due to the 2$^\text{nd}$ query), while city 3 has an economic value of $20$ (due to the 3$^\text{rd}$ query). The minimum difference of $10$ can be achieved if they choose $X = 0$. Follows are some examples where the difference is **not** minimum:
  - $X = 1$: the first businessman moves from city 1 to city 5 (with an economic value of $0$), while the second businessman moves from city 2 to city 3 (with an economic value of $20$). The difference of the economic value of the two final cities is $20$.
  - $X = 2$: the first businessman moves from city 1 $\rightarrow$ city 5 $\rightarrow$ city 3 (with an economic value of $20$), while the second businessman moves from city 2 $\rightarrow$ city 3 $\rightarrow$ city 4 (with an economic value of $0$). The difference of the economic value of the two final cities is $20$.
  - It is possible to achieve a minimum difference of $10$ with $X = 1$, e.g., the first businessman moves from city 1 to city 2 (with an economic value of $0$), while the second businessman moves from city 2 to city 1 (with an economic value of $10$). It is also possible to achieve this with $X = 2$, e.g., the first businessman moves from city 1 $\rightarrow$ city 5 $\rightarrow$ city 1 (with an economic value of $10$), while the second businessman moves from city 2 $\rightarrow$ city 3 $\rightarrow$ city 2 (with an economic value of $0$). Among all possible moves, the minimum difference which can be achieved in this case is $10$.
- 6$^\text{th}$ query: $1\ 1\ 3$. The two businessmen can choose $X = 1$ and move to city 2 (or city 5) altogether.
- 7$^\text{th}$ query: $1\ 1\ 6$. They can choose $X = 3$ and move such that the final cities are city 4 (with an economic value of $11$) and city 1 (with an economic value of $10$).