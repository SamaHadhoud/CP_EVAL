Let the happiness levels be $H_1 \ge H_2 \ge \cdots \ge H_n$. We shall describe a subset of happiness levels by their indices, in ascending order. So if we want to take objects with happiness levels $H_1, H_2, H_4$, we’ll just refer to it as $(1, 2, 4)$.

Now clearly, the greatest happiness level is attained by $(1, 2, \ldots, k)$, and the next greatest is $(1, 2, \ldots, k - 1, k + 1)$, and the next greatest is either $(1, 2, \ldots, k - 2, k, k + 1)$ or $(1, 2, \ldots, k - 1, k + 2)$.

Let’s refer to each $k$-tuple as a state, and say the happiness level of a state is the sum of the happiness levels of the indices in the state. We will try to visit states in decreasing order of happiness level. Intuitively, to find the next largest state, we should be doing something like, take one of the already visited states, then increment one of its indices by one, such that we still have $k$ distinct indices in the new state.

Treat each state as a node on a graph. From each state $s$ except the smallest state $(n - k + 1, \ldots, n)$, we will have at most two outgoing edges to other states, and these states must have happiness level not greater than the happiness level of $s$. We also want to construct this graph such that starting from the state $(1, 2, \ldots, k)$, there exists a **unique** path to any other state.

Let’s first suppose we are able to construct this graph. We will describe how to solve the problem from here. We will have a priority queue to keep track of states which are candidates for the next largest happiness level. The state with the largest happiness level will be at the top.

We start by having $(1, 2, \ldots, k)$ in the priority queue. Now, we pop the priority queue, and add the states the (popped) top of the priority queue points to in the graph.

Since we only add happiness levels greater than the state we popped, and from the initial state there is a unique path to every other state, the first $k$ states we pop will be the $k$ largest happiness levels, without duplicates of any state.

This runs in $O(l \log l)$ time.

Now we describe how to construct this graph.

View each state as a collection of contiguous segments of integer. So in the initial state we have a single segment $[1, k]$. The idea is that we want to move this segment to the right and split it at some points to get a collection of contiguous segments.

Now we consider the concept of an active segment of **indices**. The active segment $[l, r]$ will refer to the $l$-th to the $r$-th indices in the state, 1-indexed. We will refer to the $k$ indices in the state as $i_1 < i_2 < \cdots < i_k$.

We start from the active segment $[1, k]$, which refer to the indices $1, 2, \ldots, k$ at first. We describe a **move** operation on an active segment $[l, r]$: If $i_l, \ldots, i_r$ form a contiguous range of indices, then increase $i_r$ by 1. Otherwise, pick $j$ such that $i_{j+1} > i_j + 1$ and increase $i_j$ by 1.

As an example, if the active segment $[1,3]$ contains the indices $(2, 3, 4)$, then a series of move operations will change it as follows:  
$(2, 3, 4) \rightarrow (2, 3, 5) \rightarrow (2, 4, 5) \rightarrow (3, 4, 5) \rightarrow (3, 4, 6)$.

Note that with this move operation, the difference between the largest and smallest indices in an active segment $[l, r]$ will always be $r - l + 1$ (there’s a gap) or $r - l$ (all contiguous).

Now we also describe a **split** operation: if the active segment currently has the indices forming the ranges $[x, l]$ and $[x + 2, r]$ for some $l, r, x$, then change the active segment to the indices forming $[x + 2, r]$, and perform the move operation on this state.

So suppose the active operation is currently $(2, 3, 4)$ and we do 2 move operations to get $(2, 3, 4) \rightarrow (2, 3, 5) \rightarrow (2, 4, 5)$, then we do a split operation, we will get $(2, 4, 5) \rightarrow (2, 4, 6)$. This is because the active segment currently forms the ranges of indices $[2, 2]$ and $[4, 5]$, so the split operation changes the active segment to the indices forming $[4, 5]$, then we move it to get $(4, 5) \rightarrow (4, 6)$. If we do a split operation again now we’ll get $(2, 4, 6) \rightarrow (2, 4, 7)$, but the active segment will be just the index 7.

Now to construct the graph, simply start from the initial state $(1, 2, \ldots, k)$ with active segment $[1, k]$, and from each state, we draw outgoing edges to the states we get from a move and split operation respectively (if they exist).

Why this constructs a graph where there is a unique path from the initial state to every other state is left as an exercise to the reader (just consider where the splits happen).
