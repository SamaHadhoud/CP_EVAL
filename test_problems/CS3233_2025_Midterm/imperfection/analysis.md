If we want to force an element value to be the maximum of the final array, then we must delete all elements with a larger value. To leverage from this fact, we can solve the problem by trying all possible maximums of the final array, starting from the largest value to the smallest. We can store the total cost of deleting all elements greater than the current element in consideration easily.

The problem is now simplified: Find the minimum cost to delete elements such that the current maximum element is the majority element. Let the number of maximum elements be $X$, then, by a greedy argument, we delete elements smaller than maximum that has the smallest of costs until the number of such elements is at most $X - 1$. Furthermore, by another greedy argument, it is always less optimal to delete a maximum element, as we need to delete more elements while the costs of deletion is guaranteed to be positive.

Now, we need an algorithm that can maintain the following:

- Get the sum of costs for the $K$ cheapest elements. This is used to find the best cost.
- Remove an element from consideration. This is used to remove the maximum element.

Removing elements sound like what BST data structures do, but it is quite tough for these kinds of data structures to retrieve sums of values.

Instead, if we think of removing an element as setting the element’s count and cost as 0, by sorting the elements in increasing cost, we can see that the first K cheapest elements can be retrieved by a sum of some prefix of the array.

To formalize, we maintain two data structures that stores the costs and count of the elements. Initially, $cost_i = B_i$ (after sort) and $count_i = 1$ for all $1 \le i \le N$. then the two operations above can be represented as follows:

- Get the value of $cost_0 + cost_1 + \cdots + cost_j$ where $j$ is the smallest index such that $count_0 + count_1 + \cdots + count_j = K$ for a given $K$.
- Set $cost_k = count_k = 0$ for a given $k$.

The point set and (prefix) range sum can be solved by Binary Indexed Tree (or Segment Tree). We maintain two of these data structures, one of them stores cost and the other stores count. We can use binary search to find the smallest index that sums to the count we need. With this algorithm, the time complexity is $\mathcal{O}(N \log^2 N)$ which passes.

One may note that the binary search can actually be integrated in the BIT/ST. Using the properties of these data structures, we can remove the $\mathcal{O}(\log N)$ overhead from binary search, leading to a time complexity of $\mathcal{O}(N \log N)$. Details are omitted for self exploration.
