Consider a valid set of slides with the minimum speed of the slide to be $L$ and the maximum speed to be $R$. It is no less optimal to consider **every** type of slide which speeds fall into this interval. Also, if a valid set of slides with speed interval $[L, R]$ can make a valid level, then so are sets with speed intervals $[L - 1, R]$ and $[L, R + 1]$. From the two observations above, we can solve the problem by sorting the slides in order of increasing speed, and maintain two pointers representing $L$ and $R$. Consider the current interval made by these two pointers: if $[L, R]$ makes a valid set, then we know the answer is at most $W_R - W_L$, so we can check the interval $[L + 1, R]$; otherwise we should check the interval $[L, R + 1]$.

The question is now simplified to the following: given a set of slides (disregarding their speed values), can they make a valid level?

Instead of considering each type of slide as its own, we can make an adjacency matrix $mat[u][v]$ which counts the number of slides that start from button $u$ and end at button $v$. The matrix is of size $8 \times 8$, as there are only 8 buttons.

Now consider a valid level, what properties does that level have? Let $mat'[u][v]$ be the number of slides used in the level that start from button $u$ and end at button $v$. Then, we have:

- $0 \leq mat'[u][v] \leq mat[u][v]$ for all $(u,v),\ 1 \leq u,v \leq 8$
- $\sum_k mat'[k][u] = \sum_l mat'[u][l]$ for all $1 \leq u \leq 8$
- $\sum_k mat'[u][k] > 0$ for all $1 \leq u \leq 8$
- $\sum_{(u,v)} mat'[u][v] \geq K$

One can try code an Integer Linear Programming solver that tries to maximize $\sum_{(u,v)} mat'[u][v]$ but that’s difficult (and won’t pass time limit anyway).

The solution is to observe that the first two conditions are similar to that of what you would see in a Maximum Flow problem. Let’s construct a graph where we can apply Minimum Cost Maximum Flow in:

- Divide each button into two vertices in the flow graph: an in-vertex and an out-vertex.
- Represent $mat[u][v]$ as an edge from $u_{out}$ to $v_{in}$ with capacity $mat[u][v]$ and cost $-1$.
- Make an edge from $u_{in}$ to $u_{out}$ with capacity $\sum mat[u][v]$ and cost 0.
- To enforce the condition that all buttons must be used at least once, we can have another edge from $u_{in}$ to $u_{out}$ with capacity 1 and cost $-LARGE$, where $LARGE$ is a large number. This will encourage the minimum cost maximum flow algorithm to visit all buttons before considering the maximum number of edges used.

It can be shown that the minimum cost of the resulting flow will represent a valid solution to the above linear inequalities and it maximizes $\sum mat'[u][v]$. There are no source or sink nodes in this construction, as we won’t be considering an actual source-to-sink flow. Instead, we will simply resolve the negative cost cycles via a negative cycle cancellation algorithm that will fill the graph with flow in its own.

The idea of such algorithm is to run an algorithm that can find any negative cycle (such as Bellman-Ford or SPFA) and determine the smallest residual capacity (bottleneck capacity) along that cycle. We then augment the flow manually along the cycle by this minimum amount. This step effectively "cancels" the negative cycle by reducing the overall cost of the flow. We repeat until no more negative cycles are available, in which means our flow graph is solved according to the requirements of the problem.

The time complexity of such algorithm is $\mathcal{O}(Fnm)$ where $F$ represents the number of flow, $n$ represents the number of vertices, and $m$ represents the number of edges. Consider that $F \leq \sum mat[u][v] = \mathcal{O}(M)$, $n = \mathcal{O}(V)$, and $m = \mathcal{O}(V^2)$ ($V$ represents the number of buttons, which is 8), the time complexity of the algorithm with respect to the original problem is $\mathcal{O}(MV^3)$.

Since we will be looking at $\mathcal{O}(M)$ intervals from using two pointers, our final solution has a complexity of $\mathcal{O}(M^2V^3)$ (with a fractional constant).