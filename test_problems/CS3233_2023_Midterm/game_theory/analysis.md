We use dynamic programming. Let $d^+(x)$ denote the score of the game if the token is at the $x$-th cell and it’s the maximizing player’s turn, and $d^-(x)$ if it’s the minimizing player’s turn. The base case of the dynamic programming is $d^+(x) = d^-(x) = 0$ if $x > n$. Then,

$$
d^+(x) = a_x + \max\{d^-(x + k \cdot p_x) \mid k \in \mathbb{N} \}
$$

$$
d^-(x) = a_x + \min\{d^+(x + k \cdot p_x) \mid k \in \mathbb{N} \}
$$

The time complexity is equal to 
$O\left(\frac{n}{p_1} + \frac{n}{p_2} + \frac{n}{p_3} + \ldots + \frac{n}{p_n}\right)$. But notice that $p$ is a permutation, so the complexity is $O(n \log n)$ by harmonic series.