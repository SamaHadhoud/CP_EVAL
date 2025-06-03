# Problem Statement

There are $N$ teams (numbered from 1 to $N$) and $M$ problems (numbered from 1 to $M$) in this year's ICPC. The $j$-th problem has $T_j$ testcases. Surprisingly, every team submitted exactly one solution to every problem. The $i$-th team managed to solve $S_{i,j}$ testcases on the $j$-th problem.

A team solved a problem only if the team managed to solve ALL testcases on that problem. The winning team is the team with the most number of problems solved. If there are more than one team with the most number of problems solved, then the winning team is the team with the smallest index among those teams.

Determine the index of the winning team.

# Input

The first line contains two integers: $N$ $M$ ($1 \le N, M \le 100$) in a line denoting the number of teams and the number of problems. The second line contains $M$ integers: $T_1\ T_2\ \cdots\ T_M$ ($0 \le T_i \le 100$) in a line denoting the number of testcases. The next $N$ following lines, each contains $M$ integers; the $j$-th integer on the $i$-th line is $S_{i,j}$ ($0 \le S_{i,j} \le T_j$) denoting the number of solved testcases by the $i$-th team for the $j$-th problem.

# Output

The output contains the index of the winning team, in a line.

# Sample Input
```
3 2
10 20
0 19
10 0
9 19
```
# Output for Sample Input
```
2
```
# Sample Input
```
3 2
10 20
0 20
10 0
9 19
```
# Output for Sample Input
```
1
```
# Sample Input
```
1 1
1
0
```
# Output for Sample Input
```
1
```
# Explanation for the 1st sample case

On the first sample, the first and the third team did not solve any problem, and the second team solved the first problem. Therefore, the second team is the winner.


# Explanation for the 2nd sample case

On the second sample, the first team solved the second problem, the second team solved the first problem, and the third team did not solve any problem. Since the first team has a smaller index than the second team, the first team is the winner.

# Explanation for the 3rd sample case

On the third sample, there is only one team thus the winner is obvious.

