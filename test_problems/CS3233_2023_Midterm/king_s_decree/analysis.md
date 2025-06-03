Note that we want to maximize the minimum value – hence, let’s try to binary search the answer. Suppose we want to test whether every cat can have at least $\lambda$ wealth. If $w_i \leq \lambda$, then cat $i$ needs $\lambda - w_i$ wealth. If $w_i > \lambda$, then cat $i$ can donate $w_i - \max\{l_i, \lambda\}$ wealth. We simply check if the amount of wealth which can be donated is greater than or equal to the amount of wealth that is needed. In other words, whether
$$
\sum \max\{0, \lambda - w_i\} \leq \sum \max\{0, w_i - \max\{l_i, \lambda\}\}.
$$