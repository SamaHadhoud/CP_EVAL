# Problem Analysis

# The 2024 ICPC Asia Pacific Championship

This is an analysis of some possible ways to solve the problems of The 2024 ICPC Asia Pacific Championship. Since the purpose of this analysis is mainly to give the general idea to solve each problem, we left several (implementation) details in the discussion for reader’s exercise. If some of the terminology or algorithms mentioned below are not familiar to you, your favorite search engine should be able to help. If you find an error, please send an e-mail to icpc-apac-judges@googlegroups.com about it.

|Problem Title|Problem Author|Analysis Author|
|---|---|---|
|A Antiparticle Antiphysics|Kevin Charles Atienza|Kevin Charles Atienza|
|B Attraction Score|Jonathan Irvin Gunawan|Jonathan Irvin Gunawan|
|C Bit Counting Sequence|Mitsuru Kusumoto|Mitsuru Kusumoto|
|D Bánh Bò|Kevin Charles Atienza|Kevin Charles Atienza, Mitsuru Kusumoto|
|E Duplicates|Mitsuru Kusumoto|Mitsuru Kusumoto|
|F Forming Groups|Jonathan Irvin Gunawan|Jonathan Irvin Gunawan|
|G Personality Test|Meng-Tsung Tsai|Mitsuru Kusumoto|
|H Pho Restaurant|Ammar Fathin Sabili|Ammar Fathin Sabili|
|I Symmetric Boundary|Mitsuru Kusumoto|Mitsuru Kusumoto|
|J There and Back Again|Peter Rossmanith|Jonathan Irvin Gunawan|
|K Tree Quiz|William Gan|Jonathan Irvin Gunawan|
|L XOR Operations|Prabowo Djonatan, Muhammad Ayaz Dzulfikar|Mitsuru Kusumoto|
|M Zig-zag|Kevin Charles Atienza|Kevin Charles Atienza|

The contest judges would also like to thank the following for their valuable feedback to the tasks: Ashar Fuadi, Budiman Arbenda, Edbert Geraldy Cangdinata, Hocky Yudhiono, Lim Li, Masaki Nishimoto, Prof. Takashi Chikayama, R. Fausta Anugrah Dianparama, Riku Kawasaki, William Gan, Yehezkiel Raymundo Theodoroes, and Yui Hosaka.

The 2024 ICPC Asia Pacific Championship – Problem Analysis
# Antiparticle Antiphysics

Using the notation Xn to denote the string X repeated n times and ε for the empty string, we can state our rules as P → APA, A → PAP, Aa → ε, and Pp → ε. Here are more operations we can do:

- APA → P, because APA → AAPAA → AAAPAAA → . . . → AaPAa → P. Similarly, PAP → A.
- AA → PP, because AA → APAP → PP. Similarly, PP → AA.
- PA → APPP, because PA → PPAP → AAAP → APPP. Similarly, AP → PAAA.
- P → PAAAA, because P → APA → PAAAA. Similarly, A → APPPP, P → AAAAP, and A → PPPPA.
- AAAA → PPPP, because AAAA → PPAA → PPPP. Similarly, PPPP → AAAA.

We can summarize the last few operations as ε → A4 and ε → P4. By repeatedly using these along with Aa → ε and Pp → ε, we get ε ↔ Agcd(a,4) and ε ↔ Pgcd(p,4), where we begin writing “↔” because we notice that all operations are now reversible. Also, without loss of generality, we can replace a and p by gcd(a, 4) and gcd(p,4) respectively and then swap them if necessary to assume that a | p | 4.

We can then show using the above operations that any string is equivalent to one of A, AA, AAA, AAAA, P, AP, AAP, and AAAP. Let’s call these the eight special strings. Some of these can be reduced to each other further in case a and/or p are smaller than 4. But since a | p | 4, there are only a few possibilities, and we can just check one by one. The most substantial case is when a = p = 4, in which one can show that the eight special strings are distinct because they form a structure isomorphic to the quaternion group Q8, the eight-element subset {1, i, j, k, −1,−i, −j, −k} of the quaternions under multiplication. The other cases are just quotients of Q8 by some of its subgroups. All in all, we get four distinct groups: the trivial group if a = p = 1, Z/2 if a = 1 and p > 1, (Z/2)2 if a = 2, and Q8 if a = 4.

We have now shown that a string is convertible into another string iff their corresponding group elements are the same, and the above already shows one way to do so. But there are many other ways, some taking fewer steps than others. Here’s my procedure to reduce a string X to its corresponding special string:

while  X is not  special  or len(X) > gcd(a, 4)
if len(X) > a and Aa is in  X
remove the Aa
else if PA  is in X
convert the rightmost PA to APAA
else if PP  is in X
convert the leftmost PP to AA
else
insert AAAA  in  front of X

After this procedure, X will be a special string. To convert between equivalent special strings, we can just hardcode the conversion procedures—their overall contribution to the number of steps will be negligible anyway. There’s also a similar procedure to convert back to a given string from its special string. These procedures seem very efficient and seem to require less than 10,000 operations even for strings up to length 50.

# The 2024 ICPC Asia Pacific Championship – Problem Analysis
Explicit bounds can also be obtained by carefully counting the number of steps of each intermediate operation above. For example, setting n = 50 and k = 20, we see that APA → P takes k + O(1) steps, PAAA → AP takes 3k + O(1), etc. By working through the steps like this, one can show that a slightly altered version of the algorithm needs at most 8nk + 24k2 + O(n + k) steps. This is &lt; 20 000 in the worst case, so it passes comfortably.

# B    Attraction Score

Because the graph is planar, any subset of v nodes (for v &gt; 2) nodes can only contain at most 3v − 6 edges between them.

# Observation B.1.

It is sufficient to check only clique subgraphs or cliques with one missing edge. We define this as our search space.

# Proof.

For v &gt; 6, the score of any subgraph is not positive since (3v − 6) − ((v) −(3v − 6))2 < 0.

For v = 6, the maximum number of edges possible is 12, which means there are at least 3 missing edges (there are 15 edges in K6). Let x be a node with at least one missing edge, so its degree is at most 4. Removing x decreases the number of edges by at most 4 but reduces the number of missing edges to 2 or less, hence saving a penalty of at least (32 − 22) × 106 = 5 × 106, which is more than the score of 4 edges, so it’s better to remove node x. Thus, we can focus on v ≤ 5.

For v = 5, with two missing edges, the score is not better than if the node of minimum degree is removed. This reduced graph (without the node of minimum degree) is in our search space. With more than two missing edges, the score is negative.

For v < 5, the score is not positive if there is more than one missing edge.

We can enumerate all O(n) clique subgraphs of a planar graph in O(n log n) time. To do that, we look at the node with the minimum degree (call this node u). Let the set of neighbours of node u be Su. Since the graph is planar, it is guaranteed that |Su| ≤ 5, so we can obtain all cliques containing node u in O(1). We then continue the algorithm by enumerating cliques without node u. We can do so by removing node u from the graph and recursively doing the algorithm on the rest of the graph, which is still planar.

Thus, we can compute the maximum score among all clique subgraphs. We now focus on computing the maximum score among all subgraphs with 5 nodes and one missing edge. A similar argument can be applied to subgraphs with fewer nodes.

Consider node u in the recursive algorithm above. The hard case is when the missing edge is not incident to u, because then one node in the subgraph is not in Su. Let nodes a, b, and c be the nodes in the subgraph that are in Su. To find the missing node d that maximizes the score, we can note that nodes a, b, c, and d form a clique. We can also assume that there is no edge connecting nodes u and d, so the optimal choice for d is independent of u. Therefore, we can find this node d in O(1) by precomputing all O(n) cliques. This solution runs in O(n log n) time.

The 2024 ICPC Asia Pacific Championship – Problem Analysis
# Bit Counting Sequence

Observation C.1. For a non-negative integer y, if p(y) − p(y + 1) = t ≥ 0, the rightmost (t + 1) bits in the binary representation of y are all 1. Additionally, the (t + 2)-th bit of y from the right is 0. For example, consider y = 87 = (1010111)2. Here, t = p(y) − p(y + 1) = 5 − 3 = 2. As the observation states, the rightmost three bits of y are 1, and the fourth bit of y from the right is 0.

Let k be the smallest index such that ak = max(a1, . . . , an). Also, let x denote the solution to this problem. When k < n, let t = ak − ak+1. It’s worth noting that t ≥ 0 according to the definition of k. Based on the previous observation, the binary representation of x + k − 1 should be like

t+1
x + k − 1 = (. . . 0 1 . . .1)2.

The bits starting from the (t + 3)-th position from the right of x + k − 1 cannot be determined solely from this observation. However, considering that ak is the largest value among a1, . . . , an, for s = ak − (t + 1),

s
x + k − 1 = (1 . . . 1 01 . . .1)2.

is the smallest possible value for x + k − 1. Consequently, we can compute the smallest possible value of x. If any of the following conditions hold: s < 0, x < 0, or p(x + i − 1) = ai for some i, then the solution does not exist. Similarly, when k = n, the smallest possible value for x + n − 1 is 2an − 1. The runtime complexity is O(n).

# Bánh Bò

Let gi,j ∈ {0,1} be the value at row i and column j. We use zero-indexing (0 ≤ i < r and 0 ≤ j < c). Also, for simplicity sake, let r0 = 6 and c0 = 7.

Observation D.1 (“Quadrangle Equation”). For r0 ≤ i < r and c0 ≤ j < c, gi,j + gi−r0,j−c0 = gi−r0,j + gi,j−c0.

Proof. Consider a (r0 + 1) × (c0 + 1) grid. Its four r0 × c0 subgrids overlap. Adding the top-left and the bottom-right subgrids and subtracting the bottom-left and the top-right subgrids will yield the equation.

Observation D.1 has several nice consequences. For instance, in any (r0 + 1) × (c0 + 1) subgrid you can check that the only possibilities for the four corners are:

0 · · · 0 1 · · · 1 0 · · · 1 1 · · · 0 0 · · · 0 1 · · · 1

. . .

. . .

. . .

0 · · · 0 1 · · · 1 0 · · · 1 1 · · · 0 1 · · · 1 0 · · · 0

Since these six “tiles” can only be assembled in limited ways, we eventually observe that:
# Observation D.2

In the lattice {(i + r0a, j + c0b) | 0 ≤ a < r/r0, 0 ≤ b < c/c0} for each (i, j) ∈ {0, . . . , r0 − 1} × {0, . . . , c0 − 1}, at least one of the following holds: (1) for each row, all cells in it contain the same bit, or (2) for each column, all cells in it contain the same bit. (Both can be true.)

# Observation D.1

Also tells us that after we choose the topmost r0 rows and leftmost c0 columns, all the other cells are completely determined. Thus, all that remains is to choose the cells in those rows and columns while satisfying the following necessary conditions, which can also be shown to be sufficient by induction:

- For each (i, j) such that 0 ≤ i < r0 and 0 ≤ j < c0, one (or both) of the following hold: (A) gi,j = gi+r0,j = gi+2r0,j = gi+3r0,j = . . . and/or (B) gi,j = gi,j+c0 = gi,j+2c0 = gi,j+3c0 = . . .
- gi,0 + gi,1 + . . . + gi,c0−1 = gi+r0a,0 + gi+r0a,1 + . . . + gi+r0a,c0−1 for 0 ≤ i < r0 and 0 ≤ a < r/r0, and
- g0,j + g1,j + . . . + gr0−1,j = g0,j+c0b + g1,j+c0b + . . . + gr0−1,j+c0b for 0 ≤ j < c0 and 0 ≤ b < c/c0.

For further clarity, let’s define a pair (i, j) as being constant if the values of gi+r0a,j+c0b remain consistent for all a and b. If condition (A) is satisfied for (i, j), we denote the pair as being horizontally striped. Likewise, if condition (B) is met for (i, j), we denote the pair as being vertically striped. Note that these conditions for (i, j)–being constant, being horizontally striped, and being vertically striped–are not mutually exclusive. For example, being horizontally striped can include the case of being constant.

For the top-left r0 × c0 cells, let’s assume that each cell is fixed as one of constant, horizontally striped, or vertically striped. We will now determine the number of possible value assignments for the topmost r0 rows and the leftmost c0 columns. For simplicity, let p = r/r0 − 1 and q = c/c0 − 1. Let’s fix the values of gi,j to xi,j ∈ {0,1} for 0 ≤ i < r0 and 0 ≤ j < c0. Let H denote the set of horizontally striped cells and V denote the set of vertically striped cells.

- The number of ways to fill the remaining cells in the leftmost c0 columns is ∏i=0r0−1 (mi)p, where mi = #{j | (i, j) ∈ H} and ni = #{j | (i, j) ∈ H, xi,j = 1}.
- The number of ways to fill the remaining cells in the topmost r rows is ∏j=0c0−1 (m′)q, where m′ = #{i | (i, j) ∈ V} and n′ = #{i | (i, j) ∈ V, xi,j = 1}.

By summing over all possible combinations of values for xi,j, the number of ways to fill the topmost r0 rows and the leftmost c0 columns is

( r −1 m ( )p+1) ( c −1 m′ ( )q+1)

∏i=0r0−1 ∑k=0 mi ∏j=0c0−1 ∑k′=0 m′j.

Furthermore, if (i, j) is vertically striped and it is not constant, we refer to the pair as strictly vertically striped. When we fix that each of the top-left r0 × c0 cells is either strictly vertically striped or horizontally striped, the number of ways to fill the cells can be represented as ∏i=0r0−1 f(mi) ∏j=0c0−1 f′(m′) for some functions f and f′, which can be obtained through a calculation of inclusion and exclusion. Here, with H denoting a set of horizontally striped cells, we define mi = #{j | (i, j) ∈ H} and m′ = #{i | (i, j) ∈ H}. Note that the two conditions, "strictly vertically striped" and "horizontally striped," are mutually exclusive and collectively exhaustive.

The 2024 ICPC Asia Pacific Championship – Problem Analysis
From this observation, you can compute the solution of this problem in several ways. One is dynamic programming: There are 2r⁰c⁰ ways to fix "strictly vertically striped" or "horizontally striped" for each cell. Performing a brute-force search is too slow, but you can implement DP computation with some pruning tricks to improve efficiency.

Another approach is using generating functions: suppose that column j corresponds to a variable zj and that having mj horizontally striped cells in column j corresponds to zjm′. A polynomial for one row is represented as

G = &sum;x0,x1,...,xc₀−1∈{0,1} f(c0 − x0 − x1 − ... − xc⁻1)zx0...zxc₀−1.

Computing Gr₀ using Fast Fourier Transform (FFT) can lead to computing the solution efficiently. The runtime is O(N log N), where N = (r0 + 1)c0.

(Acknowledgement: This analysis is partially based on insightful feedback from Yui Hosaka.)

# E Duplicates

Let’s say a row or a column is colorful if the values within it are all distinct. Otherwise, we say it’s non-colorful. Our objective in this problem is to minimize the number of modifications needed to make all rows and columns non-colorful.

Let mr denote the number of colorful rows, and mc denote the number of colorful columns. Since the number of colorful rows and colorful columns decreases by at most one with each modification, we require a minimum of max(mr, mc) modifications to make all rows and columns in the matrix non-colorful. We can show that max(mr, mc) is indeed adequate for achieving this objective as follows.

When both mr > 0 and mc > 0 hold, there exists an entry (i, j) such that row i and column j are colorful. By changing the value at (i, j) to an arbitrary integer other than Aij, we simultaneously decrease the number of colorful rows and columns by one, respectively.

When mr = 0 and mc > 0, there exists a colorful column, let’s say column j. In this case, there must be at least one value v such that modifying an entry (1, j) to v makes column j non-colorful while keeping row 1 non-colorful.

A similar logic applies when mr > 0 and mc = 0.

Using this approach, we can accomplish the objective with max(mr, mc) modifications. The computational time is O(n3) with a sloppy implementation. This can be further improved to O(n2) time.

# F Forming Groups

For a fixed value of k, we can compute the minimum ratio by initially assuming that student 1 is standing on the leftmost position. We then try to shift student 1 one position to the right each time, while keeping track of the sum of skill levels of each group, and maintaining a data structure to keep track of the minimum and maximum sum. Each shift only modifies the sum of skill levels of two groups.

The 2024 ICPC Asia Pacific Championship – Problem Analysis
# G Personality Test

The problem can be reformulated as follows:

Suppose there exists a graph with n vertices, numbered from 1 to n. Initially, each pair of vertices is connected by an undirected edge with weight zero. We increment an integer b from 1 to n. In each step, for every 1 ≤ i < b and 1 ≤ j ≤ m such that Sij = Sbj = ‘.’, we increment the weight between vertices i and b by one. If we encounter an edge with a weight of k or more, we must report that edge and terminate the program.

A straightforward implementation of this procedure requires Θ(n2m) time. An important observation is that the sum of weights across all edges is bounded by (k − 1)(n), unless the step is at the termination of the program. This implies that the increase in edge weights occurs at most O(kn) times. With some precomputation, for each j such that Sbj = ‘.’ in the steps of the above procedure, we can enumerate all indices i satisfying Sij = Sbj in O(#{i < b | Sij = Sbj}) time, instead of Θ(b) time.

With this enumeration method, the runtime becomes O(nm + mΣ + kn2), where Σ is the number of Latin characters (in this problem, Σ = 26).

# H Pho Restaurant

Consider an easier version of this problem where we can add new tables. Clearly, it is optimal if, for the tables that are seated by different orders, we move the minority, i.e. the lesser orders between the two types of pho, to a new table.

One can assume that this strategy also works for the original problem. However, it only works if existing tables accommodate the minorities. This will not be an issue if we can have both types of pho to be a non-minority on some tables. But in case non-minorities are just one type (which also means there is just one type of minority), we need to choose a table that is dedicated to accommodating minorities instead. As this table will move the non-minority orders, it is not optimal to choose more than one dedicated table. Thus, a single careful iteration will suffice.

There is also an alternative solution that uses DP-Bitmask where we have four bits to represent: there is a

# The 2024 ICPC Asia Pacific Championship – Problem Analysis
# The 2024 ICPC Asia Pacific Championship – Problem Analysis

There is a table where we need to move ‘0’, there is a table where we need to move ‘1’, there is a table dedicated to consist of only ‘0’, and there is a table dedicated to consist of only ‘1’. All solutions run in O(n + ∑i=1n |Si|).

# I   Symmetric Boundary

Let’s denote an input point (xi, yi) by Pi. Also, let X represent the center of the point symmetry. According to the definition of point symmetry, a point Qi satisfying (Pi + Qi)/2 = X ⇐⇒ Qi = 2X − Pi should also be included in the boundary of the convex region we are seeking. The problem is to find a point X such that the 2n points P1, . . . , Pn, Q1, . . . , Qn form a hull, meaning that the boundary of the convex hull of the 2n points contains all of them. We say that a point X satisfying this condition is valid.

Somewhat briefly, the solution is as follows: for two indices a and b that are between 1 and n, inclusive, let L(a, b) be a line passing through (Pa + Pb)/2 and (Pa+1 + Pb)/2. Here, we say Pn+1 = P1. Draw n2 lines of L(a, b) on the plane for 1 ≤ a, b ≤ n. There are O(n4) intersection points between the lines. For each intersection point, verify whether the point is valid or not. If it is valid, calculate the area of the hull. This process requires O(n log n) time for each intersection point. The minimum area found by this procedure represents the solution. The runtime complexity is O(n5 log n).

But why is the above solution correct? Before delving into the rationale, let’s explain a few concepts.

- The point Qi depends on the point X, so from now on, we write Qi as Qi(X).
- For three points A, B and C, let CCW (A, B, C) be true if and only if (B − A) × (C − A) ≥ 0.
- For a valid point X, let s(X) denote the area of a convex hull formed by the points P1, . . . , Pn, Q1(X), . . . , Qn(X).

When we draw n2 lines, the plane is divided into O(n4) connected regions. Let’s select one region R. Here, R includes the boundary of the region. We denote the interior points of R by R◦. For any two points X ∈ R◦ and X′ ∈ R, and for any two indices a and b, both of the following hold.

- If CCW (Pa, Pa+1, Qb(X)) is true, CCW (Pa, Pa+1, Qb(X′)) is also true. (Hint: prove that CCW (Pa, Pa+1, Qb(X)) is equivalent to CCW ((Pa + Pb)/2, (Pa+1 + Pb)/2, X).)
- If CCW (Pa, Qb(X), Qb+1(X)) is true, CCW (Pa, Qb(X′), Qb+1(X′)) is also true.

This implies that if one point in R◦ is valid, then any point in R is valid. Also, the order of vertices on the boundary of a convex polygon formed by P1, . . . , Pn, Q1(X), . . . , Qn(X) is always the same for X ∈ R. From this, we can deduce that the function s : R2 → R is a linear function when the domain is restricted in R, because the area of a polygon is the sum of the cross product of neighboring vertices,

- Pa × Pa+1 is constant,
- Pa × Qb(X) and Qb(X) × Pa vary linearly with respect to X, and
• Qb(X) × Qb+1(X) = (2X − Pb) × (2X − Pb+1) = 2X × (Pb − Pb+1) + Pb × Pb+1 varies linearly with respect to X.

Therefore, the minimum value of s within the domain of R is always achieved on the boundary of R. Specifically, since R is a convex polygon, the minimum value is attained at the vertices of the region. This justifies the fact that we only need to check the intersection points of n2 lines.

# J There and Back Again

We would like to compute the sum of the route that has the minimum total traversal time (the best route) and the route that has the second minimum total traversal time (the second best route). Finding the best route can be done with Dijkstra algorithm. We can also note the set of roads traversed in this route.

To find the second best route, we run Dijkstra algorithm twice to compute, for each city u, the shortest path to city 1 (denoted as Su) and the shortest path to city n (denoted as Tu). We then iterate each road that is not traversed in either direction in the best route to try which edge is traversed in the second best route. For each such road (u, v), the minimum total traversal time for the second best route by traversing this edge is Su + w(u, v) + Tv where w(u, v) is the time it takes to traverse the road. We then take the minimum value among all such roads.

This solution runs in O(n log n) time.

# K Tree Quiz

The integers in L can be represented as tuples (x, LCA(x, y), y), where the tuples are ordered by x, ties are broken by LCA(x, y), and further ties are broken by y. Since each i has n tuples with i as its first element, the value of x for the k-th element of L is simply ⌈k ⌉. Next, we want to find the ((k − 1) mod n + 1)-th tuple with x as its first element. For simplicity, let m = (k − 1) mod n + 1.

Let S(i) be the set of nodes in the subtree of node i, and A = {a1, a2, ...} be the ancestors of node x in order from x (i.e., a1 = x and node ai+1 is the parent of node ai). Let f(ai) = |S(ai) \ S(ai−1)| (i.e., the number of nodes in the subtree of node ai but not in the subtree of node ai−1), and f(z) = 0 for z ∈ A. This means f(z) is the number of tuples with x and z as the first and second elements respectively.

a3 = 2

a2 = 4

x = a1 = 1

• f(a1) = f(1) = 3 (nodes 1, 6, and 7)

• f(a2) = f(4) = 2 (nodes 4 and 5)

• f(a3) = f(2) = 3 (nodes 2, 3, and 8)

• f(3) = f(5) = f(6) = f(7) = 0

The 2024 ICPC Asia Pacific Championship – Problem Analysis
The m-th tuple that we are looking for has l as LCA(x, y) where ∑i=1l f(i) ≥ m and ∑i=1l−1 f(i) < m. We can find this value using a segment tree that stores the values of f(i). For each edge (u, v), by changing the value of x from u to v, only the values of f(u) and f(v) change. Therefore, we can keep the segment tree for each x using persistent segment tree in O(n logn) preprocessing time.

Let m′ = m − ∑i=1l−1 f(i). We want to find the m′-th tuple with x and l as its first and second elements respectively. Let p be the index of A such that ap = l. Thus, we want to find the m′-th node (in increasing order) in S(ap) \ S(ap−1).

We can do this by using another segment tree. Let Gi be a segment tree that stores Gi(j) = 1 if node i is an ancestor of node j, or Gi(j) = 0 otherwise. To compute Gi, we can take Gh as the base, where node h is the child of node i with maximum |S(h)|, and change the values of Gi(j) for all nodes j ∈ (S(i) \ S(j)) individually. With another persistent segment tree, we can keep all segment trees G in O(n log2 n) preprocessing time.

With Gap and Gap−1 which represents the subtree of node ap and node ap−1 respectively, we can find the answer y that we are looking for. It is the unique integer that satisfies the following two inequalities:

∑i=1y Gap(i) − ∑i=1y Gap−1(i) ≥ m′

∑i=1y−1 Gap(i) − ∑i=1y−1 Gap−1(i) < m′

This solution runs in O(n log2(n) + q log2(n)) time for preprocessing the persistent segment trees and answering all questions.

# L XOR Operations

As a preprocessing step, let us replace each input integer ai with 2ai + 1. This alteration does not affect the result; however, it proves to be useful for subsequent calculations.

An d-bit integer can be represented by a row vector in (Z/2Z)d. For an input ai, let us denote the corresponding row vector in (Z/2Z)d by a~. The XOR operation a ⊕ aj corresponds to the addition a~i + a~j in (Z/2Z)d. Similarly, the sequence B can be represented as n row vectors in (Z/2Z)d. Now, the operation for indices i and j (b ← b ⊕ a~i ⊕ a~j and bj ← bj ⊕ a~i ⊕ a~j) can be considered as an addition of (0, . . . , 0, a~i + a~j, 0, . . . , 0) to B. Let us denote this row vector by v~. Note that the dimension of v~ is nd.

There are (n)i,j choices for i and j. The solution to this problem is the number of elements in the space spanned by the vectors v~i,j. Let M be a (n) × (nd) matrix such that each row corresponds to a vector v~i,j. The solution is equal to 2rank(M).

Thus, in summary, we only need to determine the rank of M.

The 2024 ICPC Asia Pacific Championship – Problem Analysis
To investigate the rank of M, for k ≥ 2, let

|<br/>a1 + ak|
|---|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/><br/>a1 + ak|
|a2 + ak|
|<br/>To investigate the rank of M, for k ≥ 2, let<br/><br/>a1 + ak|a2 + ak|a2 + ak| |a1 + ak|a2 + ak|a2 + ak| |

D = [Dk | Fk | O]. Also, let Mk be a matrix formed by stacking matrices R2, R3, . . . , Rk vertically. The matrix M as described above is equivalent to Mn. Without loss of generality, we can assume that s row vectors a1, . . . , as are linearly independent, and other vectors can be expressed as the sum of a subset of a1, . . . , as.

Observation L.1. rank(Ms) = s.

Proof. Suppose that ∑(i,j)∈C vi,j = 0 for some set of indices C ⊆ {(i, j) | 1 ≤ i < j ≤ s}. Let us examine the first d(i,j)∈C elements of the vector vi,j. In particular, (a1 + ai,j) = 0 must hold. Because a1, . . . , as are linearly independent, C cannot contain any element of the form (1, j) for j = 2, . . . , s. With the same arguments applied to other elements, we can deduce that C must be empty. This means that the row vectors in Ms are linearly independent, and therefore rank(Ms) = s.

Observation L.2. For k > s, rank(Mk) = rank(Mk−1) + s − 1.

From Observations L.1 and L.2, we can conclude that the rank of M is s + (n − s)(s − 1). Since the computation of s, the number of linearly independent vectors, can be performed in O(nd) time, the runtime is O(nd). (In this problem, d = 31.) The remaining part is the outline of the proof for Observation L.2.

Proof. First, we can show that the rank of Fk is s − 1. Because the rows of Fk are linear combinations of a1, . . . , as, rank(Fk) ≤ s. The rank can be further bounded by s − 1 because any combination of rows in F always contains an even number of a1, . . . , as. (Here, we utilized the result of the preprocessing, that a1, . . . , as+1 are represented by an odd number of sums of the basis a1, . . . , as.) Also, without loss of generality, we can assume that ak = a1 + . . . + at, where t ≥ 1 is an odd integer. Then, the s − 1 vectors a2 + ak, a3 + ak, . . . , as + ak are linearly independent. From this, we can conclude that rank(Fk) = s − 1.

Next, since the matrix Mk has the form of

Mk−1
O

Dk
Fk
we can say that rank(Mk) ≥ rank(Mk−1) + s − 1. To show that the inequality “≥” is actually an equality, we need to establish that the elements of Dk do not affect the rank of Mk. Suppose that there exists a subset C ⊆ {1, . . . , k − 1} such that ∑i∈C (ai + ak) = 0. That is, C represents the indices of rows in Fk whose sum equals 0. For our purposes, it suffices to show that there always exists a subset C′ ⊆ {(i, j) | 1 ≤ i < j < k} such that ∑i∈C vi,k = ∑(i,j)∈C′ vi,j.

When |C| is odd, because ak = ∑i∈C ai, C = {(i, j) | i, j ∈ C, i < j} satisfies the condition. Meanwhile, when |C| is even, because ∑i∈C ai = 0, a set C constructed from {1, . . . , t} × C satisfies the condition. This concludes the statement of the observation.

The 2024 ICPC Asia Pacific Championship – Problem Analysis
# Zig-zag

Here, we’ll only describe how to count even-length partitions of n such that a1 &lt; a2 &gt; a3 &lt; a4 &gt; a5 &lt; . . ..

There are only a few other kinds of partitions and they can all be handled similarly. Also, let N := 300 000.

Instead of counting partitions satisfying a1 &lt; a2 &gt; a3 &lt; a4 &gt; a5 &lt; . . ., it’s easier to count partitions satisfying a1 &lt; a2 ≤ a3 &lt; a4 ≤ a5 &lt; . . .. Let’s call even-length partitions satisfying the latter almost increasing. Once we have computed the latter, the former can be computed by inclusion-exclusion. Specifically, we’re counting partitions satisfying a1 &lt; a2 ? a3 &lt; a4 ? a5 &lt; . . . where each “?” is either “no condition”, or “≤”, and in the latter case, the whole term is weighted by −1 (because of inclusion-exclusion).

Formally, let the weight of an almost-increasing partition be (−1)k where k is the number of times “≤” appears, and let a(n) be the sum of the weights of all almost-increasing partitions of n. Then every partition satisfying “a1 &lt; a2 ? a3 &lt; a4 ? a5 &lt; . . .” is just a conjunction of some number k of almost-increasing partitions. Thus, the number c(n) of partitions of n satisfying a1 &lt; a2 &gt; a3 &lt; a4 &gt; a5 &lt; . . . is

∞

c(n) = ∑ ∑ a(n1)a(n2) . . . a(nk),

k=0 n ni≥0

1+...+nk=n

where the inner sum is actually a k-fold sum. The inclusion-exclusion is handled by the weights. Now, notice that this is a convolution and can be stated nicely in terms of generating functions! Let A(x) := ∑ a(n)xn and C(x) := ∑n≥0 c(n)xn. Then the above translates to

∞

C(x) = ∑ A(x)k = 1 − 1/A(x).

k=0

Thus, if we’re able to compute A(x), we’re also able to compute C(x) with a single generating function inverse in O(N log N).

The values a(n) for n ≤ N can be computed with DP by noticing that for an almost-increasing partition a of n, ai ≥ i/2 for all i, and thus its length is O(√n). Thus, we can compute them with DP by finding a recurrence for a(n, `), the sum of the weights of almost-increasing partitions with sum n and length `. Because n ≤ N and ` ≤ 2√N, with an O(1) transition this gives an O(N1.5)-time algorithm.

The 2024 ICPC Asia Pacific Championship – Problem Analysis
