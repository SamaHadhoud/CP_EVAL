Note that this problem can be simplified as follows:

Given an array $A$ where each element is between $0$ to $T$. In one operation, we can increment or decrement the element in the array, but must still be between $0$ to $T$. Determine the minimum number of operations such that the $K$-th smallest element is different from the $(K+1)$-th smallest element in the array.

Let $f(A)$ be the largest possible integer and $g(A)$ be the smallest possible integer such that:

- $f(A) \le K$ and the $f(A)$-th smallest element is different from the $(f(A)+1)$-th smallest element in the array.
- $g(A) \ge K$ and the $g(A)$-th smallest element is different from the $(g(A)+1)$-th smallest element in the array.

If $f(A) = g(A) = K$, then we are happy. Otherwise, we want to change $A$ into another array $A'$ with the minimum cost such that $f(A') = g(A') = K$. There are two possible ways to achieve this:

- Decrement each element from the $(f(A)+1)$-th smallest element to the $K$-th smallest element by one, if the element is still greater than $0$.
- Increment each element from the $K$-th smallest element to the $g(A)$-th smallest element by one, if the element is still less than $T$.

Among the possible ways, we use the one with fewer number of operations. If neither of the ways is possible, then we print $-1$.