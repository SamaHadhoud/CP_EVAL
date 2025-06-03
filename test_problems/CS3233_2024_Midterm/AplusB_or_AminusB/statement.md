# Problem Statement
Alice believes that a friendly mid-term contest should start with an $A + B$ problem. But Bob disagrees. To increase creativity, he thinks an $A - B$ problem should be used instead.

To settle the dispute, the contest organizers have decided to use both problems. Given two integers $A$ and $B$, the contestants are required to output the value of $(A + B)$ or $(A - B)$, where or is the bitwise OR operator.

However, Alice still thinks her $A + B$ idea is superior. She decides to design the sample input of the problem so that the sample output is identical to $A + B$, that is, $A$ and $B$ satisfies the equality $(A + B)$ or $(A - B) = A + B$. Furthermore, her choice of $A$ and $B$ should satisfy $L \le B \le A \le R$ for some given range $[L, R]$.

How many possible choices of the pair $(A, B)$ are there?

For example, if $L = 5$ and $R = 7$, then the possible choices are $(5, 5), (6, 5), (6, 6), (7, 6),$ and $(7, 7)$, for a total of 5 choices.

# Input

The only line of input contains two integers $L$ and $R$ ($0 \le L \le R \le 2 \times 10^9$).

# Output

Output a single integer, the number of possible choices of the pair $(A, B)$ such that $L \le B \le A \le R$ and $(A + B)$ or $(A - B) = A + B$.


# Sample Input #1
```
5 7
```
# Sample Output #1
```
5
```