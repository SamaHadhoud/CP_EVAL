# Problem Statement
You have $N$ points on a Cartesian coordinate plane. The $i$-th point is located at $(X_i, Y_i)$, where $X_i$ and $Y_i$ are integers.

As a competitive programmer, you hate non-rectilinear geometrical objects. Therefore, you wish to connect all the points such that the resulting shape is a square, whose sides are either horizontal or vertical. All of the points must lie on the **border** of the square (inclusive of its vertices). Degenerate squares (i.e. squares with zero area) are allowed.

Can you find **any** such square, or report if it is impossible to do so?

# Input

The first line of input contains an integer $N$ $(1 \leq N \leq 10^5)$, the number of points.

The next $N$ lines each contains two integers $X_i$ and $Y_i$ $(-10^8 \leq X_i, Y_i \leq 10^8)$, the coordinates of the $i$-th point.

# Output

If it is impossible to connect all the points such that the resulting shape is a square, output `Impossible`.

Otherwise, output four integers $X_1$, $X_2$, $Y_1$ and $Y_2$, the coordinates of two opposite corners of the square. The four integers must satisfy  
$-10^9 \leq X_1 \leq X_2 \leq 10^9$ and $-10^9 \leq Y_1 \leq Y_2 \leq 10^9$.

If there are multiple valid answers, you may output any of them. It can be proven that if a solution exists, there will be at least one solution that satisfies the constraints.

# Sample Input #1
```
3
7 6
2 4
4 0
```
# Sample Output #1
```
2 8 0 6
```
# Sample Input #2
```
4
2 6
8 8
10 2
3 3
```
# Sample Output #2
```
Impossible
```