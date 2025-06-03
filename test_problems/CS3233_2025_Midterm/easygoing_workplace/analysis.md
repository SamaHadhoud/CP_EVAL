Model the employee hierarchy as a rooted tree in the obvious way, so the subtree of employee $i$ contains him and all his subordinates.

First note that we can make everyone do work with the order $1, 2, \ldots, N$, since every employee gets to the office before all their subordinates. We also observe that if employees go to the office in the order $N, N - 1, \ldots, 1$, every employee reaches after all their subordinates. We show that this achieves the minimum number of people doing work.

**Proof of claim:**

Let $m_u$ be the minimum possible number of employees doing work in the subtree of $u$ across all orderings (including $u$), and $s_u$ be the number of employees doing work in the subtree of $u$ in an ordering. We claim the aforementioned ordering will achieve $s_u = m_u$ for all $u$. We prove this by induction on the depth of the nodes, starting from the deepest nodes.

Since the deepest nodes are leaf nodes, they have no subordinates and thus will always work, so their $m_u = 1$ is achieved in any ordering. Now consider a node $u$, and let

$$
S = \sum_{\text{v child of u}} m_v
$$

Clearly, $m_u \ge S$. If $m_u = S$, then we must have $s_v = m_v$ for all children $v$ of $u$ in any ordering that achieves $s_u = m_u = S$. We also must not have $u$ do work, otherwise $s_u$ is at least $S + 1$. In an ordering that achieves $s_u = m_u = S$, the maximum number of subordinates of $u$ that can be doing work when it arrives to the office is $S$, so we must have $S > A_u$ if we don’t want $u$ to do work. In this case, since $u$ comes after all his subordinates in our proposed ordering, and by induction we have $s_v = m_v$ for all children $v$ of $u$, there will be $S$ subordinates of $u$ doing work when he arrives at the office, and thus $u$ will not do work. Then $s_u = S = m_u$.

Otherwise $m_u \ge S + 1$. Then in our ordering, $u$ has $S$ subordinates doing work when he arrives at the office (by induction), and including him, there are at most $S + 1$ employees in $u$’s subtree doing work. Thus $s_u \le S + 1 \le m_u$ and thus must be $m_u$ since $m_u$ is minimum by definition.

In either case, we’ve proven our claim by induction.

Now let the minimum possible employees doing work be $L$ and the maximum be $R = N$. We claim that for every $L \le x \le R$, we have an ordering where exactly $x$ employees do work.

Consider the following sequence of orderings:

- $o_1 = (1, 2, \ldots, N)$  
- $o_2 = (2, 3, \ldots, N, 1)$  
- $o_3 = (3, 4, \ldots, N, 2, 1)$  
- $o_4 = (4, 5, \ldots, N, 3, 2, 1)$  
- $\vdots$  
- $o_N = (N, N - 1, \ldots, 1)$

Let $W$ be the number of employees doing work in a given ordering. Note that the first ordering gives $W = R = N$ and the last gives $W = L$. We claim that $W$ either stays the same or decreases by 1 when we change the ordering from $o_i$ to $o_{i+1}$ for each $1 \le i \le N - 1$.

---

**Proof of claim:**

From $o_i$ to $o_{i+1}$, employees $i + 1, \ldots, N$ still arrive before all subordinates, so they will continue to do work. Employee $i$ now arrives after all his subordinates, but still before all his superiors. If $i$ continues doing work, then there is no change in $W$ at all. Otherwise, if $i$ stops doing work, some of his superiors who weren’t working may start doing work. However, we can show that at most one of his superiors will start doing work.

Consider the largest $j < i$ such that employee $j$ is a superior of $i$ and $j$ starts doing work. Then for all $k < j < i$ where $k$ is a superior of $i$, $k$ is also a superior of $j$, and since $i$ stops doing work and $j$ starts doing work, the number of subordinates of $k$ who are doing work remains the same, and thus $k$ will not start doing work if he wasn’t already doing work. 

By discrete Intermediate Value Theorem this means that for each $L \le x \le R$, there is some $o_i$ that achieves $W = x$. Now we can simply binary search on $i$ to find the $o_i$ that gives $W = x$. To compute the value of a given ordering, we may simply iterate from $N$ to $1$ and check whether each employee will be doing work, or we can process the employees in the order they go to the office and use a Fenwick Tree to determine the number of subordinates doing work and decide whether each employee does work. These solutions work in $\mathcal{O}(N \log N)$ and $\mathcal{O}(N \log^2 N)$ respectively, and have similar performance because Fenwick is too fast. If you try to implement the $\mathcal{O}(N \log N)$ solution by doing a recursive DFS every binary search, you may TLE because DFS is pretty slow.

**Note:** there is a simpler solution in $\mathcal{O}(N)$ time (I assume most people did something like that). For example, consider employees coming to the office in the order $N, N - 1, \ldots$, until exactly $N - K$ do not work (say this happens when $x$ comes to work), then the ordering $1, 2, \ldots, x - 1, N, N - 1, \ldots, x$ will be valid. The proof of why this works is similar to the proof of the first claim.
