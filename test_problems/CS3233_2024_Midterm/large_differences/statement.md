# Problem Statement
Tom has $N$ objects of various heights, where object $i$ has height $H_i$. He wants to line them up in a row and take a photo of them. Since he likes chaos, he wants the photo to look as disorderly as possible. In particular, given an arrangement, he will calculate the minimum difference in heights, $X$, between every pair of adjacent objects in the row.

Could you help him find the maximum possible value of $X$? Formally, find the maximum possible value of $\min_{1 \leq i \leq N - 1} |p_i - p_{i+1}|$ for all possible permutations $p_1, \dots, p_N$ of $H_1, \dots, H_N$.

# Input

The first line of input consists of a single positive integer $N$ ($2 \leq N \leq 500000$).

The next line contains $N$ integers $H_1, H_2, \dots, H_N (1 \leq H_i \leq 10^9)$.

# Output

Output a single integer, the maximum possible value of $X$ as described above.

# Sample Input #1
```
4
2 6 10 2
```
# Sample Output #1
```
4
```
# Sample Input #2
```
5
2 6 7 3 8
```
# Sample Output #2
```
4
```