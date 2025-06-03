This problem can be solved with dynamic programming. The following analysis assumes the tree is rooted (simply select an arbitrary node as the root).

First, let us define a state $\langle u, s \rangle$ where $u$ is a node and $s$ is either $\{0, 1, 2\}$ as follows:

- $\langle u, 0 \rangle$ – node $u$ and its parent are cleaned by a different robot.
- $\langle u, 1 \rangle$ – node $u$ and its parent are cleaned by a different robot, and node $u$ should be cleaned together with its $2$ direct children.
- $\langle u, 2 \rangle$ – node $u$ and its parent are cleaned by the same robot.

Let a function $f(u, s)$ be the number of feasible deployment plans on a subtree rooted at node $u$. Then, the solution to this problem is simply $f(root, 0)$.

With these state and function definitions, we can derive the recurrence relation between states. Let $\eta^u$ be the set of node $u$’s children, $\eta^u_i$ be the $i^{th}$ child of node $u$, and $n$ be the number of node $u$’s children (i.e. $|\eta^u|$). The recurrence relation is:

$$
f(u, s) =
\begin{cases}
h_1(u,1) + h_2(u) + h_3(u) & \text{if } s = 0, \\
h_2(u) & \text{if } s = 1, \\
h_1(u,0) + h_3(u) & \text{if } s = 2
\end{cases}
$$

$$
h_1(u, s) = \sum_{i = [1..n]} \prod_{v \in \eta_u}
\begin{cases}
f(v, 2) & \text{if } v = \eta^u_i \\
f(v, s) & \text{if } v \ne \eta^u_i
\end{cases}
$$

$$
h_2(u) = \sum_{i = [1..n]} \sum_{j = (i..n)} \prod_{v \in \eta_u}
\begin{cases}
f(v, 2) & \text{if } v = \eta^u_i \text{ or } v = \eta^u_j \\
f(v, 0) & \text{if } v \ne \eta^u_i \text{ and } v \ne \eta^u_j
\end{cases}
$$

$$
h_3(u) = \prod_{v \in \eta_u} f(v, 1)
$$


The function $h_1(u, s)$ handles the case where we should choose one child to be cleaned together with node $u$ while the remaining other children are in state $\langle \eta^u_i, s \rangle$. The function $h_2(u)$ handles the case where we should choose two children to be cleaned together with node $u$ while the remaining other children are in state $\langle \eta^u_i, 0 \rangle$. The function $h_3(u)$ handles the case where node $u$ is not cleaned together with any of its children.



Observe that the time complexity to compute $h_1(u, s)$ naively is $O(n^2)$, while $h_2(u)$ is $O(n^3)$, and $h_3(u)$ is $O(n)$. Therefore, the time complexity to compute the above recurrence relation is $O(N^3)$, which is too slow for this problem with $N \leq 100\,000$. Optimizing these functions is the real challenge for this problem.



**First good news:** The function $h_1(u, s)$ can be computed in $O(n)$.

$$
h_1(u, s) = \prod_{v \in \eta_u} f(v, s) \times \sum_{v \in \eta_u} f(v, 2) f^{-1}(v, s)
$$

Note that we need to use the multiplicative modular inverse to compute $\frac{1}{f(v, s)}$ as we only need to find the modulo while the full number can be very large.

Another good news: The function $h_2(u)$ can be computed in $O(n)$.

$$
x^u_i = f(\eta^u_i, 2) f^{-1}(\eta^u_i, 0)
$$

$$
y^u_i = \sum_{j = [i..n]} x^u_j
$$

$$
h_2(u) = \prod_{v \in \eta_u} f(v, 0) \times \sum_{i = [1..n]} x^u_i y^u_{i+1}
$$


Note that $y^u_i$ can be computed in $O(n)$ for all $i$ with partial sum technique, making it $O(1)$ for each $i$. Also note that $y^u_i = 0$ if $i > n$. The original $h_2(u)$ function is actually in the form of $a_1 a_2 b_3 b_4 b_5 + a_1 b_2 a_3 b_4 b_5 + a_1 b_2 b_3 a_4 b_5 + \cdots + b_1 b_2 a_3 b_4 a_5 + b_1 b_2 b_3 a_4 a_5$ (an example where $n = 5$), i.e. there are 2 functions, $a$ and $b$, choose two indexes to have $a$ while the remainings are $b$; evaluate each combination (perform the multiplications) and sum the results for all the possible combinations. There are $\binom{n}{2}$ combinations, and evaluating one combination is $O(n)$, thus, the original time complexity is $O(n^3)$. Try to optimize this summation into $O(n)$ and you will arrive at the above equations.


We do not need to optimize the function $h_3(u)$ as it is already in $O(n)$. All seems good! The solution now becomes $O(N)$. However, there is a caveat in the implementation: Beware when you want to compute the modular multiplicative inverse of zero (there is no such thing). You’ll need to further work on the equations on such a case. 

**Hint:** We only need to consider the cases when there is at most $1$ zero for $h_1(u)$ and $2$ zeros for $h_2(u)$.
