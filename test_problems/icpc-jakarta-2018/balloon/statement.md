# Problem Statement

Ayu is participating in ABC 2018 (Arranging Blocks Competition). In this competition, each contestant is given $M$ minutes and $N$ tasks which should be solved one-by-one in the given order. The contestant who solves the most tasks is the winner. What makes this contest interesting to you (ICPC contestants) is that this contest uses a similar encouragement as ICPC, i.e. balloons. In particular, each time a contestant solves a task, s/he will be given a balloon.

Ayu is convinced that she can defeat all other contestants, except one particular contestant, Budi, her rival. Ayu knows well her skill, i.e. she knows exactly how long it takes for her to solve a particular task. Unsurprisingly, Ayu also knows well Budi’s skill (they are rival for a reason). Let there be two arrays of integers $A_{1..N}$ and $B_{1..N}$. $A_i$ denotes the time (in minutes) needed by Ayu to solve the $i^{th}$ task, while $B_i$ denotes the time (in minutes) needed by Budi to solve the same $i^{th}$ task.

Here comes the interesting part. Ayu knows that Budi is very sensitive to any disturbingly loud sound like a balloon being popped. Whenever Budi is surprised (due to a balloon being popped), he will lose his concentration and has to repeat the task he’s doing. For example, suppose Budi needs 10 minutes to solve a particular task. If a balloon pops at the $7^{th}$ minute, then Budi repeats the task at the $7^{th}$ minute (out of his 10 minutes), causing him to solve the task with $7 + 10 = 17$ minutes. If two balloon pops, each at the $7^{th}$ and $13^{th}$ minute, then Budi repeats the task at minute $7^{th}$ (out of his 10 minutes), repeats it again at minute $6^{th}$ (out of his 10 minutes), and finally solved the task with $7 + 6 + 10 = 23$ minutes. If a balloon pops at the same time Budi supposed to solve the task (i.e. at the $10^{th}$ minute in this example), then Budi will also not solve that task. Therefore, Budi has to spend another 10 minutes (for a total of $10 + 10 = 20$ minutes) to solve that particular task in this case.

Ayu plans to exploit Budi’s weakness in order to defeat him, i.e. Ayu will strategically use the balloons (popping them at integer minutes) she gets from solving the tasks. Your task in this problem is to find out whether it is possible for Ayu to have the number of solved tasks to be strictly larger than Budi’s. If it is possible, you should output one (any) working plan on when she should pop the balloons.

Note that if Ayu solves a task at exactly the $M^{th}$ minute, then the task is considered as solved. Similarly, if Budi solves a task at exactly the $M^{th}$ minute, then the task is considered as solved, except if Ayu decides to pop a balloon at the same time. Also, Ayu can pop a balloon as soon as she receives it. Ayu cannot pop more than one balloon at the same minute. She also cannot pop any balloon after the $M^{th}$ minute mark.

# Input

Input begins with a line containing two integers: $N$ $M$ ($1 \le N \le 100000$, $1 \le M \le 10^9$) representing the number of tasks and duration of the competition, respectively. The second line contains $N$ integers $A_i$ ($1 \le A_i \le 10^9$) representing the time needed by Ayu to solve the $i^{th}$ task. The third line contains $N$ integers $B_i$ ($1 \le B_i \le 10^9$) representing the time needed by Budi to solve the $i^{th}$ task.

# Output

If it is not possible for Ayu to win the competition by having **strictly** larger number of solved tasks than Budi, simply output $-1$ in a line. Otherwise, output begins with an integer $K$ in a line representing the number of balloons Ayu needs to pop. The next line contains $K$ integers (each separated by a single space), sorted by **increasing order**, representing the minute in which Ayu should pop a balloon. You may output any configuration as long as it’s valid, i.e. Ayu has at least one balloon when she pops a balloon, Ayu is not popping a balloon after the contest is over, Ayu is not popping more than one balloon at the same minute, and the configuration causes Ayu to have a larger number of solved tasks than Budi.

# Sample Input #1
```
4 30
9 10 10 10
4 10 5 10
```
# Sample Output #1
```
2
12 19
```
# Explanation for the sample input/output #1

Ayu gets her first balloon at minute 9, while at that time, Budi already solved the first task (at minute 4) and is currently doing his second task which needs 5 more minutes (projected to finish at minute 14). Ayu pops his first balloon at minute 12, i.e. 2 minutes before Budi finish the second task, causing Budi to repeat the second task. Now, the projected time for Budi to finish the second task is at minute 22. At minute 19, Ayu gets her second balloon and pops it right away, causing Budi to repeat the second task again. Now, the projected time for Budi to finish the second task is at minute 29. At minute 29, Ayu gets his third balloon, and at the same time, Budi also solved the second task. The competition ends at minute 30. In total, Ayu solved 3 tasks, while Budi only managed to solve 2 tasks.

# Sample Input #2
```
5 50
10 10 10 10 10
15 12 19 17 20
```
# Sample Output #2
```
0
```
# Sample Input #3
```
5 10
15 10 5 5 5
9 10 10 10 10
```
# Sample Output #3
```
-1
```
# Explanation for the sample input/output #3

Ayu cannot solve the first task during the competition, so no balloon for her to play with.
