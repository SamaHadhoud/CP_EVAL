# Problem Statement

Nerissa owns a rectangular-shaped paddy field which is divided into $R \times C$ small squares ($R$ rows and $C$ columns) of the same size. Each square is either an empty soil (empty square, which can be used for any purpose) or a rice-planted soil (specifically to plant rice).

To prevent birds like crows or sparrows from disturbing the crops, Nerissa decides to put some scarecrows on the field. Each scarecrow can only be placed on an empty square. Furthermore, to prevent other humans from stealing the crops, the scarecrow arrangement must be sacred. An arrangement is said to be sacred if all the following conditions are satisfied:

- Each row contains at least one scarecrow,
- Each consecutive two columns contain at least one scarecrow.

Now Nerissa wonders, how many different sacred arrangements are there? Two arrangements are different if there is a square that contains a scarecrow in one arrangement but not in the other arrangement. Help Nerissa to compute this number.

# Input

The first line contains two integers: $R \ C \ (1 \le R \le 14;\ 1 \le C \le 1000)$ in a line denoting the size of paddy field in terms of the number of squares (number of rows and columns, respectively).
The paddy field is given in the next $R$ lines where each line contains a string of length $C$.
Each square is represented by either . which denotes an empty soil, or 'v' which denotes a rice-planted soil.

# Output

The output contains an integer representing the number of different sacred arrangement, in a line.
As the output can be very large, modulo the output by $1,!000,!000,!007$.

# Sample Input
```
2 2
v.
..
```
# Output for Sample Input
```
3
```
# Sample Input
```
1 3
...
```
# Output for Sample Input
```
5
```
# Sample Input
```
2 3
vvv
...
```
# Output for Sample Input
```
0
```
# Sample Input
```
3 3
...
.v.
...
```
# Output for Sample Input
```
145
```
# Sample Input
```
2 4
.vv.
.v.v
```
# Output for Sample Input
```
5
```
# Explanation for the 2nd sample case

For the second sample, the following are all 5 sacred arrangements (the scarecrows are denoted by character '*'):
```
***          *.*          **.           .**          .*.
```
# Explanation for the 3rd sample case

Notice that, for the third sample, we cannot put any scarecrow in the first row. As a sacred arrangement requires at least one scarecrow in each row, thus, in this case, there is no possible sacred arrangement.

# Explanation for the 5th sample case

For the fifth sample, the following are all 5 sacred arrangements (the scarecrows are denoted by character '*'):
```
*vv*           *vv*           *vv.           *vv.           .vv*
*v*v           .v*v           .v*v           *v*v           *v*v
```