# Problem Statement
Robert lives in ERPLand, a country with $N$ cities numbered 1 to $N$. Each day in this country has $C$ seconds, numbered 0 to $C - 1$. There are $M$ bi-directional roads connecting pairs of cities. Road $i$ connects cities $A_i, B_i$, takes Robert $T_i$ seconds to travel through, and can only be entered between the $L_i$-th and $R_i$-th second (both inclusive) in each day (no restriction on exit time). It is possible to get from any city to any other city directly or indirectly using the roads, and there might be multiple roads connecting the same pair of cities.

Robert lives in city 1, and wants to get to city $N$ to visit his friend, Bert. He is allowed to wait in cities to take a break, sightsee, or just wait for roads to open. Help him find out the minimum amount of time in seconds he needs to get to city $N$ if he starts at time 0 seconds on some day.

# Input

The first line of input contains three two positive integers $N, M$, and $C$ denoting the number of cities, number of roads and the number of seconds in each day respectively ($2 \le N \le 100000$, $N - 1 \le M \le 200000$, $1 \le C \le 10^9$).

Each of the following $M$ lines contains 5 integers $A_i, B_i, T_i, L_i, R_i$ which are as described above ($1 \le A_i, B_i \le N$, $A_i \ne B_i$, $1 \le T_i \le 10^9$, $0 \le L_i \le R_i < C$).

**It is guaranteed that Robert can get from city 1 to city N.**

# Output

Output a single integer, the minimum amount of time in seconds he needs to get from city 1 to city $N$.

# Sample Input #1
```
3 3 10
1 2 3 0 5
1 3 1 7 8
3 2 4 3 3
```
# Sample Output #1
```
7
```
# Sample Input #2
```
5 5 10
1 2 1 6 8
1 3 4 3 5
2 4 4 1 5
3 4 2 2 6
4 5 6 3 7
```
# Sample Output #2
```
20
```