# Problem Statement

Adding two numbers several times is a time-consuming task, so you want to build a robot. The robot should have a string $S = S_1 S_2 \ldots S_N$ of $N$ characters on its memory that represents addition instructions. Each character of the string, $S_i$, is either ‘A’ or ‘B’.

You want to be able to give $Q$ commands to the robot, each command is either of the following types:

- `1 L R`: The robot should toggle all the characters of $S_i$ where $L \leq i \leq R$. Toggling a character means changing it to ‘A’ if it was previously ‘B’, or changing it to ‘B’ if it was previously ‘A’.
- `2 L R A B`: The robot should call $f(L, R, A, B)$ and return two integers as defined in the following pseudocode:

```
function f(L, R, A, B):
    FOR i from L to R
        if S[i] == 'A':
            A = A + B
        else:
            B = A + B
    return (A, B)
```

You want to implement the robot’s expected behavior.

# Input

Input begins with a line containing two integers: $N$ $Q$ ($1 \leq N, Q \leq 100,000$) representing the number of characters in the robot’s memory and the number of commands, respectively. The next line contains a string $S$ containing $N$ characters (each either ‘A’ or ‘B’) representing the initial string in the robot’s memory. The next $Q$ lines each contain a command of the following types:

- 1 L R ($1 \leq L \leq R \leq N$)

- 2 L R A B ($1 \leq L \leq R \leq N$; $0 \leq A, B \leq 10^9$)

There is at least one command of the second type.

# Output

For each command of the second type in the same order as input, output in a line two integers (separated by a single space), the value of $A$ and $B$ returned by $f(L, R, A, B)$, respectively. As this output can be large, you need to modulo the output by $1,000,000,007$.

# Sample Input #1
```
5 3
ABAAA
2 1 5 1 1
1 3 5
2 2 5 0 1000000000
```
# Sample Output #1
```
11 3
0 1000000000
```
# Explanation for the sample input/output #1

For the first command, calling $f(L, R, A, B)$ causes the following:

- Initially, $A = 1$ and $B = 1$.
- At the end of $i = 1$, $A = 2$ and $B = 1$.
- At the end of $i = 2$, $A = 2$ and $B = 3$.
- At the end of $i = 3$, $A = 5$ and $B = 3$.
- At the end of $i = 4$, $A = 8$ and $B = 3$.
- At the end of $i = 5$, $A = 11$ and $B = 3$.

Therefore, $f(L, R, A, B)$ will return $(11, 3)$.

For the second command, string $S$ will be updated to “ABBBB”.

For the third command, the value of $A$ will always be 0 and the value of $B$ will always be $1\,000\,000\,000$.  
Therefore, $f(L, R, A, B)$ will return $(0, 1\,000\,000\,000)$.
