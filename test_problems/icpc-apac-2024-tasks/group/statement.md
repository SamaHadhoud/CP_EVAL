# Problem Statement

There are $n$ students, numbered from $1$ to $n$, who need to form groups for the upcoming hackathon. You are student $1$, the captain of the students. Student $i$ has skill level $a_i$.

Students $2$ to $n$ are standing in a line from left to right in order. You can choose to stand in between any two students, to the left of student $2$, or to the right of student $n$. You cannot change the order of the $n - 1$ students.

You can also choose the number of groups $k$ ($k > 1$ and $k$ must be a divisor of $n$) to participate in the hackathon. The groups will be numbered from $1$ to $k$. After you have chosen your position and the value of $k$, the students will be grouped as follows:

- The first student from the left will be assigned to group $1$.
- The second student from the left will be assigned to group $2$.
- $\dots$
- The $k$-th student from the left will be assigned to group $k$.
- The $(k + 1)$-th student from the left will be assigned to group $1$.
- The $(k + 2)$-th student from the left will be assigned to group $2$.
- $\dots$
- The $n$-th student from the left will be assigned to group $k$.

Formally, for each $j$ ($1 \le j \le k$) and for each $i$ ($0 \le i < n/k$), the $(i \times k + j)$-th student from the left will be assigned to group $j$. It can be shown that each student will be assigned to exactly one group and all the groups have the same number of students.

The skill level of a group is the sum of the skill levels of the students inside the group. By choosing where you stand as well as the number of groups $k$ optimally, you want to minimize the ratio $x_{max}/x_{min}$ where

- $x_{max}$ is the skill level of the group with the largest skill level, and
- $x_{min}$ is the skill level of the group with the smallest skill level.

# Input

The first line of input contains one integer $t$ ($1 \le t \le 100\ 000$) representing the number of test cases. After that, $t$ test cases follow. Each of them is presented as follows.

The first line of a test case contains two integers $n$ and $a_1$ ($2 \le n \le 10^6;\ 1 \le a_1 \le 1000$). The next line contains $n - 1$ integers $a_2, a_3, \dots, a_n$ ($1 \le a_i \le 1000$ for all $i$).

The sum of $n$ across all test cases in one input file does not exceed $10^6$.

# Output

For each test case, output one line containing two positive integers $p$ and $q$ such that the minimum ratio is $p/q$. The fraction $p/q$ should be irreducible. In other words, $p$ and $q$ should be coprime.

# Sample Input #1
```
2
4 1
2
3
10
4 3
```
# Sample Output #1
```
1 1
10 3
```
*Explanation for the sample input/output #1*

In the first test case, by standing between students $2$ and $3$ (or between students $3$ and $4$) and choosing $k = 2$, group $1$ will have the skill level $2 + 1$ and group $2$ will have the skill level $1 + 2$, thus the ratio is $1/1$.

In the second test case, the only choice for the value of $k$ is $3$.