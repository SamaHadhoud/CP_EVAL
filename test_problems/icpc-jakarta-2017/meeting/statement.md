# Problem Statement

You are the boss of company X and have $N$ subordinates. Today, the $i$-th subordinate will come to the office $A_i$ seconds later than you.

You will have a team meeting today. Due to the capacity of the meeting room, there must be exactly $K$ people (excluding you) attending the team meeting. You can start the meeting $S$ seconds after you come to the office. You can choose the value of $S$ whatever you like, but it must be a positive real non-integer number. Everyone who is already present at the office at that start time will attend the meeting.

You can adjust the arrival time of your subordinates. By paying \$1 (one dollar) and choosing a subordinate, you can change the subordinate's arrival time by one second earlier or one second later. However, a subordinate must not arrive at the office strictly before you—that would be shameful for you. Also, a subordinate must not arrive strictly later than $T$ seconds after you—the subordinate could get fired. You can adjust the arrival time of as many subordinates as you want. You can also adjust the arrival time of the same subordinate more than once.

Determine the minimum amount of dollars needed such that you can have a meeting of exactly $K$ people (excluding you). If it is impossible to do so, output $-1$.

# Input

The first line contains three integers: $N\ K\ T$ ($1 \le K \le N \le 100,\!000$; $0 \le T \le 1,\!000,\!000,\!000$) in a line denoting the number of subordinates, the number of subordinates attending the meeting, and the maximum arrival time. The second line contains $N$ integers: $A_1\ A_2\ \cdots\ A_N$ ($0 \le A_i \le T$) in a line denoting the arrival time of each subordinate.

# Output

The output contains the minimum amount of dollars needed such that you can have a meeting of exactly $K$ people (excluding you), in a line. If it is impossible to do so, the output contains $-1$ instead.

# Sample Input
```
4 2 4
1 2 3 4
```
# Output for Sample Input
```
0
```
# Sample Input
```
4 2 4
1 2 2 4
```
# Output for Sample Input
```
1
```
# Sample Input
```
2 1 1
0 0
```
# Output for Sample Input
```
1
```
# Sample Input
```
2 1 0
0 0
```
# Output for Sample Input
```
-1
```
Explanation for the 1st sample case

For the first sample, if you start the meeting 2.5 seconds after you arrived, the meeting will be attended by the first two subordinates.

Explanation for the 2nd sample case

For the second sample, you can adjust the arrival time of the third subordinate to become one second later.

Explanation for the 3rd sample case

For the third sample, you can adjust the arrival time of the second subordinate to become one second later and start the meeting 0.71863781 seconds after you arrived.

Explanation for the 4th sample case

For the fourth sample, you cannot adjust any of the arrival time to satisfy your needs.
