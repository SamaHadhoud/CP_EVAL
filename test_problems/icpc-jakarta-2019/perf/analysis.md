We first observe that, if there is an employee with better performance than Randall gets replaced in a particular year, then Randall must have also been replaced at the same time or before the particular employee is replaced. Therefore, for Randall to stay in the company after $M$ years, all employees (original and newly added) who have better performance than Randall must also stay in the company after $M$ years. We only need to check for each year, whether the number of employees with better performanc...

Let $R[i]$ be the number of employees not replaced subtracted by the number of employees with better performance than Randall plus Randall itself on that particular year. Construct the initial array and check whether the minimum value of $R[i]$ for all $i$ is greater than or equal to $0$.

For each query, there are 4 possible cases:

1. The employee is previously worse than Randall. After the change, the employee is still worse than Randall.
2. The employee is previously worse than Randall. After the change, the employee is now better than Randall.
3. The employee is previously better than Randall. After the change, the employee is now worse than Randall.
4. The employee is previously better than Randall. After the change, the employee is still better than Randall.

For case 1 and case 4, since there is no change in the number of employees better than Randall joining the company each year, the answer stays the same as before the query happens.  
For case 2, the number of employees in the company better than Randall increased by 1 starting from the next year after the change until the last year.  
For case 3, the number of employees in the company better than Randall decreased by 1 starting from the next year after the change until the last year.

We can use Segment Tree with Lazy Propagation to support our update and query requirement. The query type is range minimum query and the update type is range update of $+1$ or $-1$.

The time complexity will be $O(N + (M + Q) \log M)$.
