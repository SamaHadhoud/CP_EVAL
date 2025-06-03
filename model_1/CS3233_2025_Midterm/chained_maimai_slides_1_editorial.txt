### Editorial: Chained Maimai Slides

#### Problem Analysis
The problem involves selecting a set of slides (edges) from a given collection to create a level for the rhythm game *maimai*. The level must meet specific conditions:
1. **Chain Slides as Cycles**: The selected slides must form one or more disjoint cycles. Each cycle is a sequence where the end of one slide is the start of the next, and the entire cycle ends at the starting button.
2. **Cover All Buttons**: Every button (1 to 8) must be the start of at least one slide in the selected set.
3. **Slide Usage Constraint**: Each slide type can be used at most once.
4. **Minimum Slide Count**: The total number of slides used must be at least \( K \).
5. **Minimize Speed Range**: The difference between the maximum and minimum speeds of the selected slides should be minimized.

If it's impossible to satisfy these conditions, the output should be \(-1\).

#### Key Insight
- **Graph Representation**: Model the problem as a directed multigraph where buttons are nodes and slides are edges (from \( X_i \) to \( Y_i \)) with speeds \( V_i \).
- **Eulerian Subgraph Requirement**: The selected edges must form an Eulerian subgraph, meaning:
  - For every node, the in-degree equals the out-degree.
  - Every node has an out-degree of at least 1 (ensuring all buttons are starts of slides).
- **Speed Range Minimization**: The goal is to find a subset of edges that satisfies the Eulerian condition, has at least \( K \) edges, and minimizes the difference between the highest and lowest speeds in the subset.

#### Algorithm Selection
Given the constraints (\( M \leq 1000 \), 8 nodes), we use a **sliding window (two-pointer) approach** over the sorted list of edges by speed. For each window of speeds, we check if there exists a valid Eulerian subgraph within the window that meets the conditions.

##### Steps:
1. **Sort Edges by Speed**: Sort all \( M \) edges in increasing order of their speeds. This allows efficient window management.
2. **Two-Pointer Technique**:
   - **Right Pointer (R)**: Expands the window to include edges with higher speeds.
   - **Left Pointer (L)**: Contracts the window by excluding edges with lower speeds as much as possible while maintaining a valid solution.
   - **Objective**: For each window \([L, R]\), check if the edges in this speed range contain a valid subgraph. The minimal range is tracked during the process.
3. **Checking Valid Subgraph in a Window**:
   - **Net Flow Representation**: For the subgraph to be Eulerian, the net flow (out-degree minus in-degree) for every node must be zero. Represent the state as a vector of net flows for 7 nodes (the 8th is derived as \(-\sum_{i=1}^7 \text{flow}_i\) due to conservation).
   - **Dynamic Programming (DP)**: Use a 7-dimensional DP array where:
     - Each dimension corresponds to the net flow of a node (nodes 1 to 7).
     - The value of each dimension ranges from \(-D\) to \(D\), where \(D\) is the maximum possible net flow (limited by the number of edges incident to the node in the window).
     - **State Transition**: For each edge \((u, v)\) in the window, update the DP state by either including or excluding the edge. Including the edge adjusts the net flow:
       - Increase the net flow of \(u\) by 1.
       - Decrease the net flow of \(v\) by 1.
     - **Initial State**: All zeros (no net flow).
     - **Final State**: All zeros (net flow zero for all nodes), and the number of edges used is at least \( K \).
   - **Out-Degree Check**: After achieving net flow zero, verify that every node has at least one outgoing edge in the selected subgraph. This is ensured by the DP constraints during state transitions.
4. **Complexity**:
   - **Sorting**: \( O(M \log M) \).
   - **Two-Pointers**: \( O(M) \) windows in the worst case.
   - **DP per Window**: The state space is \( O((2D + 1)^7) \), where \( D \) is the maximum net flow. Given \( M \leq 1000 \), \( D \) is at most the number of edges incident to a node, which is \( O(M) \). However, in practice, the net flow is bounded by the window size, and optimizations (like pruning unreachable states) make it feasible.

#### Handling Edge Cases
- **Impossible Cases**:
  - If \( K > M \), it's impossible (not enough slides).
  - If no window yields a valid Eulerian subgraph with at least \( K \) edges, output \(-1\).
- **Small \( K \)**: Even if \( K \) is small, the Eulerian condition requires at least 8 edges (since there are 8 nodes, each with out-degree \(\geq 1\), so \( K \leq 8 \) is feasible, but \( K > 8 \) might be possible if degrees exceed 1.
- **Multiple Edges with Same (X, Y, V)**: These are treated as distinct edges, so they can be selected independently if they fall within the speed window.

#### Solution Walkthrough with Sample
**Sample Input #1**:
```
12 9
4 4 7
4 8 6
8 5 5
5 1 5
1 6 7
6 2 6
2 7 10
7 3 3
3 4 7
5 5 100
6 3 1
3 4 7
```
- **Sorted Speeds**: After sorting, the edges by speed: [1, 3, 5, 5, 6, 6, 7, 7, 7, 10, 100]. The minimal range is 7 (from speed 3 to 10, excluding 100).
- **Valid Subgraph**: The edges in the speed range [3, 10] include enough slides to form cycles covering all buttons with net flow zero and at least 9 edges. The range is \(10 - 3 = 7\).

**Sample Input #2**:
```
4 1
1 3 5
4 1 1
1 8 4
1 3 5
```
- **Coverage Issue**: Button 2 has no outgoing edge, making it impossible to cover all buttons. Output \(-1\).

#### Correctness and Optimization
- **Correctness**: The sliding window ensures we consider all possible speed ranges. The DP checks the existence of a valid Eulerian subgraph by enforcing net flow conservation and out-degree constraints.
- **Optimization**:
  - **Net Flow Bounds**: The net flow for each node is bounded by the number of edges incident to it in the window, reducing the state space.
  - **Early Termination**: If expanding the window cannot yield a smaller range, terminate early.
  - **State Pruning**: Only reachable states (those that can lead to net flow zero) are processed.

#### Conclusion
The approach efficiently checks all possible speed ranges for a valid Eulerian subgraph using a sliding window and dynamic programming. The solution leverages the small number of nodes (8) to manage the state space, ensuring optimality while adhering to constraints. If no valid subgraph exists, \(-1\) is returned; otherwise, the minimal speed range is found. This method balances correctness with performance within the problem's limits.