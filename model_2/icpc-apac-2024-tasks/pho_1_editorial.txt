Restatement

- There are n tables. Table i initially has a binary string Si, whose length is the number of contestants at that table.
- A 0 means the contestant wants pho tái, a 1 means pho chín.
- You may move any contestants between the existing tables (unlimited capacity). After moving, each table must be empty or contain only 0s or only 1s.
- Goal: minimize the total number of contestants you move.

Key ideas

1) Minimize moves = maximize “stays”
- Every contestant either stays at their original table or moves once. So the number of moves equals total contestants − number of contestants who remain at their original table.
- Therefore, if we can maximize the number of people who stay at their original table, we minimize the number of moves.

2) What does it mean to “stay”?
- In the final arrangement, each table must be assigned a single dish: either 0-only or 1-only (or be empty).
- Let zi = number of 0s at table i; oi = number of 1s at table i.
- If table i ends up as a 0-table, we can keep at most zi contestants there (the 0s). If it ends up as a 1-table, we can keep at most oi. If it’s empty, we keep 0 there.
- There is no capacity limit, so moved contestants can be gathered at any chosen tables of their dish.

3) Ignoring global feasibility for a moment
- If we decide the final dish of each table independently to maximize stays per table, we would pick the better of zi and oi for that table. So the sum of stays would be sum_i max(zi, oi).
- Choosing “empty” is never better than choosing the better dish, so we can ignore “empty” in the optimization.

4) The only global feasibility constraint
- If there exists at least one 0-contestant and at least one 1-contestant overall, then in the final configuration we must have at least one 0-table and at least one 1-table to seat them. Otherwise, contestants of the missing dish would have nowhere to go.
- If all contestants are of one dish, the answer is 0 (we can make every table that dish; in fact they already are).

5) When do we need to “pay” extra to ensure both dishes exist?
- Let:
  - has0Major = there exists a table i with zi > oi (its local majority is 0).
  - has1Major = there exists a table i with oi > zi (its local majority is 1).
  - hasTie = there exists a table i with zi = oi.
- If both has0Major and has1Major are true, we can pick at least one 0-table and one 1-table while keeping each table at its local majority. No extra loss.
- If hasTie is true, we can assign one tie table to 0 and another tie or majority-1 table to 1 (or vice versa) without losing anything on the tie table. No extra loss.
- The only problematic case is when every table strictly favors the same dish and there are no ties (e.g., oi > zi for all i). Then independently maximizing stays gives only that one dish everywhere, but we must force at least one table to the other dish. Doing so on table i reduces stays there by |zi − oi|. To minimize the loss, choose the table with minimal |zi − oi|.

Putting it together

- Let T = total number of contestants = sum_i (zi + oi).
- Let S = sum_i max(zi, oi).
- Let minDiff = min_i |zi − oi|.
- Let Z = total 0s overall; O = total 1s overall.

Then:
- If Z = 0 or O = 0, answer = 0.
- Else if hasTie or (has0Major and has1Major), answer = T − S.
- Else (all tables strictly favor the same dish), answer = T − S + minDiff.

Why is this optimal?

- Lower bound: For each table i, at most max(zi, oi) can stay if the table is uniform. Summing over tables gives at most S stays, hence at least T − S moves. If all tables strictly favor the same dish with no ties, at least one table must be assigned to the minority dish; that reduces the stay count by at least minDiff, so the lower bound becomes T − S + minDiff.
- Achievability: Construct the final labels as above. If both dish types are present and either there is a tie somewhere or both majorities exist, label each table by its per-table best dish (resolving ties as needed to ensure at least one of each dish). Keep exactly max(zi, oi) at table i; move the rest to any tables of their dish. This uses exactly T − S moves. In the all-majority-same, no-tie case, pick the table with minimum |zi − oi| and flip it to the minority dish; we lose exactly minDiff additional stays, so moves = T − S + minDiff. Unlimited capacity makes routing the moved contestants trivial.

Pseudocode sketch

- Read n.
- Initialize:
  - T = 0, S = 0, Z = 0, O = 0
  - has0Major = false, has1Major = false, hasTie = false
  - minDiff = +infinity
- For each table i:
  - Count zi = number of ‘0’ in Si; let len = |Si|; oi = len − zi
  - Update:
    - T += len; Z += zi; O += oi
    - S += max(zi, oi)
    - if zi > oi: has0Major = true
      else if oi > zi: has1Major = true
      else: hasTie = true
    - minDiff = min(minDiff, |zi − oi|)
- If Z = 0 or O = 0:
  - answer = 0
- Else if hasTie or (has0Major and has1Major):
  - answer = T − S
- Else:
  - answer = T − S + minDiff
- Output answer

Complexity

- We count characters once: O(sum |Si|) ≤ 5e5, well within 2 seconds.
- Memory O(1) besides reading input.

Handling all cases

- All one dish globally: answer 0.
- Both dishes globally and both local majorities exist (or there is a tie): no extra cost beyond per-table majorities.
- Both dishes globally but every table strictly favors the same dish: flip the table with smallest margin; ties would have made the extra cost 0.

This method is correct because it precisely captures the maximum number of contestants that can remain seated at their original table under the uniformity requirement, with the only global coupling being the need to open at least one table per dish when both dishes exist.