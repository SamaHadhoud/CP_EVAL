Let $S_{L,R}$ be the set of locations of piplups, if we only consider the piplups from days $[L, R]$. Let’s first try to generate this set for all queries quickly for each query.

**Observation 1:** Firstly, note that the operation (piplups going to iceburgs) is associative. I.e. instead of processing the piplups one by one from left to right, we can first process the left half, $S_{L,M}$ and the right half, $S_{M+1,R}$ and merge them together. The merging can be done simply in $\mathcal{O}(R - L)$ via a merge sort like operation. This motivates a segment tree like solution.

This can be done by constructing a normal segment tree on the range of $[1,N]$ and in each node storing $S_{s,e}$. Note that this takes $\mathcal{O}(N \log N)$ to construct. Unfortunately, for a $[L, R]$ query, it still takes $\mathcal{O}(N)$ to generate $S_{L,R}$ as the size of the sets may be up to $\mathcal{O}(N)$ and merging will take $\mathcal{O}(N)$.

**Observation 2:** Note that for a set $S_{L,R}$, it consists of consecutive segments of iceburgs where the starting iceburg of the segment is $\le \max A_i$. Assume otherwise, it means that there exists a segment that start at $X > \max A_i$, which means that iceburg $X - 1 \ge \max A_i$ is empty. In which case, consider the piplup at $X$ it should have started at a position $\le \max A_i$, and thus stop at $X - 1$ instead of $X$ which is a contradiction.

Thus, instead of storing all the positions, we can reduce the size of the set to be $\mathcal{O}(\max A_i)$ and simply store the segments $(\texttt{StartIdx}, \texttt{Size})$. The merging operation should still be similar but take $\mathcal{O}(\max A_i)$.

Finally to answer the queries we can simply generate $S_{L,R}$ in $\mathcal{O}(\max A_i \times \log N)$ then iterate through the set until we encounter $X_i$ empty iceburgs, which can be done in $\mathcal{O}(\max A_i)$. For updates, we can do a normal segment tree update and regenerate the node sets in $\mathcal{O}(\max A_i \times \log N)$ as well.

Final complexity: $\mathcal{O}(N \log N + M \log N \times \max A_i)$

**Note:** Alternative solutions that simply involves a min+index segment tree can also pass but may require some constant time optimisations.
