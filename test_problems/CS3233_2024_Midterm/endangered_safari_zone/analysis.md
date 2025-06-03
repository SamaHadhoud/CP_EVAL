### No Updates

First let us consider how we can solve the question without any updates.

Let’s assume we are currently trying to answer query $[L, R]$. We can ignore the elements after $R$ and only consider elements in $[1, R]$.

Main Idea: For each distinct element, we want to $+1$ but if the number of distinct elements is $> K$ then $-1$.  
Thus, we can $+1$ at the last location of each value and $-1$ at the $(K+1)^{th}$ last location of each value.  Thus querying the range sum of $[L, R]$ would get our answer.

However, if we extend this idea to all queries, we notice that the last and $(K + 1)^{th}$ last location of each value differ based on the value of $R$. We need to be able to update this easily.

Since the queries are all given to us offline. We can sort the queries in increasing $R$ and incrementally add an element to the back to increase $R$. If we precompute for each element where is the next element with the same value, as well as keep track of the current location of the $+1$ and $-1$ for each value. The location of the $+1$ and $-1$ can be very easily updated.

Thus, to answer all the queries we can simply use a fenwick tree or segment tree to update the locations of the $+1$ and $-1$ and when we reach $[1, R]$ we can answer queries that endpoint is $R$ with a range query on the fenwick/segment tree on range $[L, R]$. Thus in total this takes $\mathcal{O}((Q + N) \log N)$

### Small Number of Updates

Now to handle updates, let’s first assume there are very little updates, $\mathcal{O}(H)$, we can consider handling the updates individually. Firstly, let’s define the original array values as $a_i$. And the $t_i$ be the index of the $i^{th}$ update in the original list of updates and queries. (Note that $t_{i+1}$ may be $\ne t_i + 1$ as there may be an query in between the updates) Finally, let $v_i$ and $p_i$ be the new value and location of change for an update.

For every position of the array, we position a index as mutable if there ever exist a update at that position and not mutable otherwise. For each mutable position $x$, let the indices of update that affects this position be $\{u_0, u_1, \ldots, u_k\}$. We will attempt to count the occurrences of values in these mutable positions.

We can then remove the value $a_{p_i}$ from the array and construct a history of values of the position. Constructing a list of $\{(x, a_x, 0, t_{u_0} - 1), (x, v_{u_0}, t_{u_0}, t_{u_1} - 1), \ldots, (x, v_{u_k}, t_{u_k}, \infty)\}$. This represents the (location, value, start time, end time). For each query $[L, R]$ with index $i$, we can then iterate through a combined list of these history of changes and for those with $t_{start} \le i \le t_{end}$ and $L \le x \le R$, we can add 1 to that value in a frequency table. Hence, we have a frequency table of elements in mutable positions in our query range. And if an element appears at least 1 and at most $K$, we can +1 to our sum. This takes $\mathcal{O}(H)$ per query as we are effectively iterating through all queries.

To handle non-mutable positions, we can use the same fenwick/segment tree method to count the number of valid distinct elements in the range. However, we need to consider the case where an element appears in both our mutable location frequency list and in a non-mutable position. For example, if $K = 3$ and a number appear 2 times in a mutable position and 2 times in a non-mutable position. We will count this number twice, and need to fix the answer by $-2$ since it appears a total of 4 times and should not be counted at all.

Thus, we need to somehow know for a number how many it appears in the non-mutable positions. For each number we can keep a list of positions that it appear in, this is simple to update as when we increment the $[1, R]$, we can simply push $R$ into the back of list of value $a_R$. Then for a value $v$ in the mutable frequency table, we can then binary search the number of elements $\ge L$ in the list of value $v$. This will be the number of times value $v$ appear in non-mutable positions in $[L, R]$. We can then update our query answer accordingly.

As such, we have successfully answered our query in $\mathcal{O}((Q + N) \log N) + \mathcal{O}(Q(H + H \log N))$. The time complexity of the original fenwick/segment tree solution $+ \mathcal{O}(H)$ to iterate the history list and $\mathcal{O}(H \log N)$ to fix the answer. Leading to $\mathcal{O}((Q + N) \log N)$

### Many Updates

We cannot however, use the above solution if there are many updates. However, we can separate the list of queries and updates into blocks of size $H$ to limit the number of updates by $H$ and run the solution once on each block!

Thus, for the first block we can perform the above solution on the queries and updates in the first block. For the second block however, we still need to apply the updates in the first block. Thus, we can directly apply the updates in the first block and change the values in $a$. Performing the solution on the second block with this new array. Repeat this for all subsequent blocks, rebuilding the original array of values.

Since there are at most $\frac{H}{Q}$ queries in each block. Our solution will take $\mathcal{O}((H^2 + N) \log N)$ for each block. There are $\mathcal{O}(\frac{Q}{H})$ blocks leading us to a complexity of $\mathcal{O}((QH + \frac{Q}{H}N) \log N)$. By AM-GM, the optimal value of $H$ is actually $\mathcal{O}(\sqrt{N})$ thus getting a complexity of $\mathcal{O}(Q\sqrt{N} \log N)$ which passes.
