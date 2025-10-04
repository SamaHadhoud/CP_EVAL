```cpp
#include <bits/stdc++.h>
using namespace std;

static inline uint64_t key_pair(int u, int v) {
    if (u > v) swap(u, v);
    return (uint64_t)u << 32 | (uint64_t)v;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const long long PEN = 1000000LL;

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Read and ignore coordinates
    for (int i = 0; i < n; ++i) {
        int x, y; cin >> x >> y;
    }

    vector<int> U(m), V(m);
    vector<int> W(m);
    vector<vector<int>> adj(n + 1);
    unordered_map<uint64_t, int> eid;
    eid.reserve((size_t)m * 2 + 10);

    int max_edge = 0;
    for (int i = 0; i < m; ++i) {
        int u, v, a; cin >> u >> v >> a;
        U[i] = u; V[i] = v; W[i] = a;
        adj[u].push_back(v);
        adj[v].push_back(u);
        eid[key_pair(u, v)] = i;
        if (a > max_edge) max_edge = a;
    }

    // Degeneracy ordering via lazy min-heap
    vector<int> deg(n + 1);
    for (int u = 1; u <= n; ++u) deg[u] = (int)adj[u].size();
    vector<char> removed(n + 1, 0);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    for (int u = 1; u <= n; ++u) pq.push({deg[u], u});

    vector<int> order;
    order.reserve(n);
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (removed[u]) continue;
        // d is >= current deg[u]; but due to lazy updates, deg[u] == minimal degree
        removed[u] = 1;
        order.push_back(u);
        for (int v : adj[u]) if (!removed[v]) {
            --deg[v];
            pq.push({deg[v], v});
        }
    }

    // Map vertex to its index in degeneracy order
    vector<int> pos(n + 1);
    for (int i = 0; i < n; ++i) pos[order[i]] = i;

    // Orient edges from earlier to later; store out[u] as (neighbor, edge-id)
    vector<vector<pair<int,int>>> out(n + 1);
    out.reserve(n + 1);
    for (int e = 0; e < m; ++e) {
        int u = U[e], v = V[e];
        if (pos[u] < pos[v]) out[u].push_back({v, e});
        else out[v].push_back({u, e});
    }

    // Best two leg sums per undirected edge for diamonds
    vector<long long> best1(m, -1), best2(m, -1);
    auto upd2 = [&](int e, long long val) {
        if (val > best1[e]) {
            best2[e] = best1[e];
            best1[e] = val;
        } else if (val > best2[e]) {
            best2[e] = val;
        }
    };

    auto get_eid = [&](int a, int b) -> int {
        auto it = eid.find(key_pair(a, b));
        if (it == eid.end()) return -1;
        return it->second;
    };

    long long ans = 0;
    ans = max(ans, (long long)max_edge); // k=2 case (single edge)
    // k=1 case (singleton) gives 0, already covered.

    // Enumerate per vertex in degeneracy order
    for (int idx = 0; idx < n; ++idx) {
        int u = order[idx];
        auto &L = out[u];
        int sz = (int)L.size();

        // Triangles, diamonds leg stats, and K5 - one edge
        if (sz >= 2) {
            for (int i = 0; i < sz; ++i) {
                int v = L[i].first;
                int e_uv = L[i].second;
                long long w_uv = W[e_uv];
                for (int j = i + 1; j < sz; ++j) {
                    int w = L[j].first;
                    int e_uw = L[j].second;
                    long long w_uw = W[e_uw];

                    int e_vw = get_eid(v, w);
                    if (e_vw == -1) continue; // no triangle
                    long long w_vw = W[e_vw];

                    // Triangle score
                    long long tri = w_uv + w_uw + w_vw;
                    if (tri > ans) ans = tri;

                    // Update diamond leg sums for all three edges of the triangle
                    // Base edge {v,w}, legs via u: (u,v) + (u,w)
                    upd2(e_vw, w_uv + w_uw);
                    // Base edge {u,v}, legs via w: (u,w) + (v,w)
                    upd2(e_uv, w_uw + w_vw);
                    // Base edge {u,w}, legs via v: (u,v) + (v,w)
                    upd2(e_uw, w_uv + w_vw);

                    // K5 minus one edge (two apex vertices over base triangle u-v-w)
                    // Candidates x in out[u]\{v,w} with edges (x,v) and (x,w)
                    int cand_arr[5];
                    int csz = 0;
                    for (int t = 0; t < sz; ++t) {
                        int x = L[t].first;
                        if (x == v || x == w) continue;
                        int e_xv = get_eid(x, v);
                        if (e_xv == -1) continue;
                        int e_xw = get_eid(x, w);
                        if (e_xw == -1) continue;
                        cand_arr[csz++] = x;
                    }
                    for (int a = 0; a < csz; ++a) {
                        int x = cand_arr[a];
                        int e_ux = get_eid(u, x); // exists
                        int e_vx = get_eid(v, x); // exists
                        int e_wx = get_eid(w, x); // exists
                        long long sum_x = W[e_ux] + W[e_vx] + W[e_wx];
                        for (int b = a + 1; b < csz; ++b) {
                            int y = cand_arr[b];
                            int e_uy = get_eid(u, y);
                            int e_vy = get_eid(v, y);
                            int e_wy = get_eid(w, y);
                            long long sum_y = W[e_uy] + W[e_vy] + W[e_wy];
                            long long s5 = tri + sum_x + sum_y - PEN;
                            if (s5 > ans) ans = s5;
                        }
                    }
                }
            }
        }

        // K4 with u as smallest (triples in out[u])
        if (sz >= 3) {
            for (int a = 0; a < sz; ++a) {
                int v = L[a].first;
                int e_uv = L[a].second;
                for (int b = a + 1; b < sz; ++b) {
                    int w = L[b].first;
                    int e_uw = L[b].second;
                    int e_vw = get_eid(v, w);
                    if (e_vw == -1) continue;
                    for (int c = b + 1; c < sz; ++c) {
                        int x = L[c].first;
                        int e_ux = L[c].second;
                        int e_vx = get_eid(v, x);
                        if (e_vx == -1) continue;
                        int e_wx = get_eid(w, x);
                        if (e_wx == -1) continue;

                        long long s4 = (long long)W[e_uv] + W[e_uw] + W[e_ux]
                                     + (long long)W[e_vw] + W[e_vx] + W[e_wx];
                        if (s4 > ans) ans = s4;
                    }
                }
            }
        }
    }

    // Diamonds: two triangles sharing an edge
    for (int e = 0; e < m; ++e) {
        if (best2[e] >= 0) {
            long long score_d = (long long)W[e] + best1[e] + best2[e] - PEN;
            if (score_d > ans) ans = score_d;
        }
    }

    cout << ans << '\n';
    return 0;
}
```