Let’s first try to solve the problem without updates.

Let $R_i$ denote the endpoint of the $i^{th}$ contiguous segment. The array is then split up in to the following segments: $[1, R_1], [R_1 + 1, R_2], \ldots, [R_{K-1} + 1, R_K]$.

In this question we want to maximise $K$, while ensuring that sorting all the individual segments will sort the overall sequence. We call such a partition of the sequence a valid sequence.

**Observation 1**: A sequence is valid if and only if $[1, R_i]$ is a permutation of $1$ to $R_i$ for all $i$.

**Proof**:

- Assume partition is valid, then the first $R_i$ values of the sorted sequence is $1$ to $R_i$. However, as only the elements in the segments are sorted and the segments are not rearranged, the first $R_i$ elements in the final sequence is also the elements in $[1, R_1], \ldots, [R_{i-1} + 1, R_i]$. Thus, $[1, R_i]$ contains $1$ to $R_i$.

- Assume $[1, R_i]$ is a permutation of $1$ to $R_i$ for all $i$. Then $[R_{i-1} + 1, R_i]$ contains the elements from $R_{i-1} + 1$ to $R_i$. Sorting all of the segments individually, we get the sorted sequence of $1$ to $R_1$, sorted sequence of $R_1 + 1$ to $R_2$, etc. Thus merging them we get a permutation.

Let’s define an index $X$ as a valid index if $[1, X]$ is a permutation of $1$ to $X$.

Thus, to maximise $K$, we can find all valid indices and let then be an endpoint of a contiguous segment. And from Observation 1 we know this will be a valid partition. Thus, the maximum $K$ is equivalent to the number of valid indices. This can be calculated in $\mathcal{O}(n)$ with a for loop by keeping track of the running maximum elements. However, we need a faster way of calculating this to accommodate updates.

**Observation 2**: that $X$ is valid index if and only if number of $A_i > X$ in $[1, X]$ is 0.

Let this value be $V_X$ for index $X$. Note that if $X$ is not valid, then $V_X > 1$. Thus if we can maintain a set of values $\{V_1, V_2, \ldots, V_N\}$. Then our answer would be the number of $V_i = 0$. Since 0 would be the minimum number in the set. This resolves to a classic "count number of minimum element" in a segment tree.

To update our values of $V_i$, we can identify how the different values of $A_i$ affect the values of $V_i$.

- **Case 1**: $A_i \le i$  
  Has no impact on any $V_j$ since its only present in $[1, j]$ where $i \le j$. And since $A_i \le i \le j$, it will not be counted in $V_j$. Thus we can ignore this element.

- **Case 2**: $A_i > i$  
  Will be counted in $V_j$ for $i \le j \le A_i - 1$ since this is the only range where it is in the segment and $A_i > j$. This will attribute a range addition of $+1$ to the range $[i, A_i - 1]$.

Hence, we can maintain these updates on the segment tree and whenever there is a swap operation, undo the current updates of the two elements and add the new updates after the swap. The answer can then be obtained from just querying the segment tree.

Note that the initial building of the segment tree involves $\mathcal{O}(n)$ queries and each swap takes $\mathcal{O}(1)$ queries.  
Thus we get a final complexity of $\mathcal{O}(n \log n)$.