# Problem Statement
> *SoCCat the Construction Worker is a very diligent cat. Currently, SoCCat is planning to build the tallest tower in all of cat history!*

To build the tallest tower, SoCCat needs a lot of cat dollars. SoCCat plans to make money by taking on construction jobs optimally.

There are $n$ companies that periodically offer construction jobs every $T$ cat days. Company $i$ offers a construction job at the beginning of day $x_i + k \cdot T$ (for all integers $k \geq 0$), and it will take SoCCat $l_i$ cat days to complete the job. Note that SoCCat can only take on one job at a time, and it might take SoCCat longer than $T$ cat days to complete the job.

The construction sector is very competitive, so SoCCat can only take on a job on the day it is offered (otherwise, another cat will take the job). SoCCat will begin working on it immediately, so if they took a job on day $t$, they will complete the job at the end of day $t + l_i - 1$. After they completed the job, SoCCat will receive $p_i$ million cat dollars. SoCCat can take another job starting from day $t + l_i$.

Currently, it is day $0$. SoCCat wonders $q$ possible scenarios: what is the maximum amount of cat dollars SoCCat could get before day $z_j$?

# Input

The first line of input contains two integers $T$ and $n$, denoting the number of cat days between each construction job offered by a company and the number of construction companies ($1 \leq T \leq 20\,000;\ 1 \leq n \leq 100\,000$).

The next $n$ lines each contain three integers $x_i$, $l_i$, and $p_i$, denoting the day (modulo $T$) the $i$-th company offers a construction job, the number of cat days SoCCat needs to complete the job, and the number of cat dollars (in millions) SoCCat will receive after completing the job ($0 \leq x_i < T;\ 1 \leq l_i \leq 10^{11};\ 1 \leq p_i \leq 5$).

The next line contains an integer $q$, denoting the number of scenarios ($1 \leq q \leq 20\,000$). The next $q$ lines each contain an integer $z_j$, denoting the day SoCCat wonders in scenario $j$ ($1 \leq z_j \leq 10^{11}$).

# Output

Output $q$ lines. The $j$-th line should contain the maximum amount of cat dollars (in millions) SoCCat could get before day $z_j$.

# Notes

In the sample test case, the 1-st company offers construction jobs at the start of day $0, 10, 20, \ldots$; if taken, the job will finish by the end of day $4, 14, 24, \ldots$ respectively. The 2-nd company offers construction jobs at the start of day $3, 13, 23, \ldots$; if taken, the job will finish by the end of day $18, 28, 38, \ldots$ respectively.

In the first and second scenarios, it’s possible to take the 1-st company’s construction job once to get 1 million cat dollars, but it’s not possible to take any other one. In the third scenario, it’s possible to take the 1-st company’s construction jobs twice to get 2 million cat dollars.

# Sample Input #1
```
10 2
0 5 1
3 16 4
7
5
14
15
19
1000
1005
3233
```
# Sample Output #1
```
1
1
2
4
200
201
645
```