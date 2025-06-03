# Problem Statement

A university offers $N$ courses, numbered from $1$ to $N$ in increasing order of difficulty.

To ensure that students acquire the necessary knowledge before taking advanced courses, the university may impose **course dependencies**. Each course dependency is represented by a pair of integers $(u, v)\ (1 \le u < v \le N)$, meaning that students must complete course $u$ before enrolling in course $v$.

A set of course dependencies is called **acceptable** if all $N$ courses can be completed within $K$ semesters, assuming that a student can take any number of courses (possibly zero) in a semester.

Formally, a set of course dependencies is acceptable if and only if there exists a sequence of $N$ numbers $a_1, a_2, \ldots, a_N\ (1 \le a_i \le K)$ such that $a_u < a_v$ for every $(u, v)$ in the set.

Let $S$ be the set of course dependencies. Initially, $S$ consists of $M$ course dependencies $(A_1, B_1), (A_2, B_2), \ldots, (A_M, B_M)$, and **it is guaranteed that $S$ is acceptable.**

You need to process $Q$ queries sequentially. The $i$-th query $(1 \le i \le Q)$ is as follows:

- Given two integers $C_i, D_i\ (1 \le C_i < D_i \le N)$, determine whether adding a new dependency $(C_i, D_i)$ to $S$ maintains its acceptability. If adding this dependency makes $S$ unacceptable, output **reject** and leave $S$ unchanged. Otherwise, output **accept** and add $(C_i, D_i)$ to $S$.

# Input

The first line of input contains three integers $N,\ M$ and $K\ (2 \le N \le 2 \times 10^5,\ 0 \le M \le 2 \times 10^5,\ 1 \le K \le 100)$, representing the number of courses, the number of initial course dependencies, and the number of semesters, respectively.

The $i$-th line $(1 \le i \le M)$ of the next $M$ lines contain two integers $A_i$ and $B_i\ (1 \le A_i < B_i \le N)$, denoting the $i$-th initial course dependency.

The following line contains an integer $Q\ (1 \le Q \le 2 \times 10^5)$, the number of queries.

The $i$-th line $(1 \le i \le Q)$ of the next $Q$ lines contain two integers $C_i$ and $D_i\ (1 \le C_i < D_i \le N)$, representing the $i$-th query.

# Output

Output $Q$ lines.

The $i$-th line $(1 \le i \le Q)$ of the output should contain the result for the $i$-th query: Print **accept** if adding the dependency $(C_i, D_i)$ keeps $S$ acceptable, or **reject** if it makes $S$ unacceptable.

# Sample Input #1
```
4 1 2
1 2
3
2 3
3 4
1 3
```
# Sample Output #1
```
reject
accept
reject
```
# Sample Input #2
```
6 4 3
2 5
1 3
1 4
4 6
8
2 6
3 4
4 5
1 3
2 4
3 6
2 3
5 6
```
# Sample Output #2
```
accept
reject
accept
accept
accept
accept
accept
reject
```