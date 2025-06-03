# Problem Statement
SoCCat the Mathematician is a very curious cat. Currently, SoCCat is learning linear algebra!

SoCCat deemed a matrix $T$ of size $n \times n$ consisting of integers to be absolutely symmetric if and only if $|T_{i,j}| = |T_{j,i}|$ for all $1 \le i, j \le n$. In other words, the absolute value of the entry in the $i$-th row and $j$-th column is equal to the absolute value of the entry in the $j$-th row and $i$-th column.

Now, SoCCat has a matrix $A$ of size $n \times n$ consisting of integers. SoCCat wants to decompose this matrix $A$ into a sum of matrices $X_1 + X_2 + X_3 + \dots + X_k$, where all matrices $X_i$ consist of integers and are absolutely symmetric.

Additionally, SoCCat wants to minimize the number $k$ of matrices $X_i$ in the decomposition. If there are multiple possible decompositions, SoCCat will be satisfied by any of them. Of course, SoCCat will also want to find out if no such decomposition exists.

Unfortunately, SoCCat is stumped by this problem. Can you help SoCCat?

# Input

The first line of input contains one integer $n$ denoting the size of the matrix $A$ $(1 \le n \le 50)$. The next $n$ lines each contain $n$ integers. The $j$-th integer in the $i$-th line denotes the value of $A_{i,j}$, that is, the entry in the $i$-th row and $j$-th column of the matrix $A$ ($-10^9 \le A_{i,j} \le 10^9$).


# Output

If no decomposition exists, print a single integer $-1$.

Otherwise, the first line of output should contain one integer $k$, the minimum number of matrices $X_i$ in the decomposition.

Then, if $k \le 50$, you should additionally output the $k$ matrices $X_1, X_2, \ldots, X_k$.
For each matrix, you should output $n$ lines, each containing $n$ integers, denoting the entries of the matrix. The value of each entry should be an integer between $-10^{18}$ and $10^{18}$, inclusive.


# Sample Input #1
```
3
0 0 0
0 0 0
0 0 0
```
# Sample Output #1
```
1
0 0 0
0 0 0
0 0 0
```
# Sample Input #2
```
2
0 1
0 0
```
# Sample Output #2
```
-1
```
# Sample Input #3
```
2
1 3
5 7
```
# Sample Output #3
```
2
1 -1
1 -1
0 4
4 8
```