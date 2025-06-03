The query in this problem is asking for the cost of the optimal meeting point for any given $K$ vertices on a tree (uniform cost). Given a set of $K$ vertices on a tree, the cost of a meeting point (vertex) is the total distance from each of the $K$ vertices to the meeting point.

In this editorial, we omit the proof for the solution and left it as an exercise to any interested reader.

The notion of optimal meeting point does not depend on whether the tree is rooted or unrooted, only the total distance matters. So, let's make the tree rooted as there are some calculations which can be done in a rooted tree, e.g., lowest common ancestor (LCA), which will be used in the solution. Simply choose any arbitrary vertex as the root.

$K = 1$ is a special case and trivial, thus, we remove the analysis for $K = 1$. The remaining of this editorial assumes $K \geq 2$.

Let $S$ be the set of $K$ vertices in the query. Let $T$ be the set of LCA vertices from any two vertices in $S$; there are $\mathcal{O}(K^2)$ pairs of vertices in $S$.

**Lemma-1:** $T$ contains an optimal meeting point for $S$.

Lemma-1 implies that we only need to check all the vertices in $T$ and find the one with the smallest cost. Checking the distance between two vertices in a tree can be done with LCA query, i.e. in $\mathcal{O}(\log N)$, or $\mathcal{O}(1)$ with a sparse table. Hence, evaluating a meeting point (a vertex in $T$) can be done in $\mathcal{O}(K)$, i.e. find the distances from the meeting point to all vertices in $S$.

Now, here is the cool part.

**Lemma-2:** $|T| < |S|$.

You can use mathematical induction to prove Lemma-2: Adding one vertex into $S$ will only add at most one extra vertex into $T$. Also, observe that $|T| = 1$ when $|S| = 2$.

With Lemma-2, finding the optimal meeting point can be done in $\mathcal{O}(K^2)$, instead of $\mathcal{O}(K^3)$.