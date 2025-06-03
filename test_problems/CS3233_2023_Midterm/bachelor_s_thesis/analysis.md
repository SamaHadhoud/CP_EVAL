If $k = n$, the problem is trivial – only $2$ permutations are possible, the original sequence or its reverse. We can compute the inversion of each array and take the minimum. Otherwise, if $k < n$, the following construction will sort the array $a$ (thus making the number of inversions $0$):

- **If $k + 1 < n$**, we can move the largest element to the last position by repeatedly moving the elements after the largest element to the beginning of the array. Afterward, we recurse to the case $(n - 1, k)$.

- **If $k + 1 = n$**, note that if immediately after applying one operation, we apply another operation on the first $k$ elements, the net result of the two operations is that there’s a single element which is moved to the back, while the relative ordering of all the other elements remain unchanged. Therefore, it suffices to move all elements to the back sequentially, in increasing order.

**Challenge:** can you solve the problem in a “small” number of operations?