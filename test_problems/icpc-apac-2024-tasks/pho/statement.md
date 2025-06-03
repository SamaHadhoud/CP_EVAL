# Problem Statement

As you may know, pho is one of the most common dishes in Hanoi. It contains a special kind of flour noodles, meat (usually beef or chicken), and green onions dipped in a tasty broth. Vietnamese people enjoy pho for breakfast, lunch, dinner, and even light meals. For tourists, trying pho is a must, especially in the cold of Hanoi.

You own a phở bò (beef pho) restaurant in Vietnam with $n$ tables, numbered 1 to $n$. The 2024 ICPC Asia Pacific Championship contestants are currently in your restaurant. Each contestant is initially seated at one of the tables and there is at least one contestant initially seated at each table.

Each contestant would like to order one of the two most well-known kinds of pho: *phở tái* (pho with medium-rare beef) or *phở chín* (pho with well-done beef). The initial state of table $i$ is represented by the binary string $S_i$. The length of $S_i$ is the number of contestants initially seated at table $i$. The $j$-th character of $S_i$ is 0 if the $j$-th contestant initially seated at the table would like to order a *phở tái*, and 1 if the contestant would like to order a *phở chín*.

To make it easier to track the orders, the restaurant wants the contestants seated at the same table to have the same order. That is, for each table, at least one of the following must be true:

- All of the contestants seated at that table would like to order a *phở tái*.
- All of the contestants seated at that table would like to order a *phở chín*.

To satisfy this requirement and the contestants’ orders, you want to move zero or more contestants to a different table. The destination table must be one of the $n$ tables. In other words, you must not add new tables. There is no limit to the number of contestants that can be seated at the same table. After moving the contestants, the following condition should be satisfied by each table: either there is no contestant seated at that table or all contestants seated at that table would like to order the same dish.

Since moving contestants takes some time, you would like to compute the minimum number of contestants you need to move.

# Input

The first line of input contains one integer $n$ ($2 \leq n \leq 100\,000$). Each of the next $n$ lines contains a binary string. The $i$-th line contains $S_i$ ($1 \leq |S_i| \leq 200\,000$). The sum of $|S_i|$ across all $i$ does not exceed $500\,000$.

# Output

Output an integer representing the minimum number of contestants you need to move.

# Sample Input #1
```
4
11101101
00
10001
10
```
# Sample Output #1
```
5
```
### _Explanation for the sample input/output #1_

You can move

- the seventh contestant initially seated at table 1 to table 3,
- the fourth contestant initially seated at table 1 to table 4,
- the first and fifth contestants initially seated at table 3 to table 1, and
- the first contestant initially seated at table 4 to table 1.

You will then have all contestants seated at table 1 ordering *phở chín*, while the contestants seated at the other tables will be ordering *phở tái*. It can be shown that you cannot move fewer than 5 contestants to satisfy the requirements.

# Sample Input #2
```
2
101010
010101
```
# Sample Output #2
```
6
```
# Sample Input #3
```
5
0000
11
0
00000000
1
```
# Sample Output #3
```
0
```