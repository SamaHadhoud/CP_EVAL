```cpp
#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> adj(N + 1);
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Root the tree at 1, build parent and order (post-order via reverse)
    vector<int> parent(N + 1, -1);
    vector<int> order;
    order.reserve(N);
    vector<int> st;
    st.reserve(N);
    parent[1] = 0;
    st.push_back(1);
    while (!st.empty()) {
        int v = st.back();
        st.pop_back();
        order.push_back(v);
        for (int u : adj[v]) {
            if (u != parent[v] && parent[u] == -1) {
                parent[u] = v;
                st.push_back(u);
            }
        }
    }
    
    vector<array<long long, 4>> dp(N + 1, {0, 0, 0, 0});
    for (int i = N - 1; i >= 0; --i) {
        int v = order[i];

        // Combine over children with two regimes:
        // X: when v is not saturated for unused child edges
        // Y: when v is saturated for unused child edges
        long long W0X = 1, W1X = 0, W2X = 0;
        long long W0Y = 1, W1Y = 0, W2Y = 0;

        for (int u : adj[v]) if (u != parent[v]) {
            long long X = dp[u][1] % MOD;                              // child saturated, parent edge not chosen
            long long Y = (dp[u][0] + dp[u][1]) % MOD;                 // child any, parent edge not chosen
            long long Z = (dp[u][2] + dp[u][3]) % MOD;                 // parent edge chosen

            // Update for N = X (v not saturated on unused edge)
            long long nW2X = ( (W2X * X) % MOD + (W1X * Z) % MOD ) % MOD;
            long long nW1X = ( (W1X * X) % MOD + (W0X * Z) % MOD ) % MOD;
            long long nW0X = (W0X * X) % MOD;
            W2X = nW2X; W1X = nW1X; W0X = nW0X;

            // Update for N = Y (v saturated on unused edge)
            long long nW2Y = ( (W2Y * Y) % MOD + (W1Y * Z) % MOD ) % MOD;
            long long nW1Y = ( (W1Y * Y) % MOD + (W0Y * Z) % MOD ) % MOD;
            long long nW0Y = (W0Y * Y) % MOD;
            W2Y = nW2Y; W1Y = nW1Y; W0Y = nW0Y;
        }

        // States:
        // 2: parent chosen, deg(v)=1 -> k=0, N=X
        dp[v][2] = W0X % MOD;
        // 3: parent chosen, deg(v)=2 -> k=1, N=Y
        dp[v][3] = W1Y % MOD;
        // 0: parent not chosen, deg(v)<=1 -> k in {0,1}, N=X
        dp[v][0] = (W0X + W1X) % MOD;
        // 1: parent not chosen, deg(v)=2 -> k=2, N=Y
        dp[v][1] = W2Y % MOD;
    }

    long long ans = (dp[1][0] + dp[1][1]) % MOD;
    cout << ans << '\n';
    return 0;
}
```