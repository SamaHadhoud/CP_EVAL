The solution uses a combination of dynamic programming and greedy. Let $S$ be the number of characters ‘.’ (soils) in the input, and $R$ be the number of characters ‘#’ (rocks) in the input. Note that $R$ is at most $50$.

**Observation 1**  
Since type-3 tiles can only be placed on “.#.”, then the maximum possible number of type-3 tiles that can be placed is $R$ (which is at most $50$).

**Observation 2**  
Because we want to maximize the result, any placed type-2 tile can be replaced by two type-1 tiles (when $G_1 * 2 > G_2$), or one type-1 tile (when $G_1 > G_2$ and doing the former would exceed the limit of $K$).

The solution uses dynamic programming by assuming that we can only use type-2 and type-3 tiles. Type-1 tiles can then be inserted greedily using Observation 2.

Define $dp[i][j]$ be maximum number of type-2 tiles that can be used for tiling cells $[1..i]$, using exactly $j$ type-3 tiles. The recurrence is trivial: For each state, decide whether we want to leave the $i^{th}$ cell untiled, put type-2 tile, or put type-3 tile.

After we fill our DP table, we can brute-force $j$, the number of type-3 tiles used. For each $j$, we know that:

- The number of type-2 tiles used is $dp[N][j]$
- The number of type-3 tiles used is $j$
- The number of untiled soil cells is $S - 2 * dp[N][j] - 2 * j$
- We can put type-1 tiles on the untiled cells. Then, if we still have spare type-1 tiles left, we can greedily replace some of the type-2 tiles, using Observation 2.

The answer is the maximum result over all $j$. The time complexity is $O(NR)$.
