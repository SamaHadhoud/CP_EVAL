# Problem Statement
Pichuu has just finished his catching spree in the safari zone and decided to spend the day organising his messy PC, where his Pokemon are stored. A PC has multiple boxes in a row and each box can store an unlimited amount of Pokemon. For simplicity, you can assume that the Pokemon in a box are also arranged in a row. Thus, we can define an ordering on Pokemon in the PC, where Pokemon $A$ is before $B$ if and only if $A$ is in a box that is to the left of the box $B$ is in, or $A$ and $B$ are in the same box and $A$ is to the left of $B$. Currently all of Pichuu’s Pokemon are in a single PC box.

He wishes to arrange them in a very specific order, namely he wishes the arrange them such that the the current $i^{th}$ Pokemon in the PC becomes the $A_i^{th}$ Pokemon in the PC. Following the ordering as mentioned above.

This PC is a new gen PC that the Pokemon Center recently upgraded, with only one available function, AutoSort. Where he can instantly arrange the Pokemon anywhere he wants as long as they remain in the same box. For example, if the Pokemon in 3 boxes are $[5, 2, 6], [1, 3], [7, 4]$, he can use AutoSort to instantly arrange the Pokemon as such $[2, 5, 6], [1], [3, 4, 7]$.  
**Note that each Pokemon must remain in the same box.**

Team Galactic have discovered this and plan on using this opportunity to annoy their enemy. They have hacked the PC and is able to manipulate where Pichuu’s Pokemon are. However, they are limited to only being able to separate the Pokemon into $K \le N$ contiguous segments and distributing the segments into separate boxes while maintaining the relative ordering.

For example, $[5, 2, 6, 1, 3, 7, 4]$ in box 1, can be separated into $[5, 2, 6]$ in box 1, $[1, 3]$ in box 2 and $[7, 4]$ in box 3.

They wish to do this optimally such that the number of boxes the Pokemon are in is as large as possible. (Making Pichuu have to navigate through different boxes)  
However, note that they must ensure Pichuu’s can still sort his Pokemon using the AutoSort function. Since if Pichuu is unable to sort his Pokemon, he will be so angry that he goes to Galactic HQ and destroy their HQ.

For example, if Team Galactic splits $[3, 1, 2, 5, 4, 6]$ into 3 boxes as such:  
$[3, 1, 2], [5, 4], [6]$, Pichuu can still sort his Pokemon using the AutoSort function:  
$[1, 2, 3], [4, 5], [6]$.  
However, if Team Galactic splits $[3, 1, 2, 5, 4, 6]$ into 2 boxes as such:  
$[3, 1], [2, 5, 4, 6]$, Pichuu is not able to sort his Pokemon and will destroy Team Galactic.

Furthermore, Pichuu is very fickle and can change his opinion on his Pokemon, swapping the preferred ordering of two Pokemon. Namely, he can choose two Pokemon currently at position $i$ and $j$ and swap $A_i$ and $A_j$.

# Constraints

- $1 \le N, M \le 5 \times 10^5$
- $1 \le A_i \le N$
- $A_1, \dots, A_N$ is a permutation.
- $1 \le U_i, V_i \le N$

# Input

The first line contains a two space-separated integers, $N$, the number of Pokemon and $M$ the number of swaps  
The second line contains $N$ space-separated integers, $A_1, A_2, \dots, A_N$, where $A_i$ represents the position Pichuu wants the Pokemon to be at.  
$M$ lines follows, the $i^{th}$ line contains 2 space-separated integers, $U_i, V_i$ which means that Pichuu swaps the position currently at position $U_i$ and $V_i$.

# Output

Output $M + 1$ lines, each containing a single integer.  
The first integer represents the answer prior to any swaps, and for $2 \le i \le M + 1$, the $i^{th}$ integer represents the answer after the $(i - 1)^{th}$ swap.

# Sample Input #1
```
6 5
3 1 2 5 4 6 
2 3
1 3
3 5
1 2
3 6
```
# Sample Output #1
```
3
3
5
4
3
2
```
# Sample Input #2
```
10 8
1 4 3 2 5 6 7 10 9 8 
8 10
4 2
6 7
6 7
2 4
9 10
4 2
8 9
```
# Sample Output #2
```
6
8
10
9
10
8
7
9
8
```