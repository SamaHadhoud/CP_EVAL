**Observation C.1.** *For a non-negative integer $y$, if $p(y) - p(y + 1) = t \geq 0$, the rightmost $(t + 1)$ bits in the binary representation of $y$ are all 1. Additionally, the $(t + 2)$-th bit of $y$ from the right is 0.*

For example, consider $y = 87 = (1010111)_2$. Here, $t = p(y) - p(y + 1) = 5 - 3 = 2$. As the observation states, the rightmost three bits of $y$ are 1, and the fourth bit of $y$ from the right is 0.

Let $k$ be the smallest index such that $a_k = \max(a_1, \ldots, a_n)$. Also, let $x$ denote the solution to this problem.

When $k < n$, let $t = a_k - a_{k+1}$. It’s worth noting that $t \geq 0$ according to the definition of $k$. Based on the previous observation, the binary representation of $x + k - 1$ should be like

$$
x + k - 1 = (\ldots 0\underbrace{1\ldots1}_{t+1})_2.
$$

The bits starting from the $(t+3)$-th position from the right of $x + k - 1$ cannot be determined solely from this observation. However, considering that $a_k$ is the largest value among $a_1, \ldots, a_n$, for $s = a_k - (t + 1)$,

$$
x + k - 1 = (\underbrace{1\ldots1}_{s}0\underbrace{1\ldots1}_{t+1})_2.
$$

is the smallest possible value for $x + k - 1$. Consequently, we can compute the smallest possible value of $x$.  
If any of the following conditions hold: $s < 0$, $x < 0$, or $p(x + i - 1) \ne a_i$ for some $i$, then the solution does not exist.

Similarly, when $k = n$, the smallest possible value for $x + n - 1$ is $2^n - 1$. The runtime complexity is $\mathcal{O}(n)$.