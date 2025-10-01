Restatement

You have a string S of length N over the alphabet {A, B}. Two types of operations are supported:

- Type 1: toggle all characters in a subarray L..R (A ↔ B).
- Type 2: start with numbers (A, B), then scan S[L..R] from left to right; for each i:
  - if S[i] = A: do A = A + B
  - if S[i] = B: do B = A + B
  Return the final (A, B) modulo 1,000,000,007.

You must process Q up to 100,000 operations online.


Key idea: each character is a 2×2 linear transform

Let v = [A; B] be a column vector.

- Processing an 'A' step transforms (A, B) to (A+B, B). This is
  [A'; B'] = MA · [A; B] with MA = [[1, 1], [0, 1]].
- Processing a 'B' step transforms (A, B) to (A, A+B). This is
  [A'; B'] = MB · [A; B] with MB = [[1, 0], [1, 1]].

Scanning a substring S[L..R] applies these matrices in sequence. If we use column vectors and multiply on the left, then after the whole substring:
v_out = M[L..R] · v_in where M[L..R] = M_{S[R]} · M_{S[R-1]} · … · M_{S[L]}.

Two immediate consequences:
- Range query “2 L R A B” is a single matrix-vector multiplication once we know M[L..R].
- Range toggle “1 L R” flips each MA ↔ MB inside [L..R]. There is a very useful algebraic shortcut for toggling the entire segment matrix:
  Let P = [[0, 1], [1, 0]] (the swap matrix). Then MB = P · MA · P and MA = P · MB · P.
  Therefore, toggling every character in a segment with matrix M replaces it by
  M' = Π (P · M_char · P) = P · (Π M_char) · P = P · M · P.
  Concretely, for any 2×2 matrix M = [[a, b], [c, d]], P · M · P = [[d, c], [b, a]], i.e., swap across the anti-diagonal.

This gives a clean lazy propagation operation: to “toggle” a node’s matrix, just transform [[a, b], [c, d]] into [[d, c], [b, a]] and mark a lazy flip flag (since toggling twice restores the original, a single boolean lazy flag with XOR is enough).


Data structure: segment tree on matrices with lazy range toggle

We need:
- Range product M[L..R] under non-commutative multiplication.
- Range toggle that conjugates by P: M := P M P.

Segment tree design:
- Leaf i stores the 2×2 matrix for S[i]:
  - if S[i] = 'A': MA = [[1, 1], [0, 1]]
  - if S[i] = 'B': MB = [[1, 0], [1, 1]]
- Internal node for interval [l, r] stores the matrix M[l..r] = M[r] · M[r-1] · … · M[l].
  When merging children [l, m] and [m+1, r], we must combine as:
  M[l..r] = M[m+1..r] · M[l..m].
  This “right times left” order preserves the correct sequence because later indices multiply on the left.
- Each node also has a boolean lazy flag flip. When applying a toggle to a fully covered node:
  - Replace M = [[a, b], [c, d]] by [[d, c], [b, a]].
  - flip ^= 1.
  During propagation (push), if flip is set on a parent, apply the same transformation to each child and XOR their flip flags, then clear the parent’s flip.

Matrix operations (mod MOD = 1,000,000,007):
- Multiplication: if X = [[a, b], [c, d]] and Y = [[e, f], [g, h]], then
  X · Y = [[a·e + b·g, a·f + b·h], [c·e + d·g, c·f + d·h]] modulo MOD.
- Toggle transform: toggle([[a, b], [c, d]]) = [[d, c], [b, a]].
- Identity matrix I = [[1, 0], [0, 1]].

Range query
- Query [L, R] recursively. If a node is completely inside, return its matrix; if outside, return I.
- When combining partial results from left child (Lpart) and right child (Rpart), return Rpart · Lpart.
- Finally, for input (A, B), compute:
  A' = (M00·A + M01·B) mod MOD
  B' = (M10·A + M11·B) mod MOD

Range update (toggle)
- Standard lazy segment tree range update on [L, R].
- On full cover: apply toggle to the node and flip ^= 1.
- Otherwise push and recurse to children; then recompute node as right.mat · left.mat.

Why this works

- Correctness of the matrix model:
  Each step is a linear map on (A, B), so the whole scan is the product of those maps. Associativity of matrix multiplication guarantees that merging segments as right · left yields the exact transformation M[R..L].
- Correctness of toggling via conjugation:
  Toggling replaces each MA by MB and vice versa. Because MB = P MA P and P^2 = I, toggling a whole segment M = Π M_char gives M' = Π (P M_char P) = P M P. Implemented as the anti-diagonal swap [[a, b], [c, d]] → [[d, c], [b, a]].
- Lazy propagation:
  Toggling is an involution, so a single boolean lazy flag is sufficient (XOR accumulation). Pushing the flag preserves the invariant that each node’s matrix equals the product over its current interval.
- Modulo arithmetic:
  Since the output is required modulo MOD, it is safe and efficient to perform every addition/multiplication modulo MOD.

Pseudocode sketch

- Constants:
  - MOD = 1_000_000_007
  - MA = [[1, 1], [0, 1]]
  - MB = [[1, 0], [1, 1]]
  - I = [[1, 0], [0, 1]]

- Matrix ops:
  - mul(X, Y):
    z00 = (X00*Y00 + X01*Y10) % MOD
    z01 = (X00*Y01 + X01*Y11) % MOD
    z10 = (X10*Y00 + X11*Y10) % MOD
    z11 = (X10*Y01 + X11*Y11) % MOD
    return [[z00, z01], [z10, z11]]
  - toggle(M): return [[M11, M10], [M01, M00]]

- Build(node, l, r):
  if l == r:
    tree[node].mat = (S[l] == 'A') ? MA : MB
    tree[node].flip = 0
  else:
    mid = (l + r) // 2
    Build(left, l, mid)
    Build(right, mid+1, r)
    tree[node].mat = mul(tree[right].mat, tree[left].mat)
    tree[node].flip = 0

- Push(node):
  if tree[node].flip:
    for child in {left, right}:
      tree[child].mat = toggle(tree[child].mat)
      tree[child].flip ^= 1
    tree[node].flip = 0

- UpdateToggle(node, l, r, ql, qr):
  if qr < l or r < ql: return
  if ql <= l and r <= qr:
    tree[node].mat = toggle(tree[node].mat)
    tree[node].flip ^= 1
    return
  Push(node)
  mid = (l + r) // 2
  UpdateToggle(left, l, mid, ql, qr)
  UpdateToggle(right, mid+1, r, ql, qr)
  tree[node].mat = mul(tree[right].mat, tree[left].mat)

- Query(node, l, r, ql, qr) -> Mat:
  if qr < l or r < ql: return I
  if ql <= l and r <= qr: return tree[node].mat
  Push(node)
  mid = (l + r) // 2
  Lmat = Query(left, l, mid, ql, qr)
  Rmat = Query(right, mid+1, r, ql, qr)
  return mul(Rmat, Lmat)

- Handling a type-2 query:
  M = Query(1, 1, N, L, R)
  A' = (M00*A + M01*B) % MOD
  B' = (M10*A + M11*B) % MOD
  print A', B'

Complexity

- Each update and query touches O(log N) nodes. Each node operation is O(1).
- Total time: O((N + Q) log N), which is easily within 3 seconds for N, Q ≤ 100,000.
- Memory: O(N) nodes; each node stores 4 integers and 1 boolean, well within 256 MB.

Edge cases and pitfalls

- Be careful with the merge order: always combine as right · left.
- Perform modulo after every addition/multiplication.
- Use a 64-bit type internally for products before reducing modulo.
- The identity matrix must act as neutral for the chosen merge order; returning I for disjoint segments is correct with combine = right · left.
- The lazy flag is XORed on propagation; flipping twice cancels out.

Why this handles all cases

- Any substring request reduces to a matrix product by the linearity of the step operations; the segment tree maintains exactly those products for all segments.
- Any sequence of toggles is handled correctly due to conjugation by P; toggling over different and overlapping ranges is supported via lazy propagation.
- Since addition orders do not commute, preserving multiplication order via right · left combination is essential; the design enforces it.
- The final matrix-vector multiplication yields the exact A and B after processing S[L..R], and modulus is applied as required.

Sanity check with the sample

- S = ABAAA. Query 2 1 5 1 1:
  M = M5 · M4 · M3 · M2 · M1, v_out = M·[1;1] = [11;3].
- Toggle 1 3 5 makes S = ABBBB.
- Query 2 2 5 0 1e9:
  Repeated MB steps preserve A = 0 and B = 1e9, so output is (0, 1e9).
Both match the sample output.