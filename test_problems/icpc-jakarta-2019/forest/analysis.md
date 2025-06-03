The *centroid* of a tree is a node whose removal causes the remaining tree(s) to have at most half the number of nodes of the original tree. A tree can only have at most $2$ centroids.

Recall that removal of a good cutting point (as defined in the problem statement) will cause at least two identical disconnected trees. Observe that each of those disconnected trees cannot have more than half the number of nodes of the original tree. Therefore, if a good cutting point exists in the given tree, then it must be at the centroid of the tree.

Tree centroid can be found in $O(N)$—alternatively, an $O(N \log N)$ or $O(N^2)$ if implemented efficiently, can also be used as $N$ in this problem is quite small.

The remaining task is to check whether the disconnected trees are identical (also called *isomorphic*). Rooted tree isomorphism can be solved in $O(N)$ with parenthetical tuples (AHU algorithm), although an easier $O(N^2)$ version suffices. For this problem where the trees are unrooted, simply use the centroid of each tree as the root. You need to be careful when the tree has $2$ centroids. If you opt to test all nodes in each tree as the root for tree isomorphism, then you will need the $O(N)$ algorithm to do the tree isomorphism.

Note that using the *center* of a tree (as opposed to centroid) as the only candidate for the good cutting point will not work (**WRONG-ANSWER**). The center of a tree is a node that has the lowest eccentricity (or lies in the diameter path of a tree).