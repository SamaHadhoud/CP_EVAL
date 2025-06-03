Let the transformation matrix for characters ‘A’ and ‘B’ be 
$\begin{bmatrix} 1 & 0 \\ 1 & 1 \end{bmatrix}$ and 
$\begin{bmatrix} 1 & 1 \\ 0 & 1 \end{bmatrix}$ respectively. This is because:

$$
\begin{bmatrix} a & b \end{bmatrix} \begin{bmatrix} 1 \\ 1 \end{bmatrix} = \begin{bmatrix} a + b \end{bmatrix} 
\quad \text{and} \quad 
\begin{bmatrix} a & b \end{bmatrix} \begin{bmatrix} 1 \\ 1 \end{bmatrix} = \begin{bmatrix} a + b \end{bmatrix}
$$

Next, we build a segment tree of size $N$, where each node is a $2 \times 2$ matrix which is the multiplication of its two children. The $i$-th leaf node is 
$\begin{bmatrix} 1 & 0 \\ 1 & 1 \end{bmatrix}$ if $S[i] = A$ or 
$\begin{bmatrix} 1 & 1 \\ 0 & 1 \end{bmatrix}$ if $S[i] = B$.

To answer the query, we simply do the standard range query operation on the segment tree and obtain the transformation matrix. Multiply the initial $A$ and $B$ with this matrix will give us the answer.

To serve the toggle operation, we do update on the segment tree (with lazy propagation). If a node containing 
$\begin{bmatrix} p & q \\ r & s \end{bmatrix}$ covers the range that is to be updated, we update the node to 
$\begin{bmatrix} s & r \\ q & p \end{bmatrix}$.

We can prove the correctness of the toggle operation by induction on the length of the character. The base case of a single character can obviously be seen as true. Suppose the toggle operation is true for a string of length $k$. Let 
$\begin{bmatrix} p & q \\ r & s \end{bmatrix}$ be the transformation matrix of the string of length $k$. Adding the character $A$ will give the transformation matrix:

$$
\begin{bmatrix} p & q \\ r & s \end{bmatrix} \begin{bmatrix} 1 & 0 \\ 1 & 1 \end{bmatrix} = 
\begin{bmatrix} p + q & q \\ r + s & s \end{bmatrix}
$$

Applying the toggle operation on the left hand side gives us:

$$
\begin{bmatrix} s & r \\ q & p \end{bmatrix} \begin{bmatrix} 1 & 1 \\ 0 & 1 \end{bmatrix} = 
\begin{bmatrix} s & r + s \\ q & p + q \end{bmatrix}
$$

The prove for “adding character B” can be done similarly. Hence, the toggle operation is proven to be true.

The total complexity is $\mathcal{O}(Q \log N)$.
