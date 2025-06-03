Note that an absolutely symmetric matrix has the same parity on $(i, j)$ and $(j, i)$. Therefore, if the parity of $A_{i,j}$ and $A_{j,i}$ is different, the answer is $-1$.

If the initial matrix is already absolutely symmetric, the answer is $1$ (the matrix itself). Otherwise, the answer is at least $2$. Note that the condition on $(i, j)$ is only dependent on $(j, i)$. So, let’s work on only these two values – denoted $x = A_{i,j}$ and $y = A_{j,i}$.

Let $a = \frac{x + y}{2}$ and $b = \frac{x - y}{2}$. Notice that $a + b = x$ and $a - b = y$. So we can set $X_{1, i, j} = a$, $X_{1, j, i} = a$, $X_{2, i, j} = b$, and $X_{2, j, i} = -b$. By doing this for all $i, j$, we can decompose $A$ into the sum of $2$ absolutely symmetric matrices.