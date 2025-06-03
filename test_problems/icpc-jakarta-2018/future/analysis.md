Let $M$ be the number of characters in the longest name given by Andi’s soon-to-be-grandmother-in-law (i.e. $M = \max(|S_i|)$).

This problem can be solved using dynamic programming. Let $dp(i, \text{mask})$ be the maximum possible total length of children names if we only consider $S_1, S_2, \ldots, S_i$ and we take the subsequence of $S_i$ as denoted by $\text{mask}$.

The recurrence is  
$$
dp(i, \text{mask}) = \text{popcount}(\text{mask}) + \min(dp(i - 1, \text{prev}))
$$  
where $\text{prev}$ are masks in the range $[0, 2^{|S_{i-1}|})$ that satisfy the condition that the subsequence of $S_i$ as denoted by $\text{mask}$ is lexicographically larger than the subsequence of $S_{i-1}$ as denoted by $\text{prev}$, and $\text{popcount}(\text{mask})$ is the number of active bits in $\text{mask}$.

However, this DP runs in  
$$
O(N \times M \times 2^M \times 2^M)
$$  
since it has $O(N \times 2^M)$ states and we need $O(M \times 2^M)$ time for the recurrence. This is too slow.

Let us improve this DP. First, we precompute all subsequences of each name and then sort them lexicographically with their corresponding mask. Then, we can find the largest $\text{prev}$ of $S_{i-1}$ that is still “less” than $\text{mask}$ of $S_i$ (here “less” means that the subsequence of $S_i$ as denoted by $\text{mask}$ is lexicographically larger than the subsequence of $S_{i-1}$ as denoted by $\text{prev}$), using binary search. We also store the prefix minimum of the previous DP row.

Now the recurrence roughly becomes  
$$
dp(i, \text{mask}) = \text{popcount}(\text{mask}) + \text{dpMin}(i - 1, \text{prev})
$$  
where $\text{prev}$ is found by binary search. This DP runs in  
$$
O(N \times M^2 \times 2^M)
$$

We can optimize this further by computing all values of $dp(i)$ using a two-pointers technique from $dp(i - 1)$, since the value of $\text{prev}$ we want from $dp(i, \text{mask})$ to $dp(i, \text{mask} + 1)$ never decreases. This optimizes the DP to run in  
$$
O(N \times M \times 2^M)
$$