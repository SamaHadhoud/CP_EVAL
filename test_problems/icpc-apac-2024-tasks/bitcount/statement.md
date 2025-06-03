# Problem Statement

For a non-negative integer $x$, let $p(x)$ be the number of ones in the binary representation of $x$. For example, $p(26) = 3$ because $26 = (11010)_2$.

You are given a sequence of $n$ integers $(a_1, a_2, \ldots, a_n)$. Your task is to determine whether there exists a non-negative integer $x$ such that $(p(x), p(x+1), \ldots, p(x+n-1))$ is equal to $(a_1, a_2, \ldots, a_n)$. Furthermore, if it exists, compute the smallest $x$ satisfying the condition.

# Input

The first line of input contains one integer $t$ ($1 \leq t \leq 1000$) representing the number of test cases. After that, $t$ test cases follow. Each of them is presented as follows.

The first line contains one integer $n$ ($1 \leq n \leq 500\,000$). The second line contains $n$ integers $a_1, a_2, \ldots, a_n$ ($0 \leq a_i \leq 60$ for all $i$).

The sum of $n$ across all test cases in one input file does not exceed $500\,000$.

# Output

For each test case, output the smallest non-negative integer $x$ satisfying the condition above. If there is no such $x$, output $-1$ instead.


# Sample Input #1
```
4
5
3 3 4 1 2
3
2 1 2
2
60 60
2
8 0
```
# Sample Output #1
```
13
3
2305843009213693949
-1
```
*Explanation for the sample input/output #1*

For the first test case, $x = 13$ satisfies the condition above since $(p(13), p(14), p(15), p(16), p(17)) = (3, 3, 4, 1, 2)$.  
It can be shown that there is no non-negative integer smaller than $13$ that satisfies the condition above.