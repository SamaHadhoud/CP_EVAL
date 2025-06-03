# Problem Statement

Andi likes numbers and sequences, especially, sign sequences. A sign sequence is a sequence which consists of $-1$ and $1$. Andi is a curious person, thus, he wants to build a sign sequence which length is $N$ (the positions are numbered from 1 to $N$, inclusive).

However, Andi also likes some challenges. Therefore, he prefilled some positions in the sequence with $-1$ or $1$ (the number in these positions cannot be changed). Andi also wants the sequence to fulfill $K$ constraints. For each constraint, there are 3 numbers: $A_i$, $B_i$, and $C_i$. This means that the sum of numbers which position is in the range $[A_i, B_i]$ (inclusive) must be at least $C_i$.

Sounds confusing, right? It is not done yet. Since there can be many sequences that fulfill all the criteria above, Andi wants the sequence to be lexicographically smallest. Sequence $X$ is lexicographically smaller than sequence $Y$ if and only if there exists a position $i$ where $X_i < Y_i$ and $X_j = Y_j$ for all $j < i$.

Find the sequence Andi wants.

# Input

Input begins with a line containing two integers: $N$ $K$ $(1 \le N \le 100000;\ 0 \le K \le 100000)$ representing the length of the sequence and the number of constraints, respectively. The second line contains $N$ integers: $P_i$ $(-1 \le P_i \le 1)$. If $P_i = 0$, then the $i^{th}$ position in the sequence is not prefilled, otherwise the $i^{th}$ position in the sequence is prefilled by $P_i$. The next $K$ lines, each contains three integers: $A_i$ $B_i$ $C_i$ $(1 \le A_i \le B_i \le N;\ -N \le C_i \le N)$ representing the $i^{th}$ constraint.

# Output

Output contains $N$ integers (each separated by a single space) in a line representing the sequence that Andi wants if there exists such sequence, or “**Impossible**” (without quotes) otherwise.

# Sample Input #1
```
3 2
0 0 0
1 2 2
2 3 -1
```
# Sample Output #1
```
1 1 -1
```
# Explanation for the sample input/output #1

Both sequences $[1, 1, -1]$ and $[1, 1, 1]$ satisfy the prefilled conditions and the given $K$ constraints. The former is lexicographically smaller.

# Sample Input #2
```
3 2
0 -1 0
1 2 2
2 3 -1
```
# Sample Output #2
```
Impossible
```
# Explanation for the sample input/output #2

The second position is already prefilled with −1, so it is impossible to fulfill the first constraint. There is novalid sequence in this case.
