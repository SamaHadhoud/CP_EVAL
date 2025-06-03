# Problem Statement

Mashiro has a tree with $N$ vertices and $N - 1$ unweighted edges. It is known that any two vertices in the tree are connected.

Mashiro wants to perform a *Morfonication* on the tree. To put it simply, Mashiro will do several operations to transform the original tree to a new tree to her liking. Each operation Mashiro does is of the following:

- Select an edge from the tree and remove it. After this step, the tree becomes disconnected to two components.
- Add an edge between any two vertices such that the structure is reconnected into a tree.

After every operation Mashiro does, the tree must stay as a tree, meaning it must have exactly $N - 1$ edges and any two vertices in the tree are connected.

Mashiro doesn’t have a lot of time since she’s late for practice. Can you help her find the minimum number of operations to do so?

# Input

The first line of input contains one integer $N$ $(2 \leq N \leq 10^5)$, the size of the two trees.

The second to $N$-th lines of the input contain two integers $U_i$ and $V_i$ $(1 \leq U_i, V_i \leq N, U_i \neq V_i)$, the edges of the first tree.

The $(N+1)$-th to $(2N−1)$-th lines of the input contains two integers $X_i$ and $Y_i$ $(1 \leq X_i, Y_i \leq N, X_i \neq Y_i)$, the edges of the second tree.

# Output

Output one line containing one integer denoting the minimum number of operations.

# Sample Input #1
```
4
1 2
2 3
3 4
3 1
4 1
2 4
```
# Sample Output #1
```
3
```