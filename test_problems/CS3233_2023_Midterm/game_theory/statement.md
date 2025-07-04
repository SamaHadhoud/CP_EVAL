# Problem Statement
> SoCCat the Game Theorist is a very tricky cat. Currently, SoCCat is challenging you to beat them in a game!

SoCCat challenges you to the following game:

There are $n$ cells arranged in a row, with numbers $1$ to $n$ written on them. The $i$-th cell from the left has the number $p_i$ written on it. SoCCat also has another array $a$ of size $n$.

Before the game begins, SoCCat will place a token on the $s$-th cell. Additionally, both of you will keep track of the score of the game, which is initially $0$. SoCCat aims to *minimize* the score of the game, while you aim to *maximize* the score of the game. Note that the score of the game is shared between both of you.

You will then take turns moving the token. You will move the token first, and then SoCCat will move the token, and so on until the game ends. If the token is currently on the $x$-th cell, the current player performs the following action in order:

- First, increase the score of the game by $a_x$.
- Next, choose a positive integer $k$ which is a multiple of $p_x$.
- Then, if $x + k \leq n$, move the token from the $x$-th cell to the $(x + k)$-th cell. Otherwise, if $x + k > n$, end the game.

For example, suppose $n = 5$ and $p = [2, 1, 5, 3, 4]$. Before the game begins, SoCCat places the token on the $1$-st cell. In the first turn, you can pick $k \in \{2, 4, 6, \ldots\}$. If you pick $k = 2$, then the token will move from the $1$-st cell to the $3$-rd cell, and the score of the game will increase by $a_1$. In the second turn, SoCCat can pick $k \in \{5, 10, 15, \ldots\}$. If SoCCat picks $k = 10$, then the score of the game will increase by $a_3$, and the game ends since $3 + 10 > 5$. The final score of this particular game is $a_1 + a_3$.

For each $s$ from $1$ to $n$, assuming both of you play optimally, what is the final score of the game if SoCCat initially places the token on the $s$-th cell?

# Input

The input starts with a line containing an integer $C$, denoting the number of test cases ($1 \leq C \leq 300\,000$). Each test case has the following format.

The first line contains an integer $n$, the number of cells in the row ($1 \leq n \leq 300\,000$). The sum of $n$ over all test cases does not exceed $300\,000$.

The second line contains $n$ integers $p_1, p_2, \ldots, p_n$, the integers written on the cells ($1 \leq p_i \leq n$; $p_i \ne p_j$ if $i \ne j$).

The third line contains $n$ integers $a_1, a_2, \ldots, a_n$, the integers in the array $a$ ($-10^9 \leq a_i \leq 10^9$).

# Output

For each test case, output a single line containing $n$ space-separated integers. The $s$-th integer denotes the final score of the game if SoCCat initially places the token on the $s$-th cell from the left, assuming both of you play optimally.

# Sample Input #1
```
2
10
3 1 5 2 4 9 6 10 8 7
1 -2 3 -4 5 -6 7 -8 9 -10
4
4 3 2 1
3 2 3 3
```
# Sample Output #1
```
8 7 3 -4 14 -6 7 -8 9 -10
3 2 3 3
```