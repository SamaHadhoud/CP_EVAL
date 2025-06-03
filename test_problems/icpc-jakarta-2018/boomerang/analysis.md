If the graph is not connected, we can run the boomerang partition algorithm independently for each of the connected components. Therefore, the rest of the discussion assumes that the graph is connected.

For now, assume that the graph has even number of edges. Since the boomerangs must be disjoint, and each boomerang uses two edges, there are at most $\left\lfloor \frac{|E|}{2} \right\rfloor$ boomerangs that we can find. It turns out that we can find $\left\lfloor \frac{|E|}{2} \right\rfloor$ boomerangs (i.e. a partition of boomerangs), thus finding the optimal number of boomerangs.

We can prove this by induction. If we have a connected graph with 2 edges, we can obviously find a partition of boomerangs. Also, we will prove that we can find a partition of boomerangs on a connected graph with $x$ (even) edges if we can find a partition of boomerangs on a connected graph with $y$ (even) edges for all $y < x$.

Suppose we have a connected graph $G$ with an even number of edges. There are two cases:

- Suppose there is no bridge in $G$. We can always find a boomerang in $G$ where removing the boomerang edges in $G$ causes either $G$ to remain connected, or to be separated into two connected components, each having an even number of edges.

- Suppose there is a bridge $(u, v)$ in $G$. Let $U$ and $V$ be the two connected components if we remove the edge $u, v$, with $u$ in $U$ and $v$ in $V$. Since $G$ has even number of edges, exactly one of $U$ or $V$ must have an even number of edges. Without loss of generality, suppose $U$ has an even number of edges. We can choose a node $w$ in $V$ such that removing the edge $(v, w)$ causes $V$ to remain a connected component, or $V$ to be separated into two connected components, each having an even number of edges.

In both cases, we can find one boomerang, and removing them from $G$ may cause $G$ to be separated into several connected components. However, since we can find a partition of boomerangs in each of the connected components, we can find a partition of boomerangs in $G$.

Implementing the induction above using recursion is tricky—we need a data structure that returns a bridge in an online manner. Fortunately, there is a simpler algorithm. To make the discussion easier, we will first find a partition of boomerangs on a tree with an even number of edges.

Let $T$ be a tree with an even number of edges. While there is an edge on $T$, we do the following:

- Suppose there is a node $u$ having two children $v$ and $w$ which are leaves. We can create a boomerang $(v, u, w)$ and remove these edges.

- Otherwise, we can always find a node $u$ with only one child, and that one child is a leaf. We can create a boomerang $(child(u), u, parent(u))$ and remove these edges.

Since the rest of the tree is still connected, we can still find a partition of boomerangs. We can implement this in $O(N)$ time using DFS.

We can convert the original graph $G$ into a tree while keeping the number of edges by adding the edge in $G$ one by one. If adding an edge $(u, v)$ creates a cycle, we add an edge $(u, w)$ instead, where $w$ is a new node. However, when we print the solutions, keep in mind that $w$'s original ID is actually $v$. This algorithm runs in $O(N \log N)$.

The last piece of the puzzle is to figure out what to do if we have an odd number of edges. We can simply discard any one of them, since an additional one edge is not enough to create another boomerang.