A sacred configuration satisfies the following two conditions:
1. Each row contains at least one scarecrow.  
2. Each consecutive two columns contain at least one scarecrow.

Let $f(r)$ be the number of sacred configuration, considering only rows in the subset $r \in \{1, 2, \cdots, R\}$ (i.e., we want to leave all rows not in $r$ untouched).

This can be computed using a simple DP, where the state is $\langle \text{current column, does the previous column contain scarecrow} \rangle$. For the transition, we can:  
leave the current column untouched (only possible if the previous column contains scarecrows), or  
put one or more scarecrows in the current column. If the current column contains $k$ empty rows (considering only rows in $r$), then there are $2^k - 1$ ways.

Using the principle of inclusion-exclusion, the final answer is $\sum (-1)^{R - |r|} f(r)$

There are $2^R$ possible subsets of rows. The DP for computing $f()$ for each subset takes $O(C)$ time.  
When implemented efficiently, the final time complexity is $O(2^R \cdot C)$.