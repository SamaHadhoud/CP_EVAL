# Problem Statement
You have $n$ objects that you like. The $i$-th object gives you a happiness rating $H_i$ when you bring it with you.

However, you can only bring exactly $k$ out of those $n$ objects for your upcoming holiday (due to baggage/airline limitation, etc).

Which objects should you take?

...wait a minute, have we seen this before?

As you know, there are $\binom{n}{k}$ different ways of choosing which objects you bring for your holiday. For each of these configuration, the happiness rating of a configuration is the sum of happiness ratings of the objects in the configuration.  
You are curious: what are the $l$ largest happiness ratings of all $\binom{n}{k}$ configurations?

# Input

The first line of input contains three integers $n$ ($1 \le n \le 10^6$), $k$ ($1 \le k \le n$), and $l$ ($1 \le l \le 10^6$).

The next line contains $n$ integers. The $i$-th integer denotes $H_i$ ($1 \le H_i \le 10^9$), the happiness rating that the $i$-th object gives you if you bring it with you.

# Output

Output $l$ lines, each containing a single integer. The $i$-th ($1 \le i \le l$) line should contain the $i$-th largest happiness rating of a configuration. If $i > \binom{n}{k}$, you should output $-1$ instead.

# Sample Input #1
```
4 2 6
3 2 3 3
```
# Sample Output #1
```
6
6
6
5
5
5
```
# Sample Input #2
```
5 1 10
1 1 1 1 1
```
# Sample Output #2
```
1
1
1
1
1
-1
-1
-1
-1
-1
```
# Sample Input #3
```
20 10 20
3 1 4 1 5 9 2 6 5 3 5 8 9 7 9 3 2 3 8 4
```
# Sample Output #3
```
71
70
70
70
70
70
70
69
69
69
69
69
69
69
69
69
69
69
69
69
```