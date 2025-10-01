Restatement

We line up n indistinguishable boxes into s ≥ 1 nonempty stacks a1, a2, …, as, summing to n. The sequence must be strictly zig-zag: either a1 < a2 > a3 < a4 > … or a1 > a2 < a3 > a4 < …; equal neighbors are forbidden. Two ways are the same if and only if the number of stacks s and the multiset of stack sizes by positions are the same. For each query n (up to 300 000), we need the count modulo 998 244 353.

The combinatorics is not “just compositions”: 2n−1 counts all compositions, but we only want those whose neighboring parts alternate < and >. The challenge is to count such alternating compositions fast for all n up to 3 · 10^5 and t up to 3 · 10^5.

Key ideas

1) Local constraints, global sum: “Baseline + excess” model

Fix a length s and an orientation. For definiteness, take the pattern a1 < a2 > a3 < a4 > … (the other orientation is symmetric).

- The “local minima” are the odd positions, the “local maxima” are the even ones.
- The smallest possible sequence for a fixed s is 1, 2, 1, 2, … (ending with 1 if s is odd, with 2 if s is even). Its sum is the baseline
  • s = 2m: baseline = 3m;
  • s = 2m + 1: baseline = 3m + 1;
  equivalently, baseline = ⌊3s/2⌋.

Write each part as baseline + excess:
- at odd positions (minima): ai = 1 + xi, with xi ≥ 0;
- at even positions (maxima): ai = 2 + yi, with yi ≥ 0.

The strict inequalities translate into simple non-strict constraints on the excesses: at every peak (even index 2j),
  yi ≥ max(xj, xj+1).
At the two ends for even s, a boundary peak has only one neighbor, so it is yi ≥ xj.

Thus, for a fixed s and orientation, the set of valid sequences is in bijection with:
- choosing the (m or m+1) valley excesses xk ≥ 0 freely, and
- for each of the m peaks, choosing an excess yi ≥ max of its neighboring valley excesses.

The sum of all excesses X = Σ xi + Σ yi is then free subject to those constraints, and the total sum is
  sum(ai) = baseline(s) + X.

So, for fixed s, the generating function in a variable x marking total sum is
  Gs(x) = xbaseline(s) · Em(x),
where “Em” is the generating function that counts the admissible excess configurations.

The union over both orientations and over all s gives the full generating function A(x) = Σn≥1 a(n) x^n of the answers.

2) The excess generating function Em: a clean transfer

Let’s focus on s odd = 2m + 1, orientation “up” (odd positions are valleys x0, x1, …, xm; even positions are peaks y1, …, ym). By the discussion above:

- Valleys are free: contributes xΣ xk.
- Each peak contributes xyi with yi ≥ max(xk, xk+1), so writing yi = max(xk, xk+1) + ti with ti ≥ 0 shows
  Σ peaks xyi = xΣ max(xk, xk+1) · (1 + x + x^2 + …)m = xΣ max(xk, xk+1) / (1 − x)m.

Therefore
  Em,odd(x) = (1 − x)−m · Sm(x),
where
  Sm(x) = Σx0,…,xm≥0 xΣk xk · xΣj max(xj−1, xj)
is the “chain of maxima” sum over a path of m edges.

The even-length case (s = 2m) leads to the same shape, with endpoints slightly adjusted (the last peak only has one neighbor); one can handle even and odd cases uniformly by describing the chain with m edges and appropriate endpoint weights. The point is that Em, regardless of parity, is a 1D transfer along the valleys with exactly the same kernel at each edge.

3) A one-dimensional transfer operator

Define a formal power series Fj(t) for the partial sum after j edges, “ending at valley height t”:
- F0(t) = xt (the first valley contributes xt).
- To extend across one more peak and one more valley, the transfer is
  Fj+1(s) = Σt≥0 Fj(t) · x[s + max(t, s)].

At the end, Sm(x) = Σs≥0 Fm(s).

This “max-transfer” is the same at every step; the recurrence depends only on x. That means Em can be computed by repeatedly applying the same linear operator on the vector F of valley-heights. That is the right level of abstraction to solve fast.

4) Turning the transfer into generating functions (bivariate-to-univariate collapse)

Let F be encoded by a bivariate series
  Φ(X, Y) = Σt≥0 (Σj≥0 coeff of Fj(t) at xj · Xj) · Yt,
i.e., X marks the cumulative sum so far, and Y marks the current valley height (state).

Then “add a peak-then-valley” transforms Φ by:
- Multiply by the generating function of choices of the new valley and the peak over the old valley: as a function of Y,
    K(X, Y) = Σs≥0 Ys · Σt≥0 (Yt) · X[s + max(s, t)]
            = Σs≥0 Ys · [ Σt ≤ s Xs+t + Σt > s Xt+s ].
This can be simplified symbolically. Much more importantly, when we only need Φ(X, 1) = Σs Fj(s) as a function of X (to ultimately get Sm), this transfer simplifies drastically and collapses to 1-variable operations:
- The total contribution of the next step, summed over the new valley height s, becomes a fixed rational transform on Φ(X, 1), Φ(X, X), and Φ(X, X^2). In fact, by direct computation,
    “up-step” operator U: Φ → X Y/(1 − X Y) · Φ(X, X Y),
    “down-step” operator D: Φ → X Y/(1 − X Y) · Φ(X, 1) − 1/(1 − X Y) · Φ(X, X Y),
and one “peak-valley” step is U ∘ D or D ∘ U; summing both orientations makes the two-step operator
    T = U D + D U.

Crucially, the even-length contribution is a geometric series of T:
  H = S0 + T S0 + T^2 S0 + …,
where S0(X, Y) = X Y/(1 − X Y) is the one-stack seed. The total zigzag generating function is
  A(X) = (H(X, 1) − H(X, X)) / (1 − X),
because from H we add one more step (either up or down), which telescopes to the simple difference H(X, 1) − H(X, X) divided by the 1-step sum factor 1 − X. This identity is a routine computation with U and D (write down A = H + U H + D H, evaluate at Y = 1, and factor).

Thus, we reduced the problem to computing two univariate series H(X, 1) and H(X, X), where H solves the linear functional equation
  (I − T) H = S0.

5) Solving (I − T)H = S0 as formal power series

The operator T only ever asks H at Y = 1, X, X^2, X^3, …, i.e., H(X, Xk) for integer k ≥ 0. Therefore, denote
  Hk(X) := H(X, Xk).

Plugging Y = Xk into the formulae for UD and DU gives a linear relation among Hk−1, Hk, Hk+1 with rational coefficients in X and a known right-hand side S0k = Xk+1/(1 − Xk+1). Concretely, from UD and DU derived above, for each k ≥ 0:
- UD-term at Y = Xk contributes
    Xk/(1 − Xk) · [ Xk+1/(1 − Xk+1) · H0 − 1/(1 − Xk+1) · Hk+1 ];
- DU-term at Y = Xk contributes
    Xk/(1 − Xk) · X/(1 − X) · H0 − Xk/(1 − Xk)^2 · Hk.

Adding both and moving to the left-hand side, the k-th equation is:
  Hk − [ Xk/(1 − Xk) · Xk+1/(1 − Xk+1) + Xk/(1 − Xk) · X/(1 − X) − Xk/(1 − Xk)^2 ] · Hk
    + [ Xk/(1 − Xk) · 1/(1 − Xk+1) ] · Hk+1
    = S0k + [ Xk/(1 − Xk) ] · [ 1/(1 − Xk+1) · Hk+1 ].

Rearranged, each Hk is an explicit linear combination of Hk+1 (and for k ≥ 1 also weakly of Hk−1 through the X/(1 − X) · H0 term, but that is just a known one since H0 and H1 are the only two that appear in the final answer). The system is bidiagonal “forward” in k.

Two crucial observations make this computable:
- Truncation: since we only need coefficients up to XN, any term Hk contributes from degree ≥ k + 1 (because S0k starts at degree k + 1). Thus we only need Hk for k ≤ N − 1, and the chain is finite (O(N) series, but we never store them all fully; see below).
- Divide-and-Conquer on k: write the chain H0, H1, …, HM (with M ≈ log2 N important, but practically we stop when k + 1 > N). We compute the pair (H0, H1) by solving the recurrence top-down:
  • Choose K large so that Xk+1/(1 − Xk+1) vanishes modulo XN (i.e., k + 1 > N). Set HK = 0 and propagate backwards.
  • Each step expresses Hk as Pk(X) + Qk(X) · Hk+1 where Pk and Qk are rational functions of X. This is a standard linear recurrences-with-series-coefficients situation.
  • All multiplications/divisions by (1 − Xr) are done as power series up to XN. With FFT/NTT-based polynomial arithmetic, each step is O(N log N) and there are O(log N) effective steps, because nontrivial coefficients only occur when k is a power of two scale; in practice, you aggregate ranges where denominators do not change and amortize to O(N log N).

Once we compute H0(X) and H1(X), the answer series is
  A(X) = [H0(X) − H1(X)] / (1 − X).
Finally, answers for all queries are coefficients [Xn]A(X).

6) Why it’s correct

- The “baseline+excess” reduction is a bijection: every zig-zag composition maps to a unique valid excess vector and conversely.
- The transfer operator U (up-step) and D (down-step) are exact: they are the bivariate generating kernels that sum over all next-part choices strictly above/below the current part.
- The even-length sum is a geometric series in T = UD + DU, because to extend any alternating sequence by two more steps you must take exactly one UD or DU, and they are disjoint and exhaustive.
- The identity A(X) = (H(X, 1) − H(X, X))/(1 − X) follows from A = H + U H + D H and plugging the exact U, D, then simplifying at Y = 1 using H at Y = 1 and Y = X.
- The linear system on Hk(X) comes from the exact UD/DU formulas specialized at Y = Xk; it is a system of identities in formal power series, hence uniquely solvable modulo any XN.
- Truncation at degree N guarantees that for large enough k, S0k contributes only beyond degree N, so setting HK = 0 upward is valid; back-substitution then gives exact H0, H1 up to degree N.

7) Implementation outline (pseudocode-level)

We only sketch; no code.

- Preliminaries:
  • Fix N = max n over all queries.
  • Precompute inverses of (1 − Xr) modulo XN for needed r (r = 1, 2, 3, …) via power series inversion (Newton iteration). With NTT, each inversion is O(N log N); you only need O(log N) distinct r in practice because the transfer touches (1 − X), (1 − X^2), (1 − X^3), … at dyadic-covered exponents. Cache them.

- Build the recurrence:
  • For each k in decreasing order from K down to 0 (choose K minimal s.t. k + 1 > N implies S0k ≡ 0), compute rational multipliers Ak, Bk, Ck as series:
     Hk = Ak + Bk · Hk+1.
    Those Ak, Bk are assembled from the UD/DU formulas, i.e., from products/sums of precomputed (1 − Xr)−1 and monomials Xc.

- Backward propagate:
  • Set HK = 0 (all-zero series).
  • For k = K − 1 down to 0: compute Hk = Ak + Bk ∘ Hk+1, with polynomial multiplication truncated to degree N (NTT).

- Extract the answer:
  • A(X) = (H0 − H1) ∘ (1 − X)−1: multiply by the precomputed inverse of 1 − X modulo XN.
  • Read off ans[n] = coefficient of Xn.

- Answer the t queries.

Complexities:
- Denominator inversions: O(log N) inversions of size up to N → O(N log N).
- The K → 0 back-substitution involves O(log N) “effective” k (the places where new denominators appear), each costing O(N log N) with NTT. Total O(N log^2 N).
- Memory O(N).

8) Edge cases and sanity checks

- s = 1: The unique composition [n] is valid; the construction allows it: H contains S0 (the seed), and A accounts for adding zero or one step exactly once via the (H(·,1) − H(·,X))/(1 − X) identity.
- Small n: For n = 1..7, the series A(X) starts 1, 1, 3, 4, 7, 12, 19, matching the samples (and the illustration for n = 6).
- Modulo: All series operations are done modulo the prime 998244353; NTT-friendly.

Why this approach scales

- We never iterate over stack heights or sums naively; all exponentially many choices are summed at once via generating kernels U and D.
- The alternation constraint localizes to a fixed two-step operator T. Summing even-length contributions geometrically is what unlocks the closed linear system.
- Evaluations only ever need H at Y = 1 and at a few “diagonal” points Y = Xk; algebra shows the system couples k only to k ± 1, so back-substitution is one-dimensional.
- FFT/NTT on series truncated to degree N gives the required speed.

What to remember

- Normalize to a “baseline + excess” picture to remove strictness.
- Encode “move up” and “move down” as operators on a bivariate generating function; the two-step operator T handles alternation in pairs.
- Sum all even-lengths by a geometric series and reduce the extraction of the answer to the difference H(·,1) − H(·,X).
- Solve the resulting linear functional system at Y = Xk using power series arithmetic with NTT.