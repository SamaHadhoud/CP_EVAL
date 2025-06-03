# Problem Statement
Pichuu, the Pokemon Master, faced a dilemma after destroying Team Galactic, becoming the champion and catching every legendary Pokemon. He realized he had lost his town map, a precious item given to him by his mother and also a Key Item! To avoid her scolding, Pichuu decided to reconstruct the map.

The town map was originally a bidirectional connected graph with $N$ towns, each labeled distinctly from 1 to $N$, where there was exactly one path between any two towns. Pichuu, unfortunately, forgot the direct connections between towns. While distraught, he discovered a useful feature on his trusty Poketch – it recorded the paths he took.

However, for each path from town $i$ to town $j$, the Poketch only recorded the town with the minimum label on the path, $A_{i,j}$. (The Poketch Company is still working out the bugs). For example, if the path on the map was $4 \to 2 \to 3 \to 5$, the Poketch will record 2 for the path from 4 to 5.

Working with what he got, Pichuu tries to reconstruct a fake Town Map that satisfies the records made by the Poketch. Pichuu however is a Pokemon Master, not a Graph Master. Please help him recover his town map.

Namely, construct a tree that satisfies the condition that the minimum label on the path from town $i$ to $j$ is $A_{i,j}$.

# Constraints

- $1 \leq N \leq 1500$.
- $1 \leq A_{i,j} \leq N$

# Input

The first line contains a single integer, $N$, the number of towns.  
$N$ lines follows, the $i^{th}$ line contains $N$ space-separated integers, $A_{i,1}, A_{i,2}, \dots A_{i,N}$ where $A_{i,j}$ is the town with the smallest label on the path from town $i$ to $j$.  
It is guaranteed there exists a solution given the provided values.

# Output

Output $N-1$ lines, each with two space-separated integers $A_i B_i$ representing an edge in your reconstructed town map.

# Sample Input #1
```
3
1 1 1 
1 2 1 
1 1 3 
```
# Sample Output #1
```
2 1
3 1
```
# Sample Input #2
```
6
1 1 1 1 1 1 
1 2 2 2 2 2 
1 2 3 2 2 2 
1 2 2 4 2 2 
1 2 2 2 5 2 
1 2 2 2 2 6 
```
# Sample Output #2
```
2 3
2 4
2 5
2 6
1 2
```