Firstly, note that a set of quests can be completed if and only if it can be completed when we do the quests in increasing order of their $m_i$. Thus we will sort the quests by their $m_i$ value. We shall also interchangeably use the terms "trauma rating" and "cost".

We first discuss an initial idea, that gives us a polynomial time solution at least.

If the optimal solution uses at most $K$ quests, then we can run a solution that takes the trauma rating as the sum of all quests done rather than just the $K$ largest. Let $dp[i][j][k]$ be the highest level we can reach using the first $i$ quests, if the sum of their trauma levels is $j$, and we have done exactly $k$ quests. The answer is then the minimum $j$ such that there exists $k$ with $dp[n][j][k] \ge M$.

Now note that if we do more than $K$ quests, then everything after the $K$-th highest trauma level is basically free. Hence let’s set a cutoff level for where the quests start being free, and we want to do **exactly $K$** non-free quests.

We iterate on $c = 1, \ldots, N$, and let the $c$ quests with the smallest trauma level be free (set $T_i = 0$). Now we just run a very similar DP to the above: let $dp[i][j][k]$ be the highest level we can reach using the first $i$ quests, if the sum of their trauma levels is $j$, and we have done exactly $k$ non-free quests. We update the answer with the minimum $j$ such that $dp[n][j][K] \ge M$, for each $c$. This solution runs in $\mathcal{O}(N^4 \cdot \max(T_i))$, which is too slow.

We try to optimise this by removing the last dimension in the DP state. In other words, we try to find a way that lets us not keep track of exactly how many free quests we have done. Note that if we use more than $K$ free things, we get a suboptimal answer, so we just need to deal with the case when we use less than $K$ free quests.

If we do less than $K$ free quests, then we are ignoring the fact that we’ve treated cheaper quests as free. So when we make the $c$ quests with smallest costs be free, let $C$ be the $c$-th smallest cost. We will assume we have already incurred an initial cost of $K \cdot C$, and now we replace all $c_i$ by $\max(0, c_i - C)$.

Now we define $dp[i][j]$ as the highest level we can reach using only the first $i$ quests, such that the sum of their transformed costs is $j$. We can update the answer with $\min(\text{ans}, K \cdot C + j)$, where $j$ is the minimum number such that $dp[i][j] \ge M$. This solution runs in $\mathcal{O}(N^3 \cdot \max(T_i))$ time, which is sufficiently fast.

Why this solution works is left as a simple exercise to the readers.
