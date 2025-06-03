# Problem Statement

In the company "Work Life Balance", there are $N$ employees numbered $1$ to $N$, where employee $1$ is the CEO of the company, and for $2 \le i \le N$, employee $i$ has a direct superior $B_i$. For two employees $a, b$, we say that employee $b$ is a subordinate of employee $a$ if and only if there exists a positive integer $k \ge 2$ and a sequence $u_1 = b, u_2, \ldots, u_k = a$ such that for each $1 \le i \le k - 1$, employee $u_{i+1}$ is the direct superior of employee $u_i$. Each employee also has a work tolerance level $A_i$.

Every day, the $N$ employees will go to the office in some order. When employee $i$ gets to work, if he sees that at most $A_i$ of his subordinates are doing work, he will do work as well. Otherwise, he will sit in the office and slack off. One day, the CEO of the company felt like maintaining a balance of work and slacking at the workplace, so he wonders if the $N$ employees could come to the office in some order such that exactly $K$ of them do work. Please help him find such an order, or determine that it is impossible.

# Input

The first line of input contains an two integers $N, K$ ($1 \le N \le 5 \cdot 10^5,\ 0 \le K \le N$), the number of employees and the target number of employees doing work.

The second line of input contains $N$ integers $A_1, A_2, \ldots, A_N$ ($0 \le A_i \le N - 1$).

The third line of input contains $N - 1$ integers $B_2, B_3, \ldots, B_N$ ($1 \le B_i < i$).

# Output

If it is not possible to find an order for the employees to go to the office such that exactly $K$ of them do work, output $-1$.

Otherwise, output a permutation of integers from $1$ to $N$, $P_1, P_2, \ldots, P_N$ in a single line, separated by spaces, such that if the employees go to office in the order $P_1, P_2, \ldots, P_N$, exactly $K$ of them will be doing work. If there are multiple solutions, output any of them.

# Sample Input #1
```
5 3
3 2 1 2 2 
1 2 2 1 
```
# Sample Output #1
```
-1
```
# Sample Input #2
```
7 5
4 2 0 0 3 0 1 
1 2 1 2 2 3 
```
# Sample Output #2
```
7 3 6 5 2 1 4
```