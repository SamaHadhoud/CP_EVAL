Let $n$ be the number of nodes and $m$ be the number of edges that are shared between the two trees, the answer is then $n - 1 - m$.

**Proof:** Consider the following algorithm while there exists edges that are not shared between the two trees.

1. Delete an arbitrary edge from Tree 1 that is not in Tree 2

2. The graph is now divided into exactly two trees $(S, T)$

3. Select any edge from Tree 2 which has exactly one vertex in $S$ and one in $T$

4. Add this edge from step 3

Note that step 2 is true, since by the operation given, we have the invariant that the graph remains a tree after each operation. And also we can always find an edge from tree 2 in step 3 that fulfill the conditions since otherwise Tree 2 would be disconnected. Notice that after this operation, the number of shared edges between the graph and Tree 2 increases by 1. Hence the algorithm will perform the operation $n - 1 - m$ times.
