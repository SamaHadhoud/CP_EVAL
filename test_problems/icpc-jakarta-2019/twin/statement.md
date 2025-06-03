# Problem Statement

As you might already know, space has always been a problem in ICPC Jakarta. To cope with this, ICPC Jakarta is planning to build two new buildings. These buildings should have a shape of a rectangle of the same size. Now, their problem is to find land to build the buildings.

There are $N$ lands available for sale. The $i^{\text{th}}$ land has a rectangular shape of size $L_i \times W_i$. For a good feng shui, the building’s side should be parallel to the land’s sides.

One way is to build the two buildings on two different lands, one on each land (not necessarily with the same orientation). A building of size $A \times B$ can be build on the $i^{\text{th}}$ land if and only if at least one of the following is satisfied:

- $A \leq L_i$ and $B \leq W_i$, or
- $A \leq W_i$ and $B \leq L_i$.

Alternatively, it is also possible to build two buildings of $A \times B$ on the $i^{\text{th}}$ land with the same orientation. Formally, it is possible to build two buildings of $A \times B$ on the $i^{\text{th}}$ land if and only if at least one of the following is satisfied:

- $A \times 2 \leq L_i$ and $B \leq W_i$, or
- $A \times 2 \leq W_i$ and $B \leq L_i$, or
- $A \leq L_i$ and $B \times 2 \leq W_i$, or
- $A \leq W_i$ and $B \times 2 \leq L_i$.

Your task in this problem is to help ICPC Jakarta to figure out the largest possible buildings they can build given $N$ available lands. Note that ICPC Jakarta has to build two buildings of $A \times B$; output the largest possible for $A \times B$.

# Input

Input begins with a line containing an integer: $N$ $(1 \leq N \leq 100\,000)$ representing the number of available lands. The next $N$ lines each contains two integers: $L_i\ W_i$ $(1 \leq L_i, W_i \leq 10^9)$ representing the size of the land.

# Output

Output in a line a number representing the largest building that ICPC Jakarta can build with exactly one decimal point (see sample input/output for clarity).

# Sample Input #1
```
2
5 5
3 4
```
# Sample Output #1
```
12.5
```
# Explanation for the sample input/output #1

Two buildings of 2.5 × 5 can be built both on the first land.

# Sample Input #2
```
2
2 5
4 3
```
# Sample Output #2
```
8.0
```
# Explanation for the sample input/output #2

Two buildings of 2 × 4 can be built each on the first and second lands.

# Sample Input #3
```
3
10 1
9 8
7 6
```
# Sample Output #3
```
42.0
```
# Explanation for the sample input/output #3

Two buildings of 7 × 6 can be built each on the second and third lands.
