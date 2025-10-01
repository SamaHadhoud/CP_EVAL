For simplicity, we will use graph theory terminology instead of the definitions given in the problem statement. Note that acceptability is equivalent to ensuring that there is no path of length $K$.

Let $f_i$ ($1 \leq i \leq N$) be the maximum path length ending at vertex $i$, and let $b_i$ ($1 \leq i \leq N$) be the maximum path length starting from vertex $i$. The values of $f_i$ satisfy the following recurrence relation, where $E$ denotes the set of edges:

$$
f_i = \left\{
\begin{array}{ll}
1 + \max_{(j, i) \in E} f_j & \exists (j, i) \in E \\
0 & \text{otherwise}
\end{array}
\right.
$$

Thus, the initial values of $f_i$ can be computed in $\mathcal{O}(N + M)$ time by processing vertices in ascending order. Similarly, the initial values of $b_i$ can be computed in descending order.

If we can efficiently maintain the values of $f_i$ and $b_i$, checking whether a new edge can be added becomes straightforward using the following lemma:

**Lemma.** *A new edge $(u, v)$ can be added if and only if $f_u + b_v + 1 < K$.*

**Proof.** Exercise.

The key challenge is efficiently updating $f_i$ and $b_i$ when a new edge $(u, v)$ is added. We describe the update process for $f_i$ (the update for $b_i$ follows analogously).

Given the way $f_i$ is initially computed, we can derive an update algorithm using a simple DFS, as shown in the following pseudocode:

```python
Function update(i, val):
    f[i] = val
    for each edge (i, j):
        if f[j] < f[i] + 1:
            update(j, f[i] + 1)

Function add_new_edge(u, v):
    if f[v] < f[u] + 1:
        update(v, f[u] + 1)
```

This recursive algorithm appears to require $\mathcal{O}(N + M)$ time per edge addition, leading to a total complexity of $\mathcal{O}(Q(N + M))$. However, we observe that the function update is called at most $(K - 1)N$ times in total. This is because:

- Every call of the function increases some $f_i$ by at least one

- The sum of all $f_i$ values never exceeds $(K - 1) \times N$, as the problem requires $f_i \leq K - 1$ to hold for any $i$ at all times

- The values of $f_i$ never decrease

Hence we can solve this problem in the overall time complexity of $\mathcal{O}(Q + K(N + M))$.

