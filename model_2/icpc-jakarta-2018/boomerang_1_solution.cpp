#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<Edge> E(M);
    vector<vector<pair<int,int>>> adj(N + 1);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        E[i] = {u, v};
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    
    auto other = [&](int e, int v) -> int {
        return E[e].u ^ E[e].v ^ v;
    };
    
    vector<int> tin(N + 1, 0), parent(N + 1, -1), parent_edge(N + 1, -1), idx(N + 1, 0);
    vector<char> vis(N + 1, 0);
    vector<char> used(M, 0);
    vector<vector<int>> P(N + 1); // pending edges to be paired at each vertex
    vector<array<int,3>> ans;
    
    int timer = 0;
    for (int s = 1; s <= N; ++s) {
        if (vis[s]) continue;
        // iterative DFS
        vector<int> st;
        vis[s] = 1;
        tin[s] = ++timer;
        idx[s] = 0;
        parent[s] = -1;
        parent_edge[s] = -1;
        st.push_back(s);
        
        while (!st.empty()) {
            int v = st.back();
            if (idx[v] < (int)adj[v].size()) {
                auto [u, e] = adj[v][idx[v]++];
                if (e == parent_edge[v]) continue;
                if (!vis[u]) {
                    vis[u] = 1;
                    tin[u] = ++timer;
                    idx[u] = 0;
                    parent[u] = v;
                    parent_edge[u] = e;
                    st.push_back(u);
                } else {
                    // back edge to ancestor (in undirected DFS)
                    if (tin[u] < tin[v]) {
                        if (!used[e]) P[v].push_back(e);
                    }
                }
            } else {
                // post-processing at v
                if (parent_edge[v] != -1 && (P[v].size() & 1)) {
                    P[v].push_back(parent_edge[v]);
                }
                while (P[v].size() >= 2) {
                    int e1 = P[v].back(); P[v].pop_back();
                    int e2 = P[v].back(); P[v].pop_back();
                    int a = other(e1, v);
                    int b = other(e2, v);
                    ans.push_back({a, v, b});
                    used[e1] = used[e2] = 1;
                }
                // propagate unused parent edge upward
                if (parent_edge[v] != -1 && !used[parent_edge[v]]) {
                    int p = parent[v];
                    P[p].push_back(parent_edge[v]);
                }
                st.pop_back();
            }
        }
    }
    
    cout << (int)ans.size() << '\n';
    for (auto &t : ans) {
        cout << t[0] << ' ' << t[1] << ' ' << t[2] << '\n';
    }
    return 0;
}