Restatement

- You have n students with positive skills a1, a2, …, an. Students 2..n stand in a fixed order; you (student 1 with skill a1) may insert yourself anywhere (including both ends).
- You choose any k > 1 such that k divides n.
- After insertion, reading from left to right, the students are assigned to groups 1..k cyclically: positions 1,k+1,2k+1,… go to group 1; positions 2,k+2,2k+2,… to group 2; etc. Every group has exactly n/k students.
- The skill of a group is the sum of skills of its members. You want to minimize the ratio x_max / x_min over the choice of insertion position and k, where x_max and x_min are the largest and smallest group sums, respectively.
- Output the minimal ratio as an irreducible fraction p/q.

Key ideas

1) Model the insertion by a cut index z

- Let A[1..n-1] be the array [a2, a3, …, an] (1-based).
- Choose a cut z ∈ {0, 1, …, n-1}: z means you insert a1 after the first z elements of A (i.e., before A[z+1]; z=0 means you stand at the very left, z=n-1 means at the very right).
- Let k be fixed for the moment. After insertion, the positions are 1..n and group assignment is by position modulo k. Define r = (z mod k) + 1, i.e., the group index that receives a1 when you insert after z elements.

2) Express group sums in O(1) from precomputed residue prefixes

For fixed k define, for each residue j in {1..k}:
- P_j(t) = sum of A[i] over i = 1..t with i ≡ j (mod k). In words, the partial sum on residue class j among the first t elements of A.
- Let total baseline sums (A only, no a1) going to group j if we cycled A alone be B_j = P_j(n-1).

Then the sum of group j after inserting a1 after z elements (i.e., at r = (z mod k)+1) is:
S_j(z) = P_{j-1}(n-1) + [P_j(z) − P_{j-1}(z)] + [j = r] · a1,
with the convention that j−1 wraps to k.

Explanation:
- The first part P_{j-1}(n-1) is the baseline contribution from all of A that, after the cyclic shift created by placing a1 at position r, ends up in j.
- The bracket [P_j(z) − P_{j-1}(z)] adjusts for the fact that the first z elements of A were assigned before inserting a1; this increases the residue j contribution and decreases the residue j−1 contribution.
- If j = r, a1 is added to that group.

3) O(1) updates when we move the cut by one

We do not want to recompute S_j(z) from scratch for each z. Observe the incremental effect when moving z → z+1 (i.e., we move the insertion one position to the right, past A[z+1]).

Let v = A[z+1] and r = (z mod k) + 1, then:
- The a1 injection moves from group r to group r+1 (wrapping at k).
- The element v switches from the suffix to the prefix relative to the cut, which increases P_r(z) by v and increases P_{r}(z) − P_{r-1}(z) by v in S_r, while decreasing S_{r+1} by v accordingly.

The resulting O(1) update on the current group sums is:
- S_r ← S_r + v − a1
- S_{r+1} ← S_{r+1} − v + a1
- All other S_j stay the same.

This follows directly from the formula in point 2. It means we can sweep z from 0 to n−1 and maintain all S_j with only two array updates per step.

4) Initialization for each k

For each k dividing n:
- Compute baseline bucket sums from A by residues modulo k: base[j] = sum of A[i] with (i−1) mod k = j−1 (i.e., indices ≡ j mod k).
- The initial z=0 has r=1. Plugging z=0 into the formula gives:
  - S_1(0) = base[k] + a1
  - S_j(0) = base[j−1] for j = 2..k

Then sweep z from 0 to n−1, updating S as above.

5) Minimizing the ratio

At each z (for current k), let
- x_min(z) = min_j S_j(z)
- x_max(z) = max_j S_j(z)
Update the global best ratio with x_max(z) / x_min(z).

You must compute the minimum and maximum over k current numbers S_1..S_k after each O(1) update. The only two entries that change are S_r and S_{r+1}. To keep this fast:
- Maintain the current S_j in an array.
- Maintain two priority queues (heaps) with lazy deletion:
  - A min-heap of pairs (value, index)
  - A max-heap of pairs (−value, index)
  On every update of S_r and S_{r+1}, push their new pairs in both heaps. When querying the min (or max), pop heap tops while they are stale (heap.top.value != current S[index]). This gives amortized O(log k) per step with very small constants.

Fraction comparison and output:
- Track the best pair (Xmax, Xmin) and compare fractions via cross multiplication: Xmax1·Xmin2 ? Xmax2·Xmin1 using 128-bit or careful overflow-safe comparison.
- At the end, reduce best Xmax/Xmin by gcd to output p q.

Pseudocode

For each test case:
- read n, a1
- read A[1..n−1] = [a2..an]
- enumerate all divisors k > 1 of n
- best_num = +infinity, best_den = 1

For each k in divisors(n), k > 1:
  // baseline sums by residue
  base[1..k] = 0
  for i = 1..n−1:
    r = ((i−1) mod k) + 1
    base[r] += A[i]

  // initial S for z = 0
  S[1] = base[k] + a1
  for j = 2..k:
    S[j] = base[j−1]

  // initialize two heaps with (value, index), and an array S
  push all (S[j], j) into min-heap and max-heap (max stores (−S[j], j))

  for z = 0..n−1:
    // query current min and max with lazy deletion
    m = current_min_from_heap(min-heap, S)
    M = current_max_from_heap(max-heap, S)

    // update best fraction M/m
    if M * best_den < best_num * m:
      best_num = M
      best_den = m

    if z == n−1: break
    v = A[z+1]
    r = (z mod k) + 1
    r2 = (r == k ? 1 : r+1)

    // apply O(1) update
    S[r]  += v − a1
    S[r2] += −v + a1

    // push new values into heaps (lazy deletion will handle stales)
    push (S[r],  r) into min-heap; push (−S[r],  r) into max-heap
    push (S[r2], r2) into min-heap; push (−S[r2], r2) into max-heap

After processing all k:
- reduce best_num/best_den by gcd and print p q

Why this works (correctness)

- The grouping is determined solely by position modulo k along the final array after you insert a1. Choosing the insertion “cut” z determines r = (z mod k)+1, the group receiving a1, and it also shifts the cyclic assignment for all subsequent elements by one.
- The derived identity
  S_j(z) = P_{j−1}(n−1) + [P_j(z) − P_{j−1}(z)] + [j = r]·a1
  is exact. It follows from:
  - among A’s positions, each residue class j contributes P_j(n−1) overall;
  - when the cut is at z, the first z elements of A keep their original residue assignment; the suffix’s assignment is shifted by one, which swaps amounts between adjacent residue classes, producing the difference term P_j(z) − P_{j−1}(z);
  - the group r additionally receives a1.
- The incremental update when z → z+1 (two groups change by ±A[z+1], and a1 moves from r to r+1) is an immediate consequence of the above formula; hence the sweep over z enumerates all admissible positions and computes all group sums correctly.
- For each fixed k, the sweep considers all possible insertion places. Taking the best ratio across z yields the optimum for that k. Taking the minimum over all divisors k yields the global optimum.

Complexity and feasibility

- Let τ(n) be the number of divisors of n.
- For each k:
  - Building base[] takes O(n) time.
  - Sweeping z from 0 to n−1 performs O(1) arithmetic updates per step and two heap insertions; querying min and max is amortized O(log k) per step due to lazy deletion.
  - Total per k: O(n log k).
- Summed over all k | n: O(n ∑_{k|n} log k). With n ≤ 10^6, τ(n) ≤ about 240, and log k ≤ ~20. The total number of element updates is O(n τ(n)); each update changes only two entries, so constant factors are small. With efficient I/O and memory reuse, this fits within the time limit in practice for the given constraints (sum of n across tests ≤ 10^6).

Implementation notes

- Use 64-bit integers for sums; worst-case group sums are around (n/k) · 1000 + a1 ≤ 10^9 + 1000.
- Compare fractions x_max/x_min via cross-multiplication to avoid floating-point error.
- After selecting the best pair (Xmax, Xmin), reduce by gcd(Xmax, Xmin) to print p q.
- Enumerate divisors of n by scanning up to √n and adding both d and n/d; skip k=1.

Why the ratio is minimized

- The total sum T = a1 + a2 + … + an is fixed; the average group sum is T/k. As z varies, the vector (S_1, …, S_k) stays near this average but shifts mass only between two adjacent groups at each step (plus the a1 move). The sweep enumerates all possible such redistributions, thus seeing the minimum of the continuous piecewise-linear ratio function across discrete z.
- Since all skills are positive, there is no degeneracy (x_min > 0 always), and the best ratio among the finitely many candidates is achieved at some z and k; the algorithm checks them all.

Edge cases covered

- k = n: each group has one student; the sweep yields ratio max(a)/min(a).
- Uniform skills: any k and z give equal sums; ratio 1.
- a1 very large/small: the sweep places a1 optimally (most often into the smallest group) because we check all z.

Summary

- Reduce the problem to sweeping a “cut” z for each divisor k. Maintain group sums S_1..S_k. Moving the cut by one affects only two groups and the position of a1; we can update in O(1).
- Maintain current min and max via two heaps with lazy deletion to evaluate x_max/x_min at each z in amortized O(log k).
- Take the best ratio across all z and k and output the irreducible fraction.