Denote $f(R_A, R_B)$ be the number of possible pairs $(A, B)$ such that $B \le A,\ A \le R_A$ and $B \le R_B$. Then we are finding  
$f(R, R) - f(L - 1, R) - f(R, L - 1) + f(L - 1, L - 1)$.

$f(R_A, R_B)$ can be found by dynamic programming with bitmasks. We have to maintain the following states:

- **current_bit**: The number of bits for $A$ and $B$ filled so far. We will fill the bits from the most significant to the least significant one.
- **A < R_A**: Whether the most significant `current_bit` bits of $A$ is already less than the corresponding bits of $R_A$. Otherwise, those bits of $A$ will be exactly same as the corresponding bits in $R_A$.
- **B < R_B**: Same as above, but now we’re comparing $B$ and $R_B$.
- **carry**: Whether there will be a carry from the next less significant bit in $A + B$.
- **borrow**: Whether we will be borrowing 1 to the next less significant bit in $A - B$.

By including **carry** and **borrow** in the states, we are maintaining the interaction between bits in $A + B$ and $A - B$ so we do not need to worry about them anymore.

This solution runs in $\mathcal{O}(\log R)$, with a large constant. The transition formula is left as exercise.
