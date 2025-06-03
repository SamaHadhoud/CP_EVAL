The key observation is to consider the bounding rectangle of the points, that is, the minimum and maximum $x$- and $y$-coordinates over the points. This bounding rectangle is the only possibility if there is at least one point on each side of the square.

What if there are no points on at least one side of the square? In such case, our bounding rectangle will most likely not be a square. Without loss of generality, assume the width of the rectangle is greater than the height of the rectangle. Then, we should extend the height so that it matches with the width, i.e. retain one of $min_y, max_y$ and adjust the other such that its height is equal to $max_x - min_x$.

To show why this idea works, one can prove that if a solution exists, there must be a solution such that there are points lying on at least 3 sides of the square.

This algorithm runs in $\mathcal{O}(N)$ time, as we are only checking a constant number of candidate solutions.
