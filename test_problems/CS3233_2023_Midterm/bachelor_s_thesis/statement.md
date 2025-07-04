# Problem Statement
> *SoCCat the Undergraduate is a very studious cat. Currently, SoCCat is writing their Bachelor’s Thesis!*

For SoCCat’s Final Year Project, SoCCat will write a thesis studying the behaviour of a particular operation: the *cat-split*.

Initially, suppose SoCCat has an array of $n$ integers $a_1, a_2, \dots, a_n$, and an integer $k$ $(1 \leq k \leq n)$. In one *cat-split* operation, SoCCat will do all of the following in order:

1. Choose any subsequence of $a$ which consists of *exactly* $k$ elements.
2. Delete the chosen subsequence from the array $a$.
3. Append the values of the chosen subsequence to the *beginning* of the array $a$ in *reverse order*.

For example, for an array $a = [5, 1, 4, 2, 3]$ and integer $k = 3$, SoCCat can choose the subsequence $[1, 4, 3]$. Next, SoCCat will remove the elements $[1, 4, 3]$ from $a$, after which the array will become $[5, 2]$. Finally, SoCCat will prepend the elements $[1, 4, 3]$ in reverse order, after which the array will become $[3, 4, 1, 5, 2]$.

SoCCat is interested in knowing the minimum possible number of inversions in the array $a$ after doing the *cat-split* operation between $0$ up to $100\,000$ times. As SoCCat is busy studying other properties of the *cat-split* operation, they turn to you for help!

Note that you do not have to minimize the number of *cat-split* operations, only the number of inversions in the resulting array.

As a reminder,

- $b_1, b_2, \dots, b_k$ is a subsequence of $a$ *if and only if* $b_1, b_2, \dots, b_k$ can be obtained from $a$ by deleting zero or more elements from it without changing the order of the remaining elements.
- The inversion of an array $a$ is equal to the number of pairs $(i, j)$ such that $i < j$ and $a_i > a_j$.

# Input

The first line of input contains two integers $n$ and $k$, denoting the number of elements in the array $a$ and the specific number of elements chosen in the subsequence for each *cat-split* operation $(1 \leq n \leq 100; 1 \leq k \leq n)$.

The following line contains $n$ integers $a_1, a_2, \dots, a_n$, denoting the elements of the array $a$ $(1 \leq a_i \leq 100)$.

# Output

In the first line, output the minimum possible number of inversions in the resulting array after doing the *cat-split* operation between $0$ up to $100\,000$ times.

In the second line, output $l$, denoting the number of *cat-split* operations SoCCat should perform to obtain the minimum number of inversions in the resulting array $(0 \leq l \leq 100\,000)$. You do not have to minimize $l$.

Each of the following $l$ lines should contain a binary string of length $n$, denoting the subsequence SoCCat should choose for that particular operation. The $i$-th character of the binary string should be $1$ if the $i$-th element of the array is chosen for the subsequence, and $0$ otherwise. Note that each binary string should therefore contain exactly $k$ $1$’s.

# Sample Input #1
```
4 4
3 3 2 3
```
# Sample Output #1
```
1
3
1111
1111
1111
```
# Sample Input #2
```
4 4
3 2 3 3
```
# Sample Output #2
```
1
0
```
# Sample Input #3
```
5 2
1 2 4 3 5
```
# Sample Output #3
```
0
2
01010
01100
```