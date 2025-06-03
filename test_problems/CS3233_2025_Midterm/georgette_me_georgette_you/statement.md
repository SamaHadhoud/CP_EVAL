# Problem Statement

Mutsumi is currently playing with permutation arrays. What makes permutation arrays interesting to her is that for a permutation array of size $N$, all elements are integers between $0$ and $N - 1$ inclusive, and no two elements share the same value. This means it is possible to make another permutation array where each element’s value becomes the index and the original index becomes the element’s value. Formally, given a permutation array $P$ of size $N$, she can create another permutation array $Q$ of size $N$ where $Q_{P_i} = i$ for all $0 \le i < N$.

Mutsumi noticed that for some permutation arrays, applying the rule above produces a different permutation array. Excited by the prospect of seeing different permutations, she asks: How many permutation arrays of size $N$ are there such that when she creates another permutation array using her rule, the resulting array is different from the original array?

Mutsumi will ask $T$ questions, each question will correspond to a different $N$. Since the number of permutations can be very large, she simply asks you to give her the number in modulo $M$.

### Input

The first line of input contains two integers $T\ (1 \le T \le 10^6)$ and $M$ ($10^8 \le M \le 10^9 + 7$), the number of questions and the modulo respectively.

The next $T$ lines of the input contain one integer $N\ (1 \le N \le 10^6)$, the size of the permutation array in question.

### Output

Output $T$ lines, each line containing one integer corresponding to the answer.


# Sample Input #1
```
2
4 6
######
######
######
######
20 20
.........##.........
......########......
....############....
...##############...
..################..
..################..
.##################.
.##################.
.##################.
####################
####################
.##################.
.##################.
.##################.
..################..
..################..
...##############...
....############....
......########......
.........##.........
```
# Sample Output #1
```
2/1
1/1
```