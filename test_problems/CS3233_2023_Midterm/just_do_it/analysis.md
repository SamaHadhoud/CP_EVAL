Let’s solve a simpler problem – suppose we have another array $b$ of size $n$, and the cost $c(l, r)$ of a subarray $[l, r]$ is $\min\{a_l, a_{l+1}, \ldots, a_r\} \times b_l$.

We fix the right endpoint $r$, and move it from $1$ to $n$. While we do that, we additionally maintain two other variables – `ans` which is the sum of costs of all subarrays in the subarray $[1, r]$, and `current` which is the sum of costs of all subarrays $[i, r]$ (with right endpoint $r$).

Additionally, we maintain a stack of tuples $(i, j, v)$, where it denotes that the value of $\min\{a_k, a_{k+1}, \ldots, a_r\}$ where $i \leq k \leq j$ is equal to $v$.

Now, suppose we have processed the right endpoint $r-1$, and we want to modify our variables to count the contribution of the right endpoint $r$.

- In our stack, while the top value $(i, j, v)$ of the stack has value $v \geq a_r$, we pop it. Before we pop it, however, we decrease `current` by $v \times \sum_{k=i}^{j} b_k$.
- Then, we push a new entry $(i, r, a_r)$ to the stack. After we push it, we increase `current` by $a_r \times \sum_{k=i}^{r} b_k$.
- Finally, we increase `ans` by `current`.

Essentially, `current` will always maintain the costs of the subarrays currently “living” in the stack. We need a range query in the procedure above, so we can use a segment tree to do it.

Now, back to the original problem. This time, we maintain both a minimum stack and a maximum stack. We also maintain two segment trees – one to maintain the minimum and maximum. The segment tree will have `segtree[k] = v`, for every entry $(i, j, v)$ with $i \leq k \leq j$ in the corresponding stack.

Roughly, our procedure is as follows:

- In our minimum stack, while the top value $(i, j, v)$ of the stack has value $v \geq a_r$, we pop it. Before we pop it, however, we decrease `current` by $v \times \texttt{segtreeMaximum.query}(i, j)$. Also, we now need to set all entries in the range $[i, j]$ in `segtreeMinimum` to $0$ (since the entry no longer “lives” in the stack), which we can do with a lazy segment tree with range sum update.
- Then, we push a new entry $(i, r, a_r)$ to the stack. After we push it, we increase `current` by $a_r \times \texttt{segtreeMaximum.query}(i, r)$. Also, we now need to set all entries in the range $[i, r]$ in `segtreeMinimum` to $a_r$, which we can also do with a range sum update. Note that at this point, all entries in $[i, r]$ in `segtreeMinimum` are currently set to $0$.
- Next, we do the same thing for the maximum stack.
- Finally, we increase `ans` by `current`.

There are a total of $O(n)$ updates and queries to the segment trees, so the time complexity is $O(n \log n)$.