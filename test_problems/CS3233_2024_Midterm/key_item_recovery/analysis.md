Let’s remove node 1 from the tree. This will split the tree into $1 \leq K$ subtrees.

Note that if $A_{i,j} > 1$ then nodes $i$ and $j$ are in the same subtree. Since nodes in different subtree will have to pass through town 1 in the original tree and having a minimum node of 1. Now we have a way of identifying if two nodes are in the same subtree.

Thus we can the set of nodes of each subtree, by simply iterating through the nodes, and then checking against a node in each subtree set. If it belongs to a subtree set, add it to the set, else if it does not belong to any set then create a new subtree set.

Note that we can then abitrarily choose a node from each subtree set, let this be $T_i$ for $i^{th}$ subtree, and add edge from that node to 1. This is valid as for $x, y$, if $x$ and $y$ belong in the same subtree, they will not use this edge to 1, thus they are not affected by what $T_i$ is. If $x$ and $y$ are in different subtree ($i$ and $j$), then their path will involve $x \rightarrow T_i \rightarrow 1 \rightarrow T_j \rightarrow y$. Thus regardless of what $T_i$ and $T_j$ is, it does not affect the result that $A_{x,y} = 1$.

Hence, we can construct the edges of node 1. We can then do this recursively on the smallest node in each subtree and eventually construct the entire graph.

Note that the time complexity of this recursively algorithm is $O(N^2)$. Proof is left as an exercise to the reader. Hint: Consider a subtree to be represented as an edge and how many times is that subtree set being compared against.
