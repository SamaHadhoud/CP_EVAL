Here, we’ll only describe how to count *even-length* partitions of $n$ such that $a_1 < a_2 > a_3 < a_4 > a_5 < \dots$.  
There are only a few other kinds of partitions and they can all be handled similarly. Also, let $N := 300\ 000$.

Instead of counting partitions satisfying $a_1 < a_2 > a_3 < a_4 > a_5 < \dots$, it’s easier to count partitions satisfying $a_1 < a_2 \le a_3 < a_4 \le a_5 < \dots$. Let’s call even-length partitions satisfying the latter *almost increasing*. Once we have computed the latter, the former can be computed by inclusion-exclusion. Specifically, we’re counting partitions satisfying $a_1 < a_2\ ?\ a_3 < a_4\ ?\ a_5 < \dots$ where each “?” is either “no condition”, or “$\le$”, and in the latter case, the whole term is weighted by $-1$ (because of inclusion-exclusion).

Formally, let the *weight* of an almost-increasing partition be $(-1)^k$ where $k$ is the number of times “$\le$” appears, and let $a(n)$ be the sum of the weights of all almost-increasing partitions of $n$. Then every partition satisfying “$a_1 < a_2\ ?\ a_3 < a_4\ ?\ a_5 < \dots$” is just a conjunction of some number $k$ of almost-increasing partitions. Thus, the number $c(n)$ of partitions of $n$ satisfying $a_1 < a_2 > a_3 < a_4 > a_5 < \dots$ is

$$
c(n) = \sum_{k=0}^{\infty} \sum_{n_1 + \dots + n_k = n \atop n_i \ge 0} a(n_1)a(n_2) \dots a(n_k),
$$

where the inner sum is actually a $k$-fold sum. The inclusion-exclusion is handled by the weights. Now, notice that this is a convolution and can be stated nicely in terms of generating functions! Let $A(x) := \sum_{n \ge 0} a(n)x^n$ and $C(x) := \sum_{n \ge 0} c(n)x^n$. Then the above translates to

$$
C(x) = \sum_{k=0}^{\infty} A(x)^k = \frac{1}{1 - A(x)}.
$$

Thus, if we’re able to compute $A(x)$, we’re also able to compute $C(x)$ with a single generating function inverse in $O(N \log N)$.

The values $a(n)$ for $n \le N$ can be computed with DP by noticing that for an almost-increasing partition $a$ of $n$, $a_i \ge i/2$ for all $i$, and thus its length is $O(\sqrt{n})$. Thus, we can compute them with DP by finding a recurrence for $a(n, \ell)$, the sum of the weights of almost-increasing partitions with sum $n$ and length $\ell$. Because $n \le N$ and $\ell \le 2\sqrt{N}$, with an $O(1)$ transition this gives an $O(N^{1.5})$-time algorithm.