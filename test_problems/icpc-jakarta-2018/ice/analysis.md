First, let us consider the case where $R > 2$ and $C > 2$. In this case, all the _inner_ cell, i.e. $r = [2, R-1]$ and $c = [2, C-1]$, should be a dry land as there will be no way to stop on the inner cell if it is an icy land. On the first row, last row, first column, and last column, there should be at least one dry land which does not lie on the corner. This dry land is used for us to enter the inner cells.

For example, in the following board, we can visit all cells regardless of the starting point.
```
.....#.
.#####.
.######
.#####.
######.
.#####.
...#...
```
For the case where $R \leq 2$, we only have to make sure that there is at least one dry land in each column $c = [2, C-1]$. Similarly, in the case where $C \leq 2$, we only have to make sure that there is at least one dry land in each row $r = [2, R-1]$.