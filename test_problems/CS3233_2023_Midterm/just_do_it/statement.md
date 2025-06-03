# Problem Statement
> _SoCCat the Competitive Programmer is very tired of reading long statements unrelated to the actual problem. So, here’s a problem statement that’s just a single sentence!_

Given an array $a_1, a_2, \ldots, a_n$ of integers, find the following value, modulo $1\ 000\ 003\ 233$:

$$
\sum_{i=1}^{n} \sum_{j=i}^{n} \min\{a_i, a_{i+1}, \ldots, a_j\} \times \max\{a_i, a_{i+1}, \ldots, a_j\}
$$

# Input

The first line of input contains an integer $n$, denoting the size of the array $(1 \leq n \leq 300\ 000)$.

The second line contains $n$ integers $a_1, a_2, \ldots, a_n$, denoting the integers in the array $(1 \leq a_i \leq 10^9)$.

# Output

Output a single integer denoting the value of the expression above, modulo $1\ 000\ 003\ 233$.

# Sample Input #1
```
3
1 2 3
```
# Sample Output #1
```
25
```
# Sample Input #2
```
4
1 2 1 2
```
# Sample Output #2
```
22
```
# Sample Input #3
```
6
1 1 1 1 1 1
```
# Sample Output #3
```
21
```