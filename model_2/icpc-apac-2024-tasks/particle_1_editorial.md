Restatement

We have a one-dimensional container of antiprotons (A) and protons (P), represented by a 1-indexed string. We can apply four operations:

- +P i: if position i is P, replace it by APA
- +A i: if position i is A, replace it by PAP
- -A i: delete a consecutive block of a A’s starting at i
- -P i: delete a consecutive block of p P’s starting at i

Given a, p (5 ≤ a, p ≤ 20), an initial string S and a target string E (1 ≤ |S|,|E| ≤ 50), decide if S can be transformed into E. If yes, output any sequence of at most 35,000 operations doing so; otherwise print -1.

Key observations and invariants

1) Letter-count congruences are the only global invariants:
- Each +P adds exactly 2 A’s.
- Each +A adds exactly 2 P’s.
- Each -A removes exactly a A’s.
- Each -P removes exactly p P’s.

Therefore
- The value of #A modulo gcd(2, a) is invariant.
- The value of #P modulo gcd(2, p) is invariant.

Since gcd(2, x) = 1 if x is odd and 2 if x is even, the necessary feasibility conditions reduce to:
- If a is even then parity(#A(S)) must equal parity(#A(E)).
- If p is even then parity(#P(S)) must equal parity(#P(E)).

There are no further global invariants to worry about.

2) Local “tail-extender” effect (the crucial gadget):
Look at the last two letters of the current string. Suppose the string ends with … X Y (X is the penultimate letter, Y the last), and consider applying +X to that penultimate X:

… X Y  →  … (Y X Y) Y  →  … Y X Y Y

What happens at the right end is:
- We append one Y to the very end (the rightmost Y).
- We also get one Y just to the left of the pivot X (that is, we grow a contiguous Y-block “spool” immediately before X).

If we then repeatedly apply +X always to that same penultimate X, every application:
- Appends one more Y to the end, extending the rightmost Y-run by +1, and
- Increases the size of the contiguous Y “spool” immediately left of that X by +1.

When the “spool” reaches size a (if Y = A) or size p (if Y = P), we can delete the spool in one shot using -A or -P, respectively. Importantly, this deletion does not touch the last characters we appended at the very end, since the spool lies on the left of the pivot X.

This simple, local behavior yields two powerful capabilities:
- We can append any run Y^m to the right end, in exactly m applications of +X on the penultimate X (plus occasional deletions of full spools).
- While doing so, all “mirror artifacts” (the spool) remain contiguous and disposable in bulk.

This will be the engine for both “writing” E at the right end and “cleaning” garbage to the left.

High-level plan

- First check the necessary parity conditions. If they fail, answer -1.
- Otherwise, we deterministically construct E from S with two phases:
  A) Append E (run by run) to the right end of the current string using the tail-extender gadget; delete spooled blocks as they reach size a or p.
  B) After E has been appended, erase everything to its left (the original S plus any leftover spools) using the same tail-extender/deletion trick, without touching E.

Why this works and why it is enough:

- Phase A writes E from left to right, but done run-wise:
  - We decompose E into its RLE (run-length encoding) runs, E = c1^{e1} c2^{e2} … ct^{et}, where ci ∈ {A, P} and ci ≠ c_{i+1}.
  - Before appending the next run L^m, we make sure the last letter of the current string is Opp(L). If it is already Opp(L), good. If it is L, a single +L at the very end turns the last letter into Opp(L) (details below). Then we repeatedly apply +Opp(L) on the penultimate Opp(L), exactly m times, to append L^m; in parallel we delete the spool of L to the left whenever it reaches size a (if L = A) or size p (if L = P).
  - After t runs, the rightmost portion of the string is exactly E.

- Phase B empties the prefix left of E:
  - Let the string be PFX | E, where | marks the boundary (the leftmost position of E).
  - Look at the last one or two letters of PFX. Using 1–2 local + operations, we can always form a penultimate/last pair (X,Y) before | such that +X at the penultimate position creates and grows a Y-spool right next to |. Once that spool reaches size threshold (a if Y=A, p if Y=P), delete it. This peels off a whole block of PFX without touching E.
  - Repeat until PFX disappears.

Every + and - we use is a valid operation. The deletions always apply to contiguous spools we ourselves grew. The construction never needs to look far, only at the last two symbols before the right end (for Phase A) or before current E boundary (for Phase B). Operation counts are bounded comfortably below 35k due to small |S|,|E| and a,p ≥ 5.

Details and careful steps

0) Feasibility check
- If a is even and parity(#A(S)) ≠ parity(#A(E)), print -1.
- If p is even and parity(#P(S)) ≠ parity(#P(E)), print -1.
- Otherwise proceed.

1) Notation for the tail extender
- Let T be the current string of length n ≥ 1.
- For a letter L, Opp(L) denotes the other letter.
- We will often need to “ensure the tail letter is Opp(L)”. If the last letter already is Opp(L), do nothing. If the last letter is L, just apply +L at position n (the last position). That replaces T[n] = L by “Opp(L), L, Opp(L)”; in particular, the new last letter becomes Opp(L). So we can align the last letter with Opp(L) using at most 1 operation.

2) Appending one run L^m at the right end
- Precondition: The last letter of T is Opp(L). Let its index be n (current length).
- Invariant we maintain during the loop:
  - The penultimate letter (index n) is Opp(L). The last letters to the right (indices > n) form the L-run we are building.
  - Immediately to the left of that penultimate position there is a contiguous spool of L’s (possibly empty), whose size we track modulo a (if L=A) or modulo p (if L=P).
- Loop m times:
  - Apply +Opp(L) at index n (the penultimate Opp(L)). Effect:
    - One L is appended at the very end (right run grows by +1).
    - The L-spool just left of index n grows by +1 (remains contiguous).
    - The penultimate position is still an Opp(L) (because +Opp(L) replaces one Opp(L) with “L Opp(L) L”, so the middle of that triple is still Opp(L)).
  - If the L-spool reaches threshold (a if L=A, p if L=P), delete it using -L at the spool’s leftmost index; the run we are appending on the right is not touched.
- After m iterations, we have appended L^m.

3) Appending all runs of E
- Compute the run-length decomposition E = c1^{e1} c2^{e2} … ct^{et}.
- For i = 1..t:
  - Make the last letter of the current string Opp(ci) (using 0 or 1 step as described in 1)).
  - Append the run ci^{ei} by the loop in 2).
- At this point, the rightmost |E| characters of T equal E exactly.
- The left part of T (prefix) equals the original S plus the net effect of spools after the last deletions; in any case it sits entirely to the left of E.

4) Erasing the entire prefix left of E, without touching E
- Let | mark the boundary between prefix and E. We want to repeatedly remove contiguous blocks right to the left of |.
- Look at the final characters before |. There are two easy subcases (we show how to ensure a usable pair and then peel):
  - If the last letter before | is Y and the one before it is X = Opp(Y), we can directly apply +X at that penultimate X to grow a contiguous Y-spool right against |. When the Y-spool reaches size threshold (a if Y=A, p if Y=P), apply -Y to delete it; this removes a whole block right at the boundary, moving | left.
  - If the last two letters before | are equal (say Y Y), apply +Y at the last Y (immediately before |). That replaces that Y by “X Y X” with X = Opp(Y), so now the last two letters before | are “X Y”. You are now in the previous subcase and can peel a Y-block.
- Each peel removes at least a or p letters from the prefix. Iterate until the prefix disappears completely.
- None of these operations cross into E: the + is applied strictly inside the prefix, and the - deletes a contiguous block of the last letters of the prefix.

Pseudocode sketch

For one test case:

- if (a even and (#A(S) % 2 != #A(E) % 2)) or (p even and (#P(S) % 2 != #P(E) % 2)):
    print -1
    continue

- ops = empty list
- T = S

- function ensure_tail_is(letter X):
    if T.last == X: return
    // T.last is Opp(X); do nothing
    // If T.last is not X, but we need Opp(X) instead:
    // If we need last to be Opp(X), but last is X:
    // Single +X at last makes last become Opp(X)
    // (record "+X n"; update T accordingly)

- function append_run(L, m):
    ensure_tail_is(Opp(L))
    repeat m times:
        // Let i be the index of the last Opp(L), i = current length - current L-run size
        // Apply +Opp(L) at i
        // Track the size of the L-spool just to the left of i
        // If the L-spool size hits threshold (a for A, p for P),
        //    apply -L at its leftmost position; reset spooled count modulo threshold
        // (record each operation; update T)

- // Phase A: write all runs of E
- RLE E as (c1,e1),...,(ct,et)
- for each (ci, ei):
    append_run(ci, ei)

- // Phase B: erase prefix completely
- while there is any letter left of the first position of E:
    // Let Y be the last letter before E
    // Let Z be the letter before Y if it exists; else:
    //   If there is only one letter Y before E, do +Y at that Y to create Z=Opp(Y) and Y again.
    if Z == Y:
        // Make (Z,Y) into (Opp(Y),Y) with one +Y at Y
        // Now Z = Opp(Y)
    // Now we have ... Opp(Y) Y | E
    // Grow a Y-spool immediately to the left of | using +Opp(Y) at that Opp(Y) repeatedly,
    // then delete it with -Y when spool reaches threshold (a or p).
    // Each deletion peels off a large chunk of the prefix.

- Output ops (they are at most on the order of |S|+|E| plus a few dozens of - operations and their preparatory +’s).

Why this is correct

- The only global obstructions are the parity congruences modulo gcd(2, a) for A and gcd(2, p) for P. If these hold, there is no counting barrier; our construction never violates them, and it ends exactly at E.

- The tail-extender effect is local and robust:
  - It appends exactly one copy of the target letter to the right end at each step in Phase A, so we get E exactly.
  - The mirrored letter always lands in a single contiguous “spool” immediately to the left of the pivot we control. Therefore, whenever the spool reaches the deletion threshold, we can delete it in a single - operation with no side effects elsewhere.

- Cleanup does not touch E:
  - In Phase B we always apply + strictly inside the left prefix to produce a contiguous spool closing up to the boundary |, then - removes that spool. Neither step crosses into E.

- Edge cases:
  - If the string has length 1 while appending a run, applying + at that single position still creates the needed penultimate/last pattern; the same loop then continues as before.
  - If you see a single letter before E during cleanup, one + on it creates the necessary two-letter tail to start peeling.

Complexity and operation bound

- Appending E uses exactly |E| many + operations (one per appended letter), plus at most 1 extra + at the start of each run (to align the tail with Opp(run-letter)). E has at most |E| runs, and in practice far fewer—worst case alternating gives ~|E| runs so worst-case + overhead is ≤ |E|. Additionally, each time the spool size hits its threshold, we pay one - operation. With a, p ≥ 5 and |E| ≤ 50, there are at most 10–11 such deletions.

- Cleanup: Each - deletes at least min(a, p) ≥ 5 letters from the prefix. The total number of deletions is at most ceil((|S| + leftover spools)/min(a, p)), i.e., at most around 14. Each - is preceded by at most threshold-1 local +’s (≤ 19) to grow the spool. So cleanup needs at most a few hundred operations in the worst case.

- Altogether, we are in the very low thousands in the absolute worst corner cases, well below the limit 35,000.

How to produce the actual indices

- All + operations in Phase A are at the current penultimate position (the last occurrence of the pivot letter Opp(L) before the current right run). That index is either the last position or the second to last, updated dynamically as the string grows. It is easy to maintain: after each +, the pivot Opp(L) remains in the middle of the triple we just created.

- All - operations act on the leftmost index of the current contiguous spool next to the pivot. We maintain the current spool’s left boundary index and its size modulo threshold; when it reaches the threshold, we output -A or -P at that left boundary.

- In cleanup, the + operations are applied to the last or penultimate letter before | as described, and - is applied to the contiguous spool ending at |.

Proof sketch of the tail-extender claim

If the current tail is “… X Y” and we apply +X at that X, it is replaced by “Y X Y”. So the tail becomes “… Y X Y Y”. The rightmost Y is appended; the left Y is adjacent to whatever was before X. If we keep applying +X at the same penultimate position X, every time we replace that X by “Y X Y” again; hence the rightmost character grows by one Y, and a contiguous Y-block accumulates just to the left of X. That block is the spool. Because all Y’s added to the left land adjacent to the previous Y-block, they remain contiguous and thus deletable as soon as it reaches the threshold.

Why the parity checks are necessary

The only ways to change the total number of A’s are +P (+2 A) and -A (-a A). Thus #A changes by combinations of +2 and -a, so #A mod gcd(2, a) never changes. Similarly, #P mod gcd(2, p) never changes. These are necessary conditions; the constructive procedure shows they are sufficient.

Applying to the samples

- The sequences in the sample outputs are concise instances of Phase A (growing the needed letters) and Phase B (peeling with -) around small values of a and p. The first sample just uses +P several times (appending A’s by extending at a trailing P pivot) to achieve the desired target; the fourth sample does a short +A to prepare and a -P to peel a P-block in one shot, illustrating the cleanup idea.

Summary

- Compute the parity checks; if they fail, print -1.

- Otherwise, transform S to E by:
  - Appending runs of E to the right using the tail-extender (+ at the penultimate position), interleaving deletions of full spools.
  - After E is in place, erase everything left of E by creating spools against its left boundary and deleting them.

- All steps are local, always valid, and keep the length moderate; the total operations are comfortably below the allowed 35,000. This provides both a correctness argument and a straightforward way to generate a valid sequence of operations for any feasible test case.