Restatement

You have 2N seats in a line, grouped into N adjacent pairs: (1,2), (3,4), ..., (2N−1, 2N). Each seat holds either C (Computing), A (Arts), or . (empty). If two students sit within the same pair, they’re buddies. You can make moves of this form: if a student sits next to an empty seat, you can move the student into that empty seat (i.e., swap a student with a neighboring dot). You want to know if you can rearrange so that every C has an A as their buddy (i.e., every C must be paired with an A inside one of the N pairs). If yes, output any final arrangement that achieves this; else NO.

Key observations

1) What the move allows and an invariant

- Each move swaps a student with an adjacent dot. This “slides the hole” left/right by one. With at least one empty seat in the line, you can move the dots anywhere and slide students around arbitrarily as long as you preserve the left-to-right order of the non-dot characters.
- Therefore: the relative order of the letters when you delete all dots is invariant. You cannot reorder Cs and As (only shift them and insert dots between them).

2) Pairs constraint in the final layout

- In the final layout, a C is valid only if its buddy (the other seat in the same pair) is A. Thus in any pair:
  - Allowed: AA, AC, CA, A., .A, ..
  - Forbidden: CC, C., .C
- Because order of the non-dots is fixed, seating reduces to taking the fixed sequence of letters (Cs and As only) and deciding where the N pair boundaries go, possibly inserting dots between letters. The two letters occupying the two seats of the same pair must be consecutive letters from that fixed sequence (since you can’t skip letters and still preserve order). A single-letter pair is allowed only if that letter is A.

3) Core reduction: a matching on a path

- Let T be the sequence obtained from the input by deleting all dots (T consists only of Cs and As, in that order).
- In any valid final arrangement, each C in T must be paired with an adjacent A in T: either the A immediately to its left or to its right (because the two letters in a pair must be consecutive in T).
- Those chosen pairings must be disjoint (no letter used twice). This is exactly a matching on a path: vertices are positions 1..|T|, and there is an edge between i and i+1 if T[i] != T[i+1] (i.e., AC or CA). We need a matching that covers all C-vertices.

This gives a necessary and sufficient condition when at least one dot exists: there is a matching in this path graph that saturates all Cs.

4) Important special case: no empty seats

- If there are no dots, no move is possible. The only reachable arrangement is the initial one. So the answer is YES iff each C in the input is already buddy with an A in its own pair; otherwise NO.

Algorithm

We handle two cases.

Case A: No empty seats (E = 0)

- Simply check each pair (1,2), (3,4), ...:
  - If a pair contains C, the other seat in that pair must be A.
  - If any C violates this, output NO.
- Otherwise output YES and the original string (since we can’t move).

Case B: At least one empty seat (E ≥ 1)

- Step 1: Build T = the input string with all dots removed.
- Step 2: Find a maximum matching in the path defined on positions 1..|T| with edges between i and i+1 if T[i] != T[i+1] (i.e., AC/CA edges).
  - On a path, the standard left-to-right greedy is optimal: scan i = 1..|T|−1; if i and i+1 are both unmatched and T[i] != T[i+1], match them and skip to i+2; otherwise i++.
  - This runs in linear time and returns a maximum matching.
- Step 3: Check feasibility: every C must be matched. If any C remains unmatched, output NO.
  - This also implies #C ≤ #A (necessary), and it detects “structural impossibilities” like a “CCC” where the middle C has no adjacent A to pair with.
- Step 4: Construct a valid final layout that respects the order of T and the matching:
  - Walk through T from left to right, forming seat pairs in order.
  - If i is matched with i+1 by the matching:
    - Output the pair T[i] T[i+1] (either AC or CA).
    - Advance i by 2.
  - Else (i is unmatched). Since all Cs are matched, T[i] must be A:
    - If i+1 exists and is also an unmatched A, output AA and i += 2.
    - Else output A. (A followed by dot) and i += 1.
  - After placing all letters from T, if you have used fewer than N pairs, append enough .. pairs to reach exactly N pairs.
  - Concatenate all the pairs into the final string of length 2N.

Why this works

- Reachability when E ≥ 1: Because we never change the order of letters in T and there is at least one dot, we can slide the dot(s) to realize any target arrangement that preserves T’s order. Thus any string we construct as above is reachable.
- Necessity of the matching: In any valid final arrangement preserving order, each C must be in a two-letter pair with an adjacent A from T (consecutive in T), and such pairs must be disjoint. That is precisely a matching saturating all Cs. If such a matching does not exist, no sequence of moves can help.
- Sufficiency of the matching: The greedy maximum matching on a path is optimal. If it saturates all Cs, we can then place those CA/AC pairs. All remaining letters are As, which we then pack greedily as AA pairs wherever possible and A. if a single A is left. This uses at most N pairs because the total number of occupied seats is |T| ≤ 2N and we always form two-letter pairs whenever possible. Finally, we pad with .. pairs to reach N total pairs.
- Edge handling without dots: If E = 0, the only reachable configuration is the initial one, so the check on existing pairs is both necessary and sufficient.

Pseudocode outline (no code, just structure)

- Read N, S
- Count E = number of dots in S
- If E == 0:
  - For k = 0..N-1:
    - Let x = S[2k], y = S[2k+1]
    - If x == 'C' and y != 'A' => NO
    - If y == 'C' and x != 'A' => NO
  - If all pairs pass => YES, print S
- Else:
  - Build T = S with dots removed
  - Greedy matching on T:
    - used[i] = false; mate[i] = -1
    - For i from 1 to |T|−1:
      - If not used[i] and not used[i+1] and T[i] != T[i+1]:
        - used[i] = used[i+1] = true
        - mate[i] = i+1, mate[i+1] = i
        - i += 1 (skip next)
  - Check every i with T[i] == 'C' has mate[i] != -1; if any fail => NO
  - Build pairs list:
    - i = 1
    - while i ≤ |T|:
      - if mate[i] == i+1:
        - append pair (T[i], T[i+1]); i += 2
      - else:
        - assert T[i] == 'A'
        - if i+1 ≤ |T| and mate[i+1] == -1 and T[i+1] == 'A':
          - append pair ('A','A'); i += 2
        - else:
          - append pair ('A','.')
          - i += 1
    - while number_of_pairs < N:
      - append pair ('.','.')
  - Output YES and the concatenated pairs as a string of length 2N

Correctness on the samples

- Sample 1:
  - S = C.AAAC => T = CAAAC
  - Greedy matches (C,A) at positions (1,2) and (A,C) at (4,5), all Cs covered.
  - Leftover A at pos 3 => A. pair.
  - Result pairs: CA, A., AC => CAA.AC (pad with nothing). Matches sample (order within a pair may vary).
- Sample 2:
  - S = AAA.CCAC => T = AAACCAC
  - Greedy matches (A,C) at (3,4) and (C,A) at (5,6). C at position 7 remains unmatched => NO. Matches sample.
- Sample 3:
  - S = CA.A.C.A => T = CAACA
  - Greedy matches (C,A) at (1,2) and (A,C) at (3,4), all Cs covered. Leftover A at 5 => A.
  - Build pairs: CA, AC, A., plus one .. to reach N=4: final like CA..ACA. Matches sample (up to allowed variation).

Complexity

- Building T: O(2N)
- Greedy matching on a path: O(|T|) ≤ O(2N)
- Constructing the final arrangement: O(|T|) + O(N) padding
- Total time O(N), memory O(N). Fits easily in 1s and 1024 MB.

Why this handles all cases

- If moves are impossible (no dots), we correctly accept only if the initial pairs already satisfy the buddy rule for Cs.
- If there is at least one dot, any arrangement that preserves the C/A order is reachable; we reduce feasibility to existence of a C-saturating matching between adjacent unequal letters, which is both necessary and sufficient, and is efficiently checkable. The constructed layout preserves order, uses at most N pairs (never overflows, since we pack two-letter pairs whenever possible), and ensures every C shares its pair with an A.