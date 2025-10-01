```cpp
#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> g;

// LCA preprocessing
int LOGN;
vector<int> tin, tout, depth, parent;
vector<vector<int>> up;
int timer = 0;

// Virtual tree storage (only for used nodes per query)
vector<vector<pair<int,int>>> vt; // vt[u] = list of {v, length}
vector<int> weight;               // multiplicity per node

// Iterative DFS to compute tin/tout, depth, up table
void build_lca(int root = 1) {
    tin.assign(N + 1, 0);
    tout.assign(N + 1, 0);
    depth.assign(N + 1, 0);
    parent.assign(N + 1, 0);
    up.assign(LOGN, vector<int>(N + 1, 0));

    vector<int> it(N + 1, 0);
    vector<int> st;
    st.reserve(N);

    parent[root] = root;
    depth[root] = 0;
    st.push_back(root);

    while (!st.empty()) {
        int u = st.back();
        if (it[u] == 0) {
            tin[u] = ++timer;
            up[0][u] = (u == root) ? root : parent[u];
            for (int j = 1; j < LOGN; ++j) {
                up[j][u] = up[j - 1][ up[j - 1][u] ];
            }
        }
        if (it[u] < (int)g[u].size()) {
            int v = g[u][it[u]++];
            if (v == parent[u]) continue;
            parent[v] = u;
            depth[v] = depth[u] + 1;
            st.push_back(v);
        } else {
            tout[u] = ++timer;
            st.pop_back();
        }
    }
}

inline bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[v] <= tout[u];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int j = LOGN - 1; j >= 0; --j) {
        int uu = up[j][u];
        if (!is_ancestor(uu, v)) u = uu;
    }
    return up[0][u];
}

inline int dist(int u, int v) {
    int w = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[w];
}

long long ans;
int Ktot;

int dfs_vt(int u) {
    int sz = weight[u];
    for (auto &e : vt[u]) {
        int v = e.first;
        int L = e.second;
        int child_sz = dfs_vt(v);
        ans += 1LL * L * min(child_sz, Ktot - child_sz);
        sz += child_sz;
    }
    return sz;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    g.assign(N + 1, {});
    for (int i = 0; i < N - 1; ++i) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    LOGN = 1;
    while ((1 << LOGN) <= N) ++LOGN;

    build_lca(1);

    vt.assign(N + 1, {});
    weight.assign(N + 1, 0);

    int Q;
    cin >> Q;
    while (Q--) {
        int K;
        cin >> K;
        vector<int> A(K);
        for (int i = 0; i < K; ++i) cin >> A[i];

        // Count multiplicities and collect distinct terminals
        vector<int> terminals;
        terminals.reserve(K);
        Ktot = 0;
        for (int x : A) {
            if (weight[x] == 0) terminals.push_back(x);
            weight[x]++;
            Ktot++;
        }

        // Build set U = terminals + LCAs of consecutive by tin
        vector<int> nodes = terminals;
        sort(nodes.begin(), nodes.end(), [&](int u, int v){ return tin[u] < tin[v]; });
        vector<int> U = nodes;
        for (int i = 0; i + 1 < (int)nodes.size(); ++i) {
            U.push_back(lca(nodes[i], nodes[i + 1]));
        }
        sort(U.begin(), U.end(), [&](int u, int v){ return tin[u] < tin[v]; });
        U.erase(unique(U.begin(), U.end()), U.end());

        // Clear virtual tree adjacency for all nodes in U
        for (int u : U) vt[u].clear();

        // Build virtual tree using stack
        vector<int> st;
        st.reserve(U.size());
        st.push_back(U[0]);
        for (int i = 1; i < (int)U.size(); ++i) {
            int u = U[i];
            while (!st.empty() && !is_ancestor(st.back(), u)) st.pop_back();
            int p = st.back();
            vt[p].push_back({u, dist(p, u)});
            st.push_back(u);
        }

        // Compute answer by one DFS on virtual tree
        ans = 0;
        dfs_vt(U[0]);
        cout << ans << '\n';

        // Reset weights for terminals
        for (int u : terminals) weight[u] = 0;
    }

    return 0;
}
```