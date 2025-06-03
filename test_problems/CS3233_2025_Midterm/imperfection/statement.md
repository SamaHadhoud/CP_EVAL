# Problem Statement

Mei has an array $A$ of $N$ integers. Currently, Mei considers the array to be imperfect. For Mei, an array is perfect if and only if its maximum element is also the majority element. An element is considered the majority element if and only if the number of times it appears is strictly greater than the total number of occurrences of all other elements. Formally, an array $V$ is perfect if and only if  
$ \text{count}(V_i = \max(V)) > |V| - \text{count}(V_i = \max(V)) $.

Mei wants to delete some elements from $A$ to make the array perfect. The deletion cost for the $i$-th element of $A$ is $B_i$, and the total deletion cost is the sum of the individual costs.

Can you help Mei find the minimum cost to delete elements such that the resulting array is perfect?

# Input

The first line of input contains one integer $N$ ($1 \le N \le 3 \times 10^5$), the number of elements in array $A$.

The second line of the input contains $N$ integers $A_i$ ($1 \le A_i \le 3 \times 10^5$), the elements of $A$.

The third line of the input contains $N$ integers $B_i$ ($1 \le B_i \le 3 \times 10^5$), the deletion cost of $A_i$.

# Output

Output one line containing one integer denoting the minimum cost.

# Sample Input #1
```
4
5 3 3 3
3 2 2 2
```
# Sample Output #1
```
3
```
# Sample Input #2
```
15
3 1 4 1 5 9 2 6 5 3 5 8 9 7 9
8 6 4 7 5 1 3 4 9 8 5 4 1 2 2
```
# Sample Output #2
```
34
```