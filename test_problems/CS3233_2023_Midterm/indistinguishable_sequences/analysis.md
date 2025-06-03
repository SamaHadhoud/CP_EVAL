Let’s make a few observations. Since the test cases are random, we can generate them on our own to test whether our solution is fast enough.

First, we only need to consider elements which can be a part of some LIS (longest increasing subsequence). If an element $a_i$ is never part of any LIS, then deleting it will not affect the length of the LIS.

So, how can we determine whether an element could be part of a LIS? Let $\text{lis}_1(x)$ be the longest increasing subsequence on the prefix $[1, x]$ which ends at $x$, and $\text{lis}_2(x)$ be the longest increasing subsequence on the suffix $[x, n]$ which starts at $x$. We can compute both of these using the classic $O(n \log n)$ dynamic programming for LIS.

Suppose $L$ is the length of the LIS. An element $x$ can be part of some LIS if and only if $\text{lis}_1(x) + \text{lis}_2(x) = L + 1$ (since we count the index $x$ twice). If this equality is not satisfied, then that element can never be part of any LIS. After we have this, we delete all elements that are never part of any LIS, and now solve the problem on the reduced array $a'$. After we find the number of subsequences in $a'$, we can simply multiply that answer by $2^d$, where $d$ is the number of deleted elements.

Now, in the reduced array $a'$, how do we count the number of subsequences with the same LIS length? Notice that a LIS $(i_1, i_2, \ldots, i_L)$ have $\text{lis}_1(i_j) = j$. This motivates us to partition these elements into _levels_. Specifically, level $l$ will contain all elements with $\text{lis}_1(i) = l$. After we have these levels, we add edges between levels $l$ and $l + 1$ if $a_i < a_j$ where $i \in \text{level}(l)$ and $j \in \text{level}(l + 1)$.

On a random permutation with $n = 10^5$, it turns out that the size of $a'$ is at the very most only around $\approx 4000$. In addition, the maximum number of elements in a single level is at most around $\approx 20$. Hence, we can try to use a bitmask dynamic programming on the level graph structure.

Denote $\text{dp}(l, m)$ be the number of subsets of elements in levels $< l$, where the $b$-th bit of $m$ is 1 if and only if there exists a LIS ending at $\text{level}(l, b)$, and 0 otherwise. This state contains enough information, as a LIS must go through the levels sequentially in order $1, 2, \ldots, L$.

Now, suppose we fix $m$ in $\text{dp}(l, m)$. Which states $m'$ in $\text{dp}_{l+1}(m')$ can it transition to? Let $m''$ denote the bitmask of the maximal set of elements in $\text{level}(l + 1)$ where for every element $j \in m''$, exist $i \in m$ such that $a_i < a_j$. In other words, $m''$ is the maximal set of elements where we can potentially extend the LIS.

Now that we have $m''$, the possible transitions $m'$ are those that satisfy $m' \subseteq m''$. Furthermore, for each $m'$, we contribute by $\text{dp}(l, m) \times 2^{|\text{level}(l+1)| - |m'|}$, since we can leave the $|\text{level}(l+1)| - |m'|$ elements either in or out of the subsequence without affecting which elements have a LIS ending at them.

If we iterate the subsets $m' \subseteq m''$ naively, this runs in $O(3^k)$, where $k = |\text{level}(l + 1)|$ However, we can further optimize this with the sum over subsets dynamic programming to $O(k2^k)$. We can also optimize finding $m''$ by using bitmasks as an “adjacency matrix” (i.e. the $j$-th bit of $\text{adj}[i]$ is on iff there is an edge $(i, j)$) to $O(k2^k)$ as well. In total, we can transition from layer $l$ to $l + 1$ with time complexity $O(k2^k)$.

In practice, there are very few test cases where this solution is not fast enough. We checked over $10^6$ random permutations with $n = 10^5$ and only found 14 permutations with maximum layer size 24 or more. As there are only $\approx 200$ finalized test cases for this problem, the probability that this happens is very small.

**Challenge:** Can you prove the asymptotic bounds on the maximum number of elements in a single level? What about the total number of elements across all levels?