### Approach
The problem involves simulating the migration of Piplups to icebergs over a range of days, with updates to the starting icebergs. For each query, we need to determine the iceberg where the X-th Piplup in a group would settle, considering only the migrations within a specified day range. The key insight is that the migrations form a set of occupied icebergs, and the X-th smallest unoccupied iceberg is the answer.

#### Key Insights:
1. **Simulating Migrations**: For each day in the query range, a Piplup starts at a specified iceberg. If that iceberg is occupied, it moves to the next unoccupied iceberg. This process is efficiently simulated using a Union-Find (Disjoint Set Union, DSU) data structure with path compression to quickly find the next available iceberg.
2. **Bounded Simulation**: Given the constraints (starting icebergs ≤ 500 and X ≤ 500), the simulation can be bounded to a range of `len + 1000` (where `len` is the number of days in the query). This ensures we only process relevant icebergs.
3. **Lazy Initialization**: To reset the DSU structure efficiently between queries, we use a timestamp-based lazy initialization. This avoids reinitializing the entire structure for each query, significantly improving performance.
4. **Free Iceberg Calculation**: After processing the migrations, the X-th smallest unoccupied iceberg is found either within the bounded range or beyond it, calculated as `U + (X - free_count)`, where `U` is the upper bound and `free_count` is the number of unoccupied icebergs within the bound.

### Solution Code
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX_U = 1001000; // 1000000 + 1000

int next_ptr[MAX_U + 2];
int timestamp[MAX_U + 2];
int now = 0;

int find(int x, int U) {
    if (x > U) {
        return x;
    }
    if (timestamp[x] != now) {
        timestamp[x] = now;
        next_ptr[x] = x;
    }
    if (next_ptr[x] != x) {
        next_ptr[x] = find(next_ptr[x], U);
    }
    return next_ptr[x];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    for (int i = 0; i < M; i++) {
        int type;
        cin >> type;
        if (type == 0) {
            int P, X;
            cin >> P >> X;
            A[P-1] = X;
        } else {
            int L, R, X_i;
            cin >> L >> R >> X_i;
            int len = R - L + 1;
            int U_bound = len + 1000;
            now++;
            int occ = 0;
            for (int j = L-1; j <= R-1; j++) {
                int x = A[j];
                int y = find(x, U_bound);
                if (y <= U_bound) {
                    next_ptr[y] = find(y+1, U_bound);
                    occ++;
                }
            }
            int free_count = U_bound - occ;
            if (X_i <= free_count) {
                int current = 0;
                for (int j = 0; j < X_i; j++) {
                    current = find(current+1, U_bound);
                }
                cout << current << '\n';
            } else {
                cout << U_bound + (X_i - free_count) << '\n';
            }
        }
    }
    return 0;
}
```

### Explanation
1. **Initialization**: The global arrays `next_ptr` and `timestamp` are initialized to manage the DSU structure. The `now` variable tracks the current query for lazy initialization.
2. **Query Processing**:
   - **Update Queries (Type 0)**: Modify the starting iceberg for a specific day.
   - **Migration Queries (Type 1)**:
     - **Bounded Simulation**: For each day in the range, the DSU structure is used to find the next available iceberg starting from the specified value. The bound `U_bound = len + 1000` ensures efficiency.
     - **Occupancy Tracking**: The count of occupied icebergs within the bound is maintained.
     - **Free Iceberg Calculation**: If the X-th free iceberg is within the bound, it is found using the DSU. Otherwise, it is calculated as the first free iceberg beyond the bound.
3. **Efficiency**: The DSU with path compression and lazy initialization ensures that each query is processed in near-linear time relative to the query length, making the solution feasible for large inputs. The bounded range and efficient reset mechanism further optimize performance.