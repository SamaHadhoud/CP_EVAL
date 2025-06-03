# Problem Statement

We say that a number sequence *contains duplicates* if there is an element that appears more than once in the sequence. Formally, a sequence $(a_1, \dots, a_n)$ contains duplicates if there exist two indices $i$ and $j$ such that $i \ne j$ and $a_i = a_j$.

You are given an $n \times n$ matrix $X$. Each entry in $X$ is an integer between $1$ and $n$, inclusive. You can modify zero or more entries in $X$ to arbitrary integers between $1$ and $n$, inclusive. Different entries can be modified to different integers.

Your task is to make modifications to entries of $X$ such that all of the following hold:

- For each row $i$, the sequence $(X_{i1}, X_{i2}, ..., X_{in})$ contains duplicates.
- For each column $j$, the sequence $(X_{1j}, X_{2j}, ..., X_{nj})$ contains duplicates.

Compute the minimum number of entries that need to be modified to achieve this. Also, find one possible set of modifications to do it. For each modification, you have to specify which entry will be modified and to what value. Note that the minimum number of entries to be modified can be zero when the given matrix $X$ already satisfies the conditions above.

# Input

The first line of input contains one integer $t$ $(1 \le t \le 1000)$ representing the number of test cases. After that, $t$ test cases follow. Each of them is presented as follows.

The first line of a test case contains one integer $n$ $(3 \le n \le 100)$. Each of the next $n$ lines contains $n$ integers. The $j$-th integer in the $i$-th line denotes $X_{ij}$ $(1 \le X_{ij} \le n)$.

The sum of $n^2$ across all test cases in one input file does not exceed $10\,000$.

# Output

For each test case, output a set of modifications in the following format.

On the first line, output an integer $m$ representing the minimum number of entries that need to be modified. On each of the next $m$ lines, output three integers $i$, $j$, and $v$. This represents a single modification where the entry $X_{ij}$ will be modified to $v$. All of the three integers must be between $1$ and $n$, inclusive.

If there are multiple solutions, you can output any of them.

# Sample Ιnput #1
```
5
4
3 2 1 1
2 1 3 4
1 3 3 1
4 4 4 2
3
1 3 1
2 1 3
3 2 2
5
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
3
1 1 2
2 2 1
2 3 2
3
1 1 3
3 2 1
3 1 3
```
# Sample Ouput #1
```
2
2 1 1
4 2 3
3
2 1 3
2 2 3
3 3 3
0
1
1 2 2
1
2 1 1
```
*Explanation for the sample input/output #1*

In the first test case, the matrix after the modification is as follows.

$$
\begin{bmatrix}
3 & 2 & 1 & 1 \\
\mathbf{1} & 1 & 3 & 4 \\
1 & 3 & 3 & 1 \\
4 & \mathbf{3} & 4 & 2 \\
\end{bmatrix}
$$