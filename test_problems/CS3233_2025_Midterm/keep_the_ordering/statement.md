# Problem Statement

Call a positive integer sorted in base $b$ if its base-$b$ digits appear in non-decreasing order. For example, 1223 is sorted in base 10, 132 or 120 are not.

It has been shown that there are infinitely many integers $n$ such that both $n$ and $n^2$ are sorted in base $b$ if and only if $b - 1$ is divisible by a nontrivial square (though I can’t track down the source). Therefore, fortunately, when $b = 10$ therefore infinitely many integers such that both $n$ and $n^2$ are sorted in base 10.

The problem: given $X$ as an input, output number of integers $n \in \{1, \ldots, X\}$ such that both $n$ and $n^2$ are sorted in base 10.

# Input

The first line is a positive integer, the number of test cases $T \le 10^5$. There are $T$ more lines, each contains a positive integer $X \le 10^{18}$.

# Output

For each test case, output a line containing the result.

# Sample Input #1
```
3
5
8
13
```
# Sample Output #1
```
5
7
9
```