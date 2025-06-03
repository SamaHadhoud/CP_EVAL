# Problem Statement
> SoCCat the Spellcaster is a very magical cat. Currently, SoCCat is learning about combination spells!

SoCCat has just learned about combination spells, which are spells that combine two or more spells from different spellcasters to deal more damage.

According to the Unified Theory of Magic, if there are $k$ spellcasters casting spells $x_1, x_2, \ldots, x_k$ respectively, then the combination spell will deal damage equal to $x_1 \oplus x_2 \oplus \ldots \oplus x_k$, where $\oplus$ is the bitwise XOR operation.

SoCCat knew of the famous spellcasting group, the $n$-membered Spellcasters of the Round Table. SoCCat also knew that the $i$-th member of the group can cast spells $0, 1, 2, \ldots, a_i$.

Now that SoCCat has learned about combination spells, SoCCat wants to know the maximum damage that can be dealt by the Spellcasters of the Round Table using a combination spell. Help SoCCat calculate the maximum damage!

### Notes

*Bitwise XOR* of two integers $x$ and $y$ can be computed as follows:

1. Write $x$ and $y$ in base two with the same bit length.
2. Perform the **XOR** operation for each corresponding bit pair:
    1. $0 \text{ XOR } 0 = 0$
    2. $0 \text{ XOR } 1 = 1$
    3. $1 \text{ XOR } 0 = 1$
    4. $1 \text{ XOR } 1 = 0$
3. Convert the **XOR** result back to base ten.

As an example, $6 \text{ XOR } 10 = 12$. Because $6 = 0110$ and $10 = 1010$ in base two, the bitwise XOR obtained is $1100 = 12$ in base ten.

# Input

The input starts with a line containing an integer $C$, denoting the number of test cases $(1 \leq C \leq 100)$. Each test case has the following format.

The first line contains an integer $n$, denoting the number of members in the Spellcasters of the Round Table $(1 \leq n \leq 100)$.

The next line contains $n$ integers $a_1, a_2, \ldots, a_n$ $(0 \leq a_i \leq 10^{18})$. The $i$-th integer denotes that the $i$-th member of the Spellcasters of the Round Table can cast spells $0, 1, 2, \ldots, a_i$.

# Output

For each test case, output two lines.

The first line of output should contain an integer denoting the maximum damage that can be dealt by the Spellcasters of the Round Table using a combination spell.

The next line should contain $n$ integers $x_1, x_2, \ldots, x_n$, where $x_i$ is the spell that the $i$-th member of the Spellcasters of the Round Table should cast for the combination spell to deal the maximum amount of damage $(0 \leq x_i \leq a_i)$. If there are multiple possible answers, output any one of them.

# Sample Input #1
```
2
3
3 7 1
4
3 2 3 3
```
# Sample Output #1
```
7
3 5 1
3
3 0 3 3
```