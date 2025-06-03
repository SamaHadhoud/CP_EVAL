Let's start with the fact that the value of $K$ in input is not important at all. Let $L$ be the length of the string instead.

There are some trivial (or tricky?) cases:
- If the string ends with '=' and $L > N$, then there is no solution.
- If the string doesn't end with '=' and $L \geq N$, then there is no solution.
- If the string doesn't end with '=' and $L < N$, then there is a simple solution: one by one from the first to last character, output the largest possible number if it is '<' or smallest possible number if it is '>'. For example, $N = 10$ and $S = \texttt{"><><>"}$ will construct $1 \sim 10 \sim 9 \sim 2 \sim 8$.

The main problem is how to construct a solution given that the string ends with '=' and $L \leq N$, means that Romanos has to win on exactly the $L$-th turn given the scenario of the string. It is provable that if Romanos is actually able to win on $X$-th turn, then he can make himself win on $Y$-th turn where $X \leq Y \leq L$. Hence, we can simulate the best strategy to find out if Romanos can win on the $L$-th turn; which is the binary search strategy.

Let boolean function $\texttt{isPossible}(A, B, \texttt{pos})$ be the function returning if it is possible to construct a solution given that the smallest and largest possible numbers are $A$ and $B$ respectively and now we're on the $\texttt{pos}$-th turn (of $L$, 0-indexed). Based on previous proof, binary search can be used for this function. Let's call the number used to guess is $\texttt{mid}$; if the $\texttt{pos}$-th character is '<' then the number of numbers less than $\texttt{mid}$ has to $\leq$ the number of numbers more than $\texttt{mid}$, else (if the character is '>') then the number of numbers less than $\texttt{mid}$ has to $>$ the number of numbers more than $\texttt{mid}$. It turns out the value of $\texttt{mid}$ is $\left\lfloor\frac{A + B + 1}{2}\right\rfloor$ if '<' or $\left\lfloor\frac{A + B - 1}{2}\right\rfloor$ if '>'. The algorithm will be like this:

```
bool isPossible(long long A, long long B, int pos) {
    if (A == B) return true;
    if (S[pos] == '=') return false;

    if (S[pos] == '<') {
        long long mid = (A + B + 1) / 2;
        return isPossible(A, mid-1, pos+1);
    } else {
        long long mid = (A + B - 1) / 2;
        return isPossible(mid+1, B, pos+1);
    }
}
```
Calling $\texttt{isPossible}(1, N, 0)$ will check if there exists a solution, but how can we construct the solution? There are actually two strategies that can be used by Romanos on each turn: using best strategy which is using binary search, or the "nonsense" one which is "the largest possible number if it is '<' or smallest possible number if it is '>'". Those two strategies are working dependently: in case Romanos needs to guess fast ($L$ is small) he needs first strategy, or in case of need to guess slow but sure ($L$ is large) he needs second strategy. To not making the game ends faster than $L$ turns, Romanos always chooses the second strategy whenever possible. These strategies are sufficient to construct one of possible solution.

Let $\texttt{solve}(A, B, \texttt{pos})$ be the procedure to construct a solution given that the smallest and largest possible numbers are $A$ and $B$ respectively and now we're on the $\texttt{pos}$-th turn (of $L$, 0-indexed). Calling $\texttt{solve}(1, N, 0)$ will do. The algorithm will be like this:
```
void solve(long long A, long long B, int pos) {
    if (S[pos] == '=') {
        guess(A);
    } else if (S[pos] == '<' && isPossible(A, B-1, pos+1)) {
        guess(B);
        solve(A, B-1, pos+1);
    } else if (S[pos] == '<') {
        long long mid = (A + B + 1) / 2;
        guess(mid);
        solve(A, mid-1, pos+1);
    } else if (S[pos] == '>' && isPossible(A+1, B, pos+1)) {
        guess(A);
        solve(A+1, B, pos+1);
    } else if (S[pos] == '>') {
        long long mid = (A + B - 1) / 2;
        guess(mid);
        solve(mid+1, B, pos+1);
    }
}
```