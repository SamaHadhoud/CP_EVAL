# Problem Statement

There are $n$ students taking a personality test consisting of $m$ questions. The students are numbered from $1$ to $n$ and the questions are numbered from $1$ to $m$. For each question, each student can either answer it with a single uppercase Latin character (A–Z) or not answer it. Let $S_i$ be a string of $m$ characters representing the answers of student $i$, where the $j$-th character of $S_i$ is an uppercase Latin character if they answered question $j$, or a period (.) if they did not.

Two students are considered *similar* if there is a set of at least $k$ questions where both students answered all questions in the set, and for each question in the set, they answered it with the same answer.

For example, let $n = 3$, $m = 3$, $k = 2$, $S_1 = \texttt{BBC}$, $S_2 = \texttt{..C}$, and $S_3 = \texttt{.BC}$. In this example, students 1 and 3 are similar since they answered questions 2 and 3 with the same answer, while students 2 and 3 are not similar since they answered only question 3 with the same answer.

You want to find a pair of integers $(a, b)$ such that $a < b$ and students $a$ and $b$ are similar, or determine if there is no such pair. If there is more than one pair, find the one with the *smallest* $b$. If there is still more than one pair, find the one with the *largest* $a$.

# Input

The first line of input contains three integers $n$, $m$, and $k$ ($2 \leq n \leq 5000$; $1 \leq m \leq 3000$; $1 \leq k \leq 5$). Each of the next $n$ lines contains a string of $m$ characters. The $i$-th line contains the string $S_i$.

# Output

Output one line containing the integers $a$ and $b$ representing the pair of similar students as mentioned in the problem statement, or just the integer $-1$ if there is no such pair.

# Sample Input #1
```
3 3 2
BBC
..C
.BC
```
# Sample Output #1
```
1 3
```
*Explanation for the sample input/output #1*

This is the example in the problem statement.

# Sample Input #2
```
3 3 1
BBC
..C
.BC
```
# Sample Output #2
```
1 2
```
*Explanation for the sample input/output #2*

Students 1 and 2 are similar.

# Sample Input #3
```
3 3 3
BBC
..C
.BC
```
# Sample Output #3
```
-1
```
*Explanation for the sample input/output #3*

There is no pair of similar students.

# Sample Input #4
```
4 12 2
GOOD.LUCK.IN
WINNING.ICPC
ASIA.PACIFIC
CHAMPIONSHIP
```
# Sample Output #4
```
2 3
```