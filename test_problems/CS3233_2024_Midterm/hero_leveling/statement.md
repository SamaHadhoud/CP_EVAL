# Problem Statement
Edgar wants to be a hero! However, to take the Hero Qualification Test, he must be at least level $L$. He is currently level $0$, and there are $N$ quests available for him to take. Quest $i$ requires him to be at least level $M_i$ to complete, and he will gain $X_i$ levels upon completing the quest. Each quest can only be done at most once. Additionally, due to the nature of these quests, quest $i$ has a trauma level $T_i$ associated with it.

Luckily, Edgar has poor memory so he will only remember the $K$ most traumatic quests he has done! His _trauma rating_ is therefore the sum of the $K$ largest trauma values among the quests he has done. If he does less than $K$ quests, then his trauma rating is just the sum of the trauma level of all quests he has done.

Help Edgar determine what is the minimum trauma rating he needs to reach at least level $L$ and take the Hero Qualification Test!

# Input

The first line of input consists of a three positive integers $N$, $K$, $L$, the number of quests, the maximum number of trauma levels he can remember, and the minimum level he needs to reach  
$(1 \le N \le 150,\ 1 \le K \le N,\ 1 \le L \le 1.5 \cdot 10^{11})$.

The following $N$ lines each contain three positive integers $M_i, X_i, T_i$, the minimum level Edgar needs to be to do quest $i$, the number of levels he gains from doing it, and the trauma level of the quest  
$(0 \le M_i \le 1.5 \cdot 10^{11},\ 1 \le X_i \le 10^9,\ 1 \le T_i \le 150)$.

**Additional Constraint**: It is guaranteed that Edgar will be able to reach level $L$ with the quests given.

# Output

Output a single integer, the minimum trauma rating Edgar needs to reach at least level $L$.

# Sample Input #1
```
5 2 10
0 3 5
5 5 9
2 2 3
7 4 6
0 4 7
```
# Sample Output #1
```
13
```
# Sample Input #2
```
6 4 10
3 6 6
0 4 7
0 3 5
4 2 4
6 7 4
7 3 3
```
# Sample Output #2
```
13
```