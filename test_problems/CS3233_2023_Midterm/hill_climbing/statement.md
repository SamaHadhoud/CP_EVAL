# Problem Statement
> SoCCat the Hill Climber is a very determined cat. Currently, SoCCat is climbing the hill of an NP-hard problem!

Everyone knows that the graph edge partitioning problem is NP-hard. As there are many variants of it, here’s the one that SoCCat is talking about:

There is a connected graph with $n$ vertices and $m$ edges. You can color each edge into $m$ possible colors, numbered from $1$ to $m$. However, you can only color at most $a_c$ edges with color $c$.

A coloring of a subset of edges is **valid** if it satisfies all of the following conditions for each color $1 \leq c \leq m$:

- There are at most $a_c$ edges with color $c$.
- Every pair of edges with color $c$ is reachable from each other by only using edges with color $c$. In other words, if $V_c$ is the set of all vertices adjacent to an edge with color $c$, and $E_c$ is the set of all edges with color $c$, then the subgraph $(V_c, E_c)$ is connected.

Note that you can leave some edges uncolored. Uncolored edges do not have to satisfy any of the above conditions.

SoCCat is confident that their hill-climbing algorithm can correctly determine whether a **valid** coloring with at least $\left\lceil \frac{n}{2} \right\rceil$ **colored edges** exist with 99.8244353% probability.

SoCCat does not believe you could match their algorithm’s performance in a mere 4 hours. After all, SoCCat took weeks to design and code a very efficient and robust hill-climbing algorithm to do this task.

Can you do it?

# Input

The input starts with a line containing an integer $C$, denoting the number of test cases $(1 \leq C \leq 300\ 000)$. Each test case has the following format.

The first line contains two integers $n$ and $m$, denoting the number of vertices and the number of edges in the graph, respectively $(2 \leq n \leq 300\ 000;\ n - 1 \leq m \leq 300\ 000)$. The sum of $n$ and $m$ over all test cases does not exceed 300 000.

The second line contains $m$ space-separated integers $a_1, a_2, \ldots, a_m$, where $a_i$ denotes the maximum number of edges you can color with color $i$ $(0 \leq a_i \leq m;\ \sum_i a_i \leq m)$.

The following $m$ lines each contain two integers $u$ and $v$, denoting an edge between vertices $u$ and $v$ $(1 \leq u, v \leq n)$. The graph is guaranteed to be connected and has no self-loops or multiple edges.

# Output

For each test case, output **Yes** if there exists a valid coloring with at least $\left\lceil \frac{n}{2} \right\rceil$ colored edges, and **No** otherwise.

Additionally, if the answer is **Yes**, output a line containing $m$ space-separated integers $c_1, c_2, \ldots, c_m$. $c_i$ should be equal to the color of the $i$-th edge, or $0$ if the $i$-th edge is uncolored $(0 \leq c_i \leq m)$.

There should be at least $\left\lceil \frac{n}{2} \right\rceil$ colored edges, and the coloring should be valid. If there are multiple possible answers, output any one of them.

# Sample Input #1
```
4
5 4
4 0 0 0
2 3
2 1
4 5
4 3
6 11
0 0 0 0 0 0 0 3 2 3 3
1 2
2 3
3 4
4 5
5 6
6 1
1 4
4 2
2 5
5 3
4 6
6 11
0 0 0 0 0 0 0 3 2 3 3
1 2
2 3
3 4
4 5
5 6
6 1
1 4
4 2
2 5
5 3
4 6
5 10
1 0 0 0 0 0 0 0 0 1
1 2
2 3
3 4
4 5
5 1
1 3
3 5
5 2
2 4
4 1
```
# Sample Output #1
```
Yes
1 1 0 1
Yes
8 11 10 10 9 8 8 11 9 10 11
Yes
11 11 11 0 0 0 0 0 0 0 0
No
```