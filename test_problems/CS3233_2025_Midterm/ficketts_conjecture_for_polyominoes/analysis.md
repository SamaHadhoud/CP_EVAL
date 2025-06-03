**Claim.** _A convex polyomino in a board with m columns and n rows has at most $\mathcal{O}(m + n)$ boundary cells._

**Proof.** Note that each boundary cell is 4-adjacent to a cell which is 4-adjacent to an empty (non-polyomino) cell. By definition of a convex polyomino, on each row there can be at most 2 cells that are horizontally adjacent to an empty cell, similar for column, so there are at most $\mathcal{O}(m + n)$ cells 4-adjacent to an empty cell. Therefore there are also at most $\mathcal{O}(m + n)$ boundary cells. $\square$

As such, we consider all rotations, for each rotation we consider all translations, and count number of overlapping cells using pre-computed list of coordinates of the boundary cells.

Brute force should not pass but maybe some vectorization magic or constant factor optimization can squeeze in the time limit.

On a side note, the conjecture (has been solved since then) is [Matematicheskie Trudy, 2001, Volume 4, Number 1, Pages 111–121	(Mi mt7): On One Extremal Problem on the Euclidean Plane](https://www.mathnet.ru/php/archive.phtml?show=paper&jrnid=mt&paperid=7&option_lang=eng). Though I’m not too interested in checking the correctness of the proof.
