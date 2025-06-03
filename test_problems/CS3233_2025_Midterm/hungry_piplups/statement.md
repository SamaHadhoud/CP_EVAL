# Problem Statement

Near Snowpoint City, food has become scarce, and the local Piplups have been fighting over fishing rights on the many icebergs in the area. In Snowpoint Bay, there are an infinite number of icebergs arranged in a line. Every day, exactly one Piplup journeys from the mainland to one of these icebergs, making it their home and fishing ground. Piplups are very territorial, if piplup swims to a iceburg $X$, that has already been claimed, he will be chased out by the piplup already on the island and will attempt to swim to iceburg $X + 1$ instead. This continues until he reaches an empty iceburg.

Professor Piplup, who has eaten enough gummis to maximize his IQ, has been observing and recording these Piplup migration patterns. Over $N$ days, he has recorded, for the $i^{th}$ day, exactly one piplup left for iceburg $A_i$. Some of his graduate students managed to steal some his records to plan for their migration trip and have the following queries for you.

For each query the student only have records from days $L_i$ to $R_i$, they assume that no Piplups left on days outside this range. As a group of $X_i$ Piplups, they will start leaving one by one, all heading for iceburg 1. Not wanting to drift too far downstream, they wish to determine the furthest iceburg (i.e., the largest numbered iceburg) that any of them will end up at with the information they have. Note that despite been friends, **there must still be at most one piplup per iceburg**.

There are also updates as the professor occasionally remembers that he recorded the $P_i^{th}$ Piplup going to a different iceburg, $X_i$, prompting an update to his records.

The students are unsure of the correct answer and seek your help in answering their queries. Please assist the students in determining the answer to their migration dilemma!

### Task Summary

Notably, the task is as such. Define an operation $\oplus$ as $S \oplus x = S \cup \{\text{Smallest } y : y \notin S \text{ and } y \ge x \}$.  
Given $N$ numbers $\{A_1, A_2, \ldots, A_N\}$. Handle $M$ queries:

- **Update**: Change $A_{P_i} = X_i$.
- **Query**: Find the $X_i^{th}$ smallest number not in $(\ldots ((\emptyset \oplus A_{L_i}) \oplus \ldots) \oplus A_{R_i})$.

# Input

The first line of input contains two integers $N, M$, as mentioned above.  
The second line of input contains $N$ integers $A_1, A_2, \ldots, A_N (0 \le A_i \le N - 1)$.

The third line of input contains $M$ queries of two possible types:

- Type 0: `0 Pi Xi`
- Type 1: `1 Li Ri Xi`

For type 0 query, the value at $P_i$ is changed to $X_i$. (I.e. $A_{P_i} = X_i$).  
For type 1 query, you are asked to output the position of the $X_i^{th}$ piplup only considering piplups from day $L_i$ to $R_i$ inclusive.

# Limits

- $1 \le N \le 10^6$
- $1 \le M \le 5 \times 10^3$
- $1 \le X_i, A_i \le 5 \times 10^2$
- $1 \le L_i \le R_i \le N$
- $1 \le P_i \le N$

# Output

For each type 1 query, output a single integer representing the position of the $X_i^{th}$ piplup only considering piplups from day $L_i$ to $R_i$ inclusive.

# Sample Explanation

For the first type 1 query, the piplups are at iceburgs 2, 3, 4, thus the $2^{nd}$ piplup will be at iceburg 5.

For the seond type 1 query, the piplups are at iceburgs 1, 2, 3, thus the $2^{nd}$ piplup will be at iceburg 5.

For the seond type 1 query, the piplups are at iceburgs 1, 2, 3, 4, 5, thus the $10^{th}$ piplup will be at iceburg 15.



# Sample Input #1
```
5 4
1 3 2 4 5 
1 2 4 2
0 2 1
1 1 3 2
1 1 5 10
```
# Sample Output #1
```
5
5
15
```