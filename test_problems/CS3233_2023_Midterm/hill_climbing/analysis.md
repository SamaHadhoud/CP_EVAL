If $\sum a_i < \left\lceil \frac{n}{2} \right\rceil$, the answer is **No** since we can’t color enough edges.

Otherwise, the answer is **Yes**. We solve for the case $\sum a_i = \left\lceil \frac{n}{2} \right\rceil$. If it’s greater, we can simply decrement any $a_i$ until equality is reached. Also, we can simply consider an arbitrary spanning tree $T$ of the graph. We will color $\left\lceil \frac{n}{2} \right\rceil$ of the edges in $T$ (out of a total of $n-1$ edges).

First, we will color edges in increasing order of the frequency $a_c$. We root $T$ at any vertex, and perform a depth-first search. The depth-first search function $\texttt{dfs}(u)$ would return the number of uncolored edges reachable from $u$ without going through any colored edges.

Suppose we are in $\texttt{dfs}(u)$, and let $v_1, v_2, \ldots, v_k$ be the children of $u$, and this is the _first time_ we have $\sum_{i=1}^{k}(1 + \texttt{dfs}(v_i)) \ge a_1$.

If there exist an index $i$ where $(1 + \texttt{dfs}(v_i)) \ge a_1$, we simply color the edges in the subtree of $i$ plus the edge $(u, v_i)$. Note that since we assume $u$ is the _first time_ we have $\sum_{i=1}^{k}(1 + \texttt{dfs}(v_i)) \ge a_1$, then it must mean $\texttt{dfs}(v_i) < a_1$. We color these edges, and delete the subtree of $v_i$. We continue from $a_2$ onwards.

Otherwise, let $l$ be the minimum integer such that $\sum_{i=1}^{l}(1 + \texttt{dfs}(v_i)) \ge a_1$. Then, we have $\sum_{i=1}^{l-1}(1 + \texttt{dfs}(v_i)) \le a_1 - 1$ and $(1 + \texttt{dfs}(v_l)) \le a_1 - 1$. Thus $a_1 \le \sum_{i=1}^{l}(1 + \texttt{dfs}(v_i)) \le 2a_1 - 2$. We color $a_1$ of these edges (such that the colored edges are connected), and delete the subtrees $v_1, v_2, \ldots, v_l$. We continue from $a_2$ onwards.

Note that since $a_1 \le a_2 \le \cdots$, after we process and color $a_i$ edges, all assumptions of the returned values of the $\texttt{dfs}$ function still hold since $\texttt{dfs}(v) < a_i \le a_{i+1}$.

Furthermore, for each color $c$, we delete at most $\max\{a_c, 2a_c - 2\}$ edges in our $\texttt{dfs}$ function. Suppose we only consider colors with a frequency greater than one (for $a_c = 1$, we can delete a leaf and recurse). If there are $o$ non-zero color frequencies, then we use at most $2\left\lceil \frac{n}{2} \right\rceil - 2o \le n - 1$ edges. Thus our $\texttt{dfs}$ function will always return a valid coloring. We can implement the $\texttt{dfs}$ function in such a way that it runs in linear time.

**Remark:** We did not, in fact, try implementing a hill-climbing algorithm. Perhaps it could pass the test cases?