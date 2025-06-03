### Problem Analysis
The problem involves managing a set of course dependencies in a directed acyclic graph (DAG) where nodes represent courses and edges represent dependencies (an edge from \(u\) to \(v\) means course \(u\) must be completed before \(v\)). The goal is to process queries that add new dependencies while ensuring that the entire set of courses can be scheduled within \(K\) semesters. The scheduling constraint requires that the longest chain of dependencies (a path of consecutive dependencies) does not exceed \(K\) courses, as each course in the chain must be in a distinct semester.

### Key Insight
The core insight is that the acceptability of the dependency set is determined by the length of the longest chain (in terms of the number of nodes). A chain of length \(L\) requires at least \(L\) semesters. Thus, the dependency set is acceptable only if the longest chain in the DAG has length at most \(K\). 

For each query to add a new edge \((C, D)\), the critical check is whether the new chain formed by concatenating the longest chain ending at \(C\) and the longest chain starting at \(D\) (via the new edge) exceeds \(K\) nodes. Specifically, if the sum of the lengths of these two chains is at least \(K+1\), adding the edge creates a chain that is too long, making the set unacceptable.

### Algorithm Selection
1. **Precomputation**:
   - **Initial Graph Setup**: Store the initial dependencies in adjacency lists for outgoing and incoming edges.
   - **Dynamic Programming Arrays**:
     - `dp_in[u]` represents the length of the longest chain ending at node \(u\).
     - `dp_out[u]` represents the length of the longest chain starting at node \(u\).
   - Compute `dp_in` by processing nodes in increasing order (since edges go from lower to higher indices, this is a topological order). For each node \(u\), `dp_in[u]` is the maximum of 1 and `dp_in[v] + 1` for all incoming edges \((v, u)\).
   - Compute `dp_out` by processing nodes in decreasing order. For each node \(u\), `dp_out[u]` is the maximum of 1 and `dp_out[w] + 1` for all outgoing edges \((u, w)\).

2. **Query Processing**:
   - For each query \((C, D)\):
     - If the edge \((C, D)\) already exists, output "accept" (no change).
     - Otherwise, check if `dp_in[C] + dp_out[D] >= K+1`. If true, output "reject" (adding the edge would create a chain longer than \(K\)).
     - If false, output "accept" and add the edge to the graph. Then update:
       - **Update `dp_in` for \(D\)**: Set `dp_in[D] = max(dp_in[D], dp_in[C] + 1)` if this new value is \(\leq K\). Propagate this update to all descendants of \(D\) using BFS: for each descendant \(w\), if `dp_in[u] + 1` (where \(u\) is the current node) is greater than `dp_in[w]` and \(\leq K\), update `dp_in[w]` and continue propagating.
       - **Update `dp_out` for \(C\)**: Set `dp_out[C] = max(dp_out[C], dp_out[D] + 1)` if this new value is \(\leq K\). Propagate this update to all ancestors of \(C\) using BFS: for each ancestor \(w\), if `dp_out[u] + 1` is greater than `dp_out[w]` and \(\leq K\), update `dp_out[w]` and continue propagating.

### Complexity Analysis
- **Precomputation**:
  - Building adjacency lists: \(O(N + M)\).
  - Computing `dp_in` and `dp_out`: \(O(N + M)\) each.
- **Query Processing**:
  - Each query check: \(O(1)\).
  - Edge addition and propagation: Each node can be updated at most \(K\) times (since chain lengths are bounded by \(K\)). Each update processes the node's outgoing or incoming edges. The total work over all queries is \(O(K \cdot (M + Q))\), where \(M + Q\) is the total number of edges after all queries.
- **Overall Complexity**: \(O(N + M + Q + K \cdot (M + Q))\). Given \(K \leq 100\) and \(N, M, Q \leq 2 \times 10^5\), this is efficient.

### Solution Code
```python
import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it)); m = int(next(it)); K_val = int(next(it))
    
    edges_set = set()
    adj_out = [[] for _ in range(n+1)]
    adj_in = [[] for _ in range(n+1)]
    
    for _ in range(m):
        u = int(next(it)); v = int(next(it))
        edges_set.add((u, v))
        adj_out[u].append(v)
        adj_in[v].append(u)
    
    dp_in = [1] * (n+1)
    dp_out = [1] * (n+1)
    
    for i in range(1, n+1):
        for j in adj_in[i]:
            if dp_in[j] + 1 > dp_in[i]:
                dp_in[i] = dp_in[j] + 1
                
    for i in range(n, 0, -1):
        for j in adj_out[i]:
            if dp_out[j] + 1 > dp_out[i]:
                dp_out[i] = dp_out[j] + 1
                
    q = int(next(it))
    output_lines = []
    
    for _ in range(q):
        c = int(next(it)); d = int(next(it))
        if (c, d) in edges_set:
            output_lines.append("accept")
        else:
            if dp_in[c] + dp_out[d] >= K_val+1:
                output_lines.append("reject")
            else:
                output_lines.append("accept")
                edges_set.add((c, d))
                adj_out[c].append(d)
                adj_in[d].append(c)
                
                new_in = dp_in[c] + 1
                if new_in > dp_in[d] and new_in <= K_val:
                    dp_in[d] = new_in
                    queue_in = deque([d])
                    while queue_in:
                        u = queue_in.popleft()
                        for w in adj_out[u]:
                            candidate = dp_in[u] + 1
                            if candidate > dp_in[w] and candidate <= K_val:
                                dp_in[w] = candidate
                                queue_in.append(w)
                                
                new_out = dp_out[d] + 1
                if new_out > dp_out[c] and new_out <= K_val:
                    dp_out[c] = new_out
                    queue_out = deque([c])
                    while queue_out:
                        u = queue_out.popleft()
                        for w in adj_in[u]:
                            candidate = dp_out[u] + 1
                            if candidate > dp_out[w] and candidate <= K_val:
                                dp_out[w] = candidate
                                queue_out.append(w)
                                
    print("\n".join(output_lines))

if __name__ == "__main__":
    main()
```

### Explanation
- **Initial Setup**: The initial graph is built from input, storing edges in adjacency lists and a set for quick lookup.
- **Precomputation**: The `dp_in` array is computed by processing nodes from 1 to \(N\), updating each node's value based on incoming edges. The `dp_out` array is computed by processing nodes from \(N\) down to 1, updating each node's value based on outgoing edges.
- **Query Handling**: For each query:
  - If the edge exists, "accept" is outputted immediately.
  - Otherwise, the sum `dp_in[C] + dp_out[D]` is checked. If it exceeds \(K\), the edge is rejected.
  - If accepted, the edge is added to the graph, and BFS propagates updates to `dp_in` for \(D\)'s descendants and `dp_out` for \(C\)'s ancestors, ensuring all affected nodes reflect the new longest chains within the \(K\) constraint.
- **Efficiency**: The BFS propagation is bounded by \(K\), ensuring each node is updated at most \(K\) times, making the solution efficient for large inputs. The overall approach efficiently maintains the DAG properties and checks the scheduling constraint dynamically.