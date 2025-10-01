```cpp
#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge {
        int to, cap, rev;
    };
    int N;
    vector<vector<Edge>> G;
    vector<int> level, it;

    Dinic(int n = 0) { init(n); }
    void init(int n) {
        N = n;
        G.assign(N, {});
        level.resize(N);
        it.resize(N);
    }
    void addEdge(int u, int v, int c) {
        Edge a{v, c, (int)G[v].size()};
        Edge b{u, 0, (int)G[u].size()};
        G[u].push_back(a);
        G[v].push_back(b);
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto &e : G[u]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }
    int dfs(int u, int t, int f) {
        if (u == t) return f;
        for (int &i = it[u]; i < (int)G[u].size(); ++i) {
            Edge &e = G[u][i];
            if (e.cap > 0 && level[u] < level[e.to]) {
                int ret = dfs(e.to, t, min(f, e.cap));
                if (ret > 0) {
                    e.cap -= ret;
                    G[e.to][e.rev].cap += ret;
                    return ret;
                }
            }
        }
        return 0;
    }
    int maxflow(int s, int t) {
        int flow = 0;
        const int INF = INT_MAX;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            int f;
            while ((f = dfs(s, t, INF)) > 0) flow += f;
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<int> A(N);
    vector<vector<long long>> B(N);
    long long totalMi = 0;
    for (int i = 0; i < N; ++i) {
        int Mi;
        cin >> A[i] >> Mi;
        --A[i]; // to 0-based
        B[i].resize(Mi);
        for (int j = 0; j < Mi; ++j) cin >> B[i][j];
        totalMi += Mi;
    }
    vector<long long> C(K);
    for (int i = 0; i < K; ++i) cin >> C[i];

    // Early necessary condition: need at least N-1 edges to connect N nodes
    if (K < N - 1) {
        cout << -1 << '\n';
        return 0;
    }

    // Identify the unique cycle (nodes with indeg > 0 after peeling)
    vector<int> indeg(N, 0);
    for (int i = 0; i < N; ++i) indeg[A[i]]++;
    queue<int> q;
    for (int i = 0; i < N; ++i) if (indeg[i] == 0) q.push(i);
    vector<char> is_cycle(N, 1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        is_cycle[u] = 0;
        int v = A[u];
        if (--indeg[v] == 0) q.push(v);
    }
    int L = 0;
    for (int i = 0; i < N; ++i) if (is_cycle[i]) L++;

    // Group workers by material
    unordered_map<long long, int> mat_id;
    mat_id.reserve(K * 2 + 7);
    vector<vector<int>> workers_by_mat; // worker indices per material
    for (int i = 0; i < K; ++i) {
        long long m = C[i];
        auto it = mat_id.find(m);
        if (it == mat_id.end()) {
            int id = (int)workers_by_mat.size();
            mat_id.emplace(m, id);
            workers_by_mat.push_back({});
            workers_by_mat[id].push_back(i);
        } else {
            workers_by_mat[it->second].push_back(i);
        }
    }
    int U = (int)workers_by_mat.size();

    // Filter B[i] to materials present among workers (map to material node indices)
    vector<vector<int>> prop_to_mat(N);
    for (int i = 0; i < N; ++i) {
        for (long long m : B[i]) {
            auto it = mat_id.find(m);
            if (it != mat_id.end()) prop_to_mat[i].push_back(it->second);
        }
    }

    // If any non-cycle proposal has no usable materials -> impossible
    for (int i = 0; i < N; ++i) {
        if (!is_cycle[i] && prop_to_mat[i].empty()) {
            cout << -1 << '\n';
            return 0;
        }
    }

    // Build flow network
    int S = 0;
    int p_base = 1;
    int m_base = p_base + N;
    int T = m_base + U;
    Dinic din(T + 1);

    // S -> proposals (only non-cycle in phase 1)
    for (int i = 0; i < N; ++i) {
        if (!is_cycle[i]) {
            din.addEdge(S, p_base + i, 1);
        }
    }
    // proposals -> materials
    for (int i = 0; i < N; ++i) {
        int u = p_base + i;
        for (int mid : prop_to_mat[i]) {
            din.addEdge(u, m_base + mid, 1);
        }
    }
    // materials -> T
    for (int mid = 0; mid < U; ++mid) {
        int cap = (int)workers_by_mat[mid].size();
        if (cap > 0) din.addEdge(m_base + mid, T, cap);
    }

    int mandatory = N - L;
    int f1 = din.maxflow(S, T);
    if (f1 < mandatory) {
        cout << -1 << '\n';
        return 0;
    }

    // Phase 2: enable cycle proposals
    for (int i = 0; i < N; ++i) {
        if (is_cycle[i]) {
            din.addEdge(S, p_base + i, 1);
        }
    }
    int add = din.maxflow(S, T);
    if (add < L - 1) {
        cout << -1 << '\n';
        return 0;
    }

    // Reconstruct assignments: for each matched proposal i, find material edge with flow 1
    vector<pair<int,int>> ans(K, {0, 0}); // per worker: (u, v)
    // We'll pop workers from the back for each material
    vector<int> ptr(U, 0); // not used; we'll use back()
    for (int i = 0; i < N; ++i) {
        int u = p_base + i;
        for (const auto &e : din.G[u]) {
            int v = e.to;
            if (v >= m_base && v < m_base + U) {
                // This edge was added with cap 1. If residual cap == 0, it carries flow 1.
                if (e.cap == 0) {
                    int mid = v - m_base;
                    // assign any unused worker of this material
                    int t = workers_by_mat[mid].back();
                    workers_by_mat[mid].pop_back();
                    ans[t] = {i + 1, A[i] + 1}; // back to 1-based cities
                    break; // proposal matched once at most
                }
            }
        }
    }

    // Output per worker
    for (int i = 0; i < K; ++i) {
        cout << ans[i].first << ' ' << ans[i].second << '\n';
    }
    return 0;
}
```