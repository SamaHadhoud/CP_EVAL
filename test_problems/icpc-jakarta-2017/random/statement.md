# Problem Statement

Aristides has a random number generator. Each time he asks the random number generator, it will return a random integer between 1 to $N$ inclusive uniformly.

Aristides' friend, Iorgos, will note the returned number one by one. Aristides would keep asking a random number until Iorgos stop him. To analyse the randomness of the random number generator, he would stop Aristides immediately after each number from 1 to $N$ is returned at least twice.

Aristides has already asked $K$ numbers to the random number generator. The $i$-th number returned by the random number generator is $A_i$. Since Iorgos does not stop him yet, he knows that at least one of the integer between 1 to $N$ inclusive has not been returned at least twice.

Aristides wants to know the expected number of additional numbers to be asked until Iorgos stopped him.

# Input

The first line contains an integer: $T$ $(1 \leq T \leq 100{,}000)$ denoting the number of testcases. The first line of each testcase contains two integers: $N$ $K$ $(1 \leq N \leq 3{,}000; 0 \leq K \leq 100{,}000)$ in a line denoting the range of the returned integers and the number of integers that is already asked. The second line of each testcase contains $K$ integers: $A_1$ $A_2$ $\cdots$ $A_K$ $(1 \leq A_i \leq N)$ in a line denoting the first $K$ returned integers. It is guaranteed that at least one of the integer between 1 to $N$ inclusive has not been returned at least twice. It is also guaranteed that the sum of the value of $K$ on all testcases is not more than 100,000.

# Output

For each testcase, the output contains the expected number of additional numbers to be asked by Aristides until Iorgos stopped him, in a line. Your answer will be considered correct if the relative or absolute difference between your answer and judge's answer is not more than $10^{-6}$.

# Sample Input
```
4
1 0
1 1
1
2 10
3 0
2 2 2 2 2 2 2 2 2 2
```
# Output for Sample Input
```
2.000000000
1.000000000
4.000000000
9.638888889

```
# Explanation for the 1st sample case

For the first sample, the first two numbers returned by the random number generator is always 1. Therefore, by asking the random number generator twice, each number from 1 to N will be returned at least twice.

# Explanation for the 2nd sample case

For the second sample, Aristides already asked one number from the random number generator. Therefore, he only need to ask for one additional number.