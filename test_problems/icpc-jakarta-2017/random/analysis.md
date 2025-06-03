Let's start with a simple Dynamic Programming (DP) solution. We didn't actually care about which numbers has already been called once, twice (or more), or not at all; what we really need are only the number (a.k.a. how many numbers) of them. Denote $DP_1(cd_0, cd_1, cd_2)$ be the expected number of calling until all numbers are called minimum twice, given that there are $cd_0$ numbers that has not been called at all, $cd_1$ numbers that has been called once, and $cd_2$ numbers that has been called twice or more. The formulation will be like this:

$$
DP_1(0,0,N) = 0
$$

$$
DP_1(cd_0, cd_1, cd_2) = 1 + \frac{cd_0}{N} \times DP_1(cd_0 - 1, cd_1 + 1, cd_2) + \frac{cd_1}{N} \times DP_1(cd_0, cd_1 - 1, cd_2 + 1) + \frac{cd_2}{N} \times DP_1(cd_0, cd_1, cd_2)
$$

Take a note that there is a cyclic recursion on second equation: $DP_1(cd_0, cd_1, cd_2)$ calls $DP_1(cd_0, cd_1, cd_2)$. We can rewrite the formula by moving them to Left Hand Side.

$$
DP_1(cd_0, cd_1, cd_2) = 1 + \frac{cd_0}{N} \times DP_1(cd_0 - 1, cd_1 + 1, cd_2) + \frac{cd_1}{N} \times DP_1(cd_0, cd_1 - 1, cd_2 + 1) + \frac{cd_2}{N} \times DP_1(cd_0, cd_1, cd_2)
$$

$$
(1 - \frac{cd_2}{N}) \times DP_1(cd_0, cd_1, cd_2) = 1 + \frac{cd_0}{N} \times DP_1(cd_0 - 1, cd_1 + 1, cd_2) + \frac{cd_1}{N} \times DP_1(cd_0, cd_1 - 1, cd_2 + 1)
$$

$$
DP_1(cd_0, cd_1, cd_2) = \frac{N}{N - cd_2} \times (1 + \frac{cd_0}{N} \times DP_1(cd_0 - 1, cd_1 + 1, cd_2) + \frac{cd_1}{N} \times DP_1(cd_0, cd_1 - 1, cd_2 + 1))
$$

$$
DP_1(cd_0, cd_1, cd_2) = \frac{N}{N - cd_2} + \frac{cd_0}{N - cd_2} \times DP_1(cd_0 - 1, cd_1 + 1, cd_2) + \frac{cd_1}{N - cd_2} \times DP_1(cd_0, cd_1 - 1, cd_2 + 1)
$$

The DP formulation is complete now, but it takes up to $O(N^3)$ complexity on memory. A trick to make it more efficient is to reduce the state number. Note that $cd_0 + cd_1 + cd_2$ is always $N$, then we can remove one of the state, i.e. $cd_2$. Denote $DP_2(cd_0, cd_1)$ be the expected number of calling until all numbers are called minimum twice, given that there are $cd_0$ numbers that has not been called at all and $cd_1$ numbers that has been called once. The formulation will be like this:

$$
DP_2(0,0) = 0
$$

$$
DP_2(cd_0, cd_1) = \frac{N}{cd_0 + cd_1} + \frac{cd_0}{cd_0 + cd_1} \times DP_2(cd_0 - 1, cd_1 + 1) + \frac{cd_1}{cd_0 + cd_1} \times DP_2(cd_0, cd_1 - 1)
$$

Above DP will run in $O(N^2)$ complexity. Unfortunately, running it on each testcase will takes $O(TN^2)$; even reusing the DP on each distinct value of $N$ of the testcases will take $O(N^3)$.

As long as there is variable $N$ inside the DP, we can't reuse the DP for all testcases. There is another trick that can be done: divide the DP equations by $N$. It turns out that the $N$ will disappear completely from the DP. Let's denote the formulation after dividing by $N$ by $DP_3$ with these following formulation:

$$
DP_3(0, 0) = \frac{DP_2(0, 0)}{N}
$$

$$
DP_3(0, 0) = 0
$$

$$
DP_3(cd_0, cd_1) = \frac{DP_2(cd_0, cd_1)}{N}
$$

$$
DP_3(cd_0, cd_1) = \frac{1}{N} \times \left(\frac{N}{cd_0 + cd_1} + \frac{cd_0}{cd_0 + cd_1} \times DP_2(cd_0 - 1, cd_1 + 1) + \frac{cd_1}{cd_0 + cd_1} \times DP_2(cd_0, cd_1 - 1)\right)
$$

$$
DP_3(cd_0, cd_1) = \frac{1}{cd_0 + cd_1} + \frac{cd_0}{cd_0 + cd_1} \times \frac{DP_2(cd_0 - 1, cd_1 + 1)}{N} + \frac{cd_1}{cd_0 + cd_1} \times \frac{DP_2(cd_0, cd_1 - 1)}{N}
$$

$$
DP_3(cd_0, cd_1) = \frac{1}{cd_0 + cd_1} + \frac{cd_0}{cd_0 + cd_1} \times DP_3(cd_0 - 1, cd_1 + 1) + \frac{cd_1}{cd_0 + cd_1} \times DP_3(cd_0, cd_1 - 1)
$$

Finally, the rests are simple. $DP_3$ can be reused without resetting it for each testcase. For the $i$-th testcase, count how many numbers that has not been called at all, as $X_i$, and how many numbers that has been called once, as $Y_i$. The answer will be $N_i \times DP_3(X_i, Y_i)$ where $N_i$ is the value of $N$ on $i$-th testcase.