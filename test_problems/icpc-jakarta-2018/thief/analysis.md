Observe that the length of the shortest string which contains $K$ distinct substrings of length $N$ (in other words, the length of the output) is exactly $N + K - 1$. More specifically, the output for this problem corresponds to a partial **de Bruijn sequence**.

Let $G = (V, E)$ be a directed graph where the set of nodes $V$ be all possible strings of length $N - 1$, and a node $u$ has a directed edge $(u, v)$ to node $v$ if and only if the string represented by node $v$ can be obtained by appending one character to $u$ and deleting the front-most character of $u$, e.g., “abcd” $\to$ “bcde” and the corresponding directed edge’s label is “e”. Then, the output for this problem corresponds to an **Eulerian path** of length $K$ in $G$.

However, the constraint for $N$ in the problem can be as large as $100000$, thus, we cannot simply build $G$ as described. Note that there are $M^{(N-1)}$ nodes in $G$.

Let’s consider two separated cases.

1. **Small** $N$ (e.g., $\leq 40$). In this case, we can solve the problem by using graph $G$, however, we should not construct $G$ as it. Instead, we should construct $G$ *on-the-fly* (as needed) and stop once we obtained the desired result, thus, we only have a “partial” $G$. With **Hierholzer’s algorithm** to find an Eulerian path, we can solve this in $O(NK)$ time, or $O(NK \log N)$ if you’re using an easier implementation.

2. **Large** $N$. In this case, simply output a random string (with the given alphabets) of length $N + K - 1$. Note that there are $M^N$ distinct substrings of length $N$. If $N$ is large and $M \geq 2$, then $M^N \gg K$, thus, the probability that the same substring appear more than once in the output is extremely low. Additionally, you can write a function to check whether the output string is valid, e.g., with **rolling hash** function (like the one implemented in the checker program for this problem), but it’s not necessary.

A note on Hierholzer’s algorithm. This algorithm finds an Eulerian tour in $O(E)$ and works by maintaining two lists, i.e. exploring list and result list. At any time, each list contains a valid Eulerian path. So, for the purpose of this problem, we can simply terminate when any of the lists contains $\geq K$ nodes, and work the output from that list.

Also, note that there are some implementation details not mentioned in this analysis for you to figure out.