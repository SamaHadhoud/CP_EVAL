# Problem Statement
SoCCat the Drill Sergeant is a very strict cat. Currently, SoCCat is training cat recruits!

Drill Sergeant SoCCat is known for their ability to turn raw cat recruits into disciplined cat soldiers.

There are $n$ cat recruits, numbered $1$ to $n$. Cat recruit $i$ has height $i$ cat units.

Being cats, some of them dislike each other. In particular, there are $m$ pairs of cat recruits $(x_i, y_i)$ who dislike each other.

One day, SoCCat gathered the recruits and gave them $Q$ simple instructions. The instructions are either of the following:

Cat recruit $i$ should move into the line!

Cat recruit $i$ should move out of the line!

After each instruction, whichever cat is in the line should line themselves up such that they are sorted in increasing order of height. The frontmost cat in the line should be the cat with the smallest height, and the backmost cat in the line should be the cat with the largest height.

For example, if cat recruits $[1, 3, 5]$ are currently in the line, and Drill Sergeant SoCCat gives the instruction "Cat recruit $2$ should move into the line!", then the cat recruits in the line will be $[1, 2, 3, 5]$. After, if Drill Sergeant SoCCat gives the instruction "Cat recruit $3$ should move out of the line!", then the cat recruits in the line will be $[1, 2, 5]$.

SoCCat is very strict, and will not tolerate any cat disobeying their orders. Inevitably, some of the cat recruits will be forced to stand adjacent to some of the cats they dislike.

The discontentment of cat recruit $i$ is equal to:

- $3233$, if cat recruit $i$ dislikes both cats directly in front and behind them.
- $323$, if cat recruit $i$ dislikes the cat directly in front of them (and not the cat directly behind them).
- $32$, if cat recruit $i$ dislikes the cat directly behind them (and not the cat directly in front of them).
- $3$, if cat recruit $i$ does not dislike both cats directly in front and behind them. After all, all cat recruits (at the very least) dislike Drill Sergeant SoCCat!

After each instruction is carried out, Drill Sergeant SoCCat wants to find the sum of discontentment of all the cat recruits. As SoCCat is busy making sure none of the cat recruits disobey their orders, they have asked you to help them in counting the sum of discontentment. Help SoCCat!

# Input

The first line of input contains two integers $n$ and $m$, denoting the number of cat recruits and the number of pairs of cat recruits who dislike each other $(2 \leq n \leq 10^9; 1 \leq m \leq 100,000)$.

The next $m$ lines each contain two integers $x_i$ and $y_i$, denoting that cat recruit $x_i$ and cat recruit $y_i$ dislike each other $(1 \leq x_i < y_i \leq n; (x_i, y_i) \ne (x_j, y_j)$ if $i \ne j)$.

The next line contains a single integer $q$, denoting the number of instructions that Drill Sergeant SoCCat gave $(1 \leq q \leq 300,000)$.

The next $q$ lines each contain two integers $d_i$ and $z_i$ $(d_i \in {1, 2}; 1 \leq z_i \leq n)$, denoting that Drill Sergeant SoCCat gave the following instruction:

- If $d_i = 1$, then cat recruit $z_i$ should move into the line. It is guaranteed that cat recruit $z_i$ is not currently in the line.
- If $d_i = 2$, then cat recruit $z_i$ should move out of the line. It is guaranteed that cat recruit $z_i$ is currently in the line.

# Output

Output $q$ lines. The $i$-th line should contain the sum of discontentment of all the cat recruits after the $i$-th instruction is carried out.

# Sample Input #1
```
10 4
1 3
3 5
5 7
7 9
13
1 1
1 3
1 5
1 7
1 2
1 8
1 9
1 4
2 8
1 6
2 1
2 2
2 9
```
# Sample Output #1
```
3
355
3588
6821
3594
3597
3600
373
3600
373
370
367
15
```