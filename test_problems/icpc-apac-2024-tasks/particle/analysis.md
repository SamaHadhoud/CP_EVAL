Using the notation $X^n$ to denote the string $X$ repeated $n$ times and $\varepsilon$ for the empty string, we can state our rules as $\text{P} \rightarrow \text{APA}$, $\text{A} \rightarrow \text{PPA}$, $\text{A}^a \rightarrow \varepsilon$, and $\text{PP} \rightarrow \varepsilon$. Here are more operations we can do:

- APA $\rightarrow$ P, because APA $\rightarrow$ AAPAA $\rightarrow$ AAAPAAPA $\rightarrow \cdots \rightarrow A^cP^a \rightarrow$ P. Similarly, PAP $\rightarrow$ A.
- AA $\rightarrow$ PP, because AA $\rightarrow$ APAP $\rightarrow$ PP. Similarly, PP $\rightarrow$ AA.
- PA $\rightarrow$ APPP, because PA $\rightarrow$ PPAP $\rightarrow$ AAAP $\rightarrow$ APPP. Similarly, AP $\rightarrow$ PAAA.
- P $\rightarrow$ PAAAA, because P $\rightarrow$ APA $\rightarrow$ PAAAA. Similarly, A $\rightarrow$ APPPP, P $\rightarrow$ AAAAP, and A $\rightarrow$ PPPPA.
- AAAA $\rightarrow$ PPPP, because AAAA $\rightarrow$ PPAA $\rightarrow$ PPPP. Similarly, PPPP $\rightarrow$ AAAA.

We can summarize the last few operations as $\varepsilon \rightarrow \text{A}^4$ and $\varepsilon \rightarrow \text{P}^4$. By repeatedly using these along with $\text{A}^a \rightarrow \varepsilon$ and $\text{P}^p \rightarrow \varepsilon$, we get $\varepsilon \leftrightarrow \text{A}^{\text{gcd}(a,4)}$ and $\varepsilon \leftrightarrow \text{P}^{\text{gcd}(p,4)}$, where we begin writing “$\leftrightarrow$” because we notice that all operations are now reversible. Also, without loss of generality, we can replace $a$ and $p$ by $\text{gcd}(a,4)$ and $\text{gcd}(p,4)$ respectively and then swap them if necessary to assume that $a \mid p \mid 4$.

We can then show using the above operations that any string is equivalent to one of A, AA, AAA, AAAA, P, AP, AAP, and AAAP. Let’s call these the eight special strings. Some of these can be reduced to each other further in case $a$ and/or $p$ are smaller than 4. But since $a \mid p \mid 4$, there are only a few possibilities, and we can just check one by one. The most substantial case is when $a = p = 4$, in which one can show that the eight special strings are distinct because they form a structure isomorphic to the quaternion group $Q_8$, the eight-element subset $\{1, i, j, k, -1, -i, -j, -k\}$ of the quaternions under multiplication. The other cases are just quotients of $Q_8$ by some of its subgroups. All in all, we get four distinct groups: the trivial group if $a = p = 1$, $\mathbb{Z}/2$ if $a = 1$ and $p > 1$, $(\mathbb{Z}/2)^2$ if $a = 2$, and $Q_8$ if $a = 4$.

We have now shown that a string is convertible into another string iff their corresponding group elements are the same, and the above already shows one way to do so. But there are many other ways, some taking fewer steps than others. Here’s my procedure to reduce a string $X$ to its corresponding special string:

```python
while X is not special or len(X) > gcd(a, 4)
    if len(X) > a and A**a is in X
        remove the A**a
    elif PA is in X
        convert the rightmost PA to APAA
    elif PP is in X
        convert the leftmost PP to AA
    else
        insert AAAA in front of X
```
After this procedure, $X$ will be a special string. To convert between equivalent special strings, we can just hardcode the conversion procedures—their overall contribution to the number of steps will be negligible anyway. There’s also a similar procedure to convert back to a given string from its special string. These procedures seem very efficient and seem to require less than 10 000 operations even for strings up to length 50.

Explicit bounds can also be obtained by carefully counting the number of steps of each intermediate operation above. For example, setting $n = 50$ and $k = 20$, we see that APA $\rightarrow$ P takes $k + O(1)$ steps, PAAA $\rightarrow$ AP takes $3k + O(1)$, etc. By working through the steps like this, one can show that a slightly altered version of the algorithm needs at most $8nk + 24k^2 + O(n + k)$ steps. This is $< 20,000$ in the worst case, so it passes comfortably.