There are two solutions to this problem:

- **Dynamic Programming**: Observe that our ultimate goal is to pair up the students as much as possible, as we risk running out of space (there are only $N$ pairs of seats) if we do not pair them up sufficiently. We can let $\texttt{dp}[i]$ be the minimum number of pairs needed to pair up students $1, 2, \ldots, i$ such that the constraint is satisfied. To compute $\texttt{dp}[i]$, it suffices to condition on whether student $i$ is paired with student $i - 1$:

$$
\texttt{dp}[i] = \min \left\\{
\begin{array}{ll}
\texttt{dp}[i-2] + 1 & \text{if student } i \text{ and } i-1 \text{ forms a valid pair}\\
\texttt{dp}[i-1] + 1 & \text{if student } i \text{ can sit alone (i.e.\ arts student)}
\end{array}
\right.
$$

A solution exists if $\mathtt{dp}[#\text{ students}] \le N$, and we can backtrack to find the construction.

- **Greedy**: Notice that if we have a computing student at the beginning / end of the array or we have two consecutive computing students, then their seating arrangements are fixed. (For example, if we have ACCA as a substring, we know that the pairs must be AC and CA.) We first split the string whenever we see **ACCA**. For the remaining chunks without two consecutive computing students, we can greedily seat them using $$\left\lfloor \frac{\text{\# students}}{2} \right\rfloor$$ pairs (if we have more computing students than arts students, then this is clearly impossible). It is easy to see that this greedy strategy minimizes the number of pairs needed to seat all students.

P.S. Many teams attempted to code a (wrong) greedy solution right away. It’s a better idea to (informally) justify the correctness of your greedy approach or try coming up with counter-examples before coding. When in doubt, dynamic programming is the safe choice to go with, as long as it’s fast enough!

