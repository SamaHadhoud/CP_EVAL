Because the graph is planar, any subset of $v$ nodes (for $v > 2$) nodes can only contain at most $3v - 6$ edges between them.

**Observation B.1.**  
It is sufficient to check only clique subgraphs or cliques with one missing edge. We define this as our search space.

**Proof.**  
For $v > 6$, the score of any subgraph is not positive since $(3v - 6) - \binom{v}{2} - (3v - 6))^2 < 0$.

For $v = 6$, the maximum number of edges possible is 12, which means there are at least 3 missing edges (there are 15 edges in $K_6$). Let $x$ be a node with at least one missing edge, so its degree is at most 4. Removing $x$ decreases the number of edges by at most 4 but reduces the number of missing edges to 2 or less, hence saving a penalty of at least $(3^2 - 2^2) \times 10^6 = 5 \times 10^6$, which is more than the score of 4 edges, so it’s better to remove node $x$. Thus, we can focus on $v \leq 5$.

For $v = 5$, with two missing edges, the score is not better than if the node of minimum degree is removed. This reduced graph (without the node of minimum degree) is in our search space. With more than two missing edges, the score is negative.

For $v < 5$, the score is not positive if there is more than one missing edge.

We can enumerate all $O(n)$ clique subgraphs of a planar graph in $O(n \log n)$ time. To do that, we look at the node with the minimum degree (call this node $u$). Let the set of neighbours of node $u$ be $S_u$. Since the graph is planar, it is guaranteed that $|S_u| \leq 5$, so we can obtain all cliques containing node $u$ in $O(1)$. We then continue the algorithm by enumerating cliques without node $u$. We can do so by removing node $u$ from the graph and recursively doing the algorithm on the rest of the graph, which is still planar.

Thus, we can compute the maximum score among all clique subgraphs. We now focus on computing the maximum score among all subgraphs with 5 nodes and one missing edge. A similar argument can be applied to subgraphs with fewer nodes.

Consider node $u$ in the recursive algorithm above. The hard case is when the missing edge is *not* incident to $u$, because then one node in the subgraph is not in $S_u$. Let nodes $a$, $b$, and $c$ be the nodes in the subgraph that are in $S_u$. To find the missing node $d$ that maximizes the score, we can note that nodes $a$, $b$, $c$, and $d$ form a clique. We can also assume that there is no edge connecting nodes $u$ and $d$, so the optimal choice for $d$ is independent of $u$. Therefore, we can find this node $d$ in $O(1)$ by precomputing all $O(n)$ cliques.

This solution runs in $O(n \log n)$ time.