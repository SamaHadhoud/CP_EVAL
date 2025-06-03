We can solve this problem using a segment tree. Notice that we can abstract any information in a segment tree node as long as it supports efficient merging between nodes.

For each node in the segment tree, if we only maintain the density of the substring, we will run into issues during merging, in particular, when the first substring ends with the same character the second substring starts with. It is impossible to determine the density for the concatenation of two substrings if we are only given their respective densities.

To solve this issue, we can additionally maintain the starting and ending character of the substring, as well as their (consecutive) counts. For example, for the substring `aabccaccc`, we will maintain that the substring starts with 2 `a`'s and ends with 3 `c`'s. With this additional information, we can now calculate the density of the concatenation of two substrings.

This solution runs in $\mathcal{O}((N + Q)\log N)$.
