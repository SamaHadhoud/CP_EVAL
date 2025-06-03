Let us solve the complementary problem: Count the number of permutations $P$ such that when a permutation $Q$ is defined as $Q_{P_i} = i$, $P_i = Q_i$ holds for all $i$.

We will show that for the above condition to be fulfilled, there must only be 1-cycles or 2-cycles in the permutation $P$:

- 1-cycles are such that $P_i = i$ for some $i$. It is clear when this is the case $Q_i = i$.
- 2-cycles are such that $P_{P_i} = i$ for some $i$. It can be checked by letting $P_i = k$ and $P_k = i$, we get $Q_k = i$ and $Q_i = k$, fulfilling the requirement.

Any permutation that does not consist fully of 1-cycles and 2-cycles must have a cycle of length of at least 3. This means that there exists a tuple $(a, b, c), a \ne c$, such that $P_a = b$ and $P_b = c$. When performing the operation on such a permutation, $Q_b = a \ne c = P_b$, these permutations do not satisfy the requirements of the problem.

We can use Dynamic Programming to count the number of permutations with only 1-cycles and 2-cycles. Let $DP[N]$ be the number of such permutations with length $N$.

- We can add a 1-cycle on top of a fulfilling permutation by appending the next missing element at the end of the permutation. That is, the permutation will be in the form of $[0, 1, \ldots, N - 2](N - 1)$ where the elements in square brackets represent a solved permutation. Hence, we have the transition  
  $DP[N] += DP[N - 1]$.

- We can add a 2-cycle on top of a fulfilling permutation of length $N - 2$. Let the 2-cycle be $(k, N - 1)$ for some $0 \le k < N - 1$. Then, the permutation can be in the form of $[0, 1, \ldots, k - 1](k + 1, \ldots, N - 2](k)$ where the elements in square brackets when joined represent a solved permutation. There are $N - 1$ possible choices of $k$, so we have the transition  
  $DP[N] += (N - 1) \times DP[N - 2]$.

The answer to the original problem is simply $N! - DP[N]$.
