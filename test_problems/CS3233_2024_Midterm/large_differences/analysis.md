Sort the array and suppose $ a_1 \leq a_2 \leq \cdots \leq a_n $. Consider 2 cases.

**Case 1**: $ n = 2k $.

Note that for any $ k+1 $ elements of $ a $, there must be two which are adjacent. Thus among $ a_1, \ldots, a_{k+1} $, there are two of them which are adjacent. Thus there is an adjacent difference which is at most $ a_{k+1} - a_1 $.

By repeatedly using the same logic, we can conclude that there is an adjacent difference which is at most  
$ \min(a_{k+1} - a_1, a_{k+2} - a_2, \ldots, a_{2k} - a_k) $.

We can achieve equality with the permutation  
$ (a_{k+1}, a_1, a_{k+2}, a_2, \ldots, a_{2k}, a_k) $.

**Case 2**: $ n = 2k + 1 $.

We use the same idea: there can be at most one set of $ k+1 $ elements of $ a $ where no two are adjacent. As such, there is an adjacent difference which is at most the second smallest among  
$ (a_{k+1} - a_1, a_{k+2} - a_2, \ldots, a_{2k+1} - a_{k+1}) $.

We now show that equality can be achieved. Suppose the smallest is $ a_{k+i} - a_i $. Then at positions  
$ 1, 3, \ldots, 2k+1 $ we put $ a_i, a_{i+1}, \ldots, a_{k+i} $ in that order, and in positions  
$ 2, 4, \ldots, 2k $ we put $ a_{k+i+1}, a_{k+i+2}, \ldots, a_{2k+1}, a_1, a_2, \ldots, a_{i-1} $ in that order.
