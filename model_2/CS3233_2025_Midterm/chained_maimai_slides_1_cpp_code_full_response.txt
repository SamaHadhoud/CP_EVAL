```cpp
#include <bits/stdc++.h>
using namespace std;

struct Slide {
    int x, y;
    long long v;
};

// Dinic for max flow (used only for feasibility with lower bounds)
struct Dinic {
    struct Edge { int to, cap, rev; };
    int N;
    vector<vector<Edge>> G;
    vector<int> level, it;
    Dinic(int n = 0): N(n), G(n), level(n), it(n) {}
    void init(int n) { N = n; G.assign(n, {}); level.resize(n); it.resize(n); }
    int addEdgeGetIndex(int u, int v, int cap) {
        Edge a{v, cap, (int)G[v].size()};
        Edge b{u, 0,   (int)G[u].size()};
        int idx = (int)G[u].size();
        G[u].push_back(a);
        G[v].push_back(b);
        return idx; // index of forward edge in G[u]
    }
    void addEdge(int u, int v, int cap) { addEdgeGetIndex(u, v, cap); }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0; q.push(s);
        while(!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e: G[u]) if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
        return level[t] >= 0;
    }
    int dfs(int u, int t, int f) {
        if (!f || u == t) return f;
        for (int &i = it[u]; i < (int)G[u].size(); ++i) {
            auto &e = G[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                int ret = dfs(e.to, t, min(f, e.cap));
                if (ret) {
                    e.cap -= ret;
                    G[e.to][e.rev].cap += ret;
                    return ret;
                }
            }
        }
        return 0;
    }
    long long maxflow(int s, int t) {
        long long flow = 0;
        const int INF = 1e9;
        while (bfs(s,t)) {
            fill(it.begin(), it.end(), 0);
            int f;
            while ((f = dfs(s,t,INF)) > 0) flow += f;
        }
        return flow;
    }
};

struct OriginalEdge {
    int u, v;      // nodes in [0..15]
    int lower;     // lower bound
    int upper;     // upper bound
    int cost;      // 0 for internal, -1 for slide
    int flow;      // current flow (after feasibility and cycle-canceling)
    int dinicU, dinicIdx; // position of its forward edge in Dinic graph for reading flow
};

// Negative cycle canceling on small graph (16 nodes)
struct ResidualArc {
    int from, to;
    int cap;
    int cost;
    int eid;    // index of OriginalEdge
    bool fwd;   // true if forward residual (u->v), false if backward residual (v->u)
};

// Build and solve per window
struct WindowSolver {
    // counts c[u][v]: multiplicity of slides u->v in window
    int c[8][8];
    int outCap[8], inCap[8];
    int TOT;
    int Kneed;

    WindowSolver(): TOT(0), Kneed(0) {
        memset(c, 0, sizeof(c));
        memset(outCap, 0, sizeof(outCap));
        memset(inCap, 0, sizeof(inCap));
    }

    // Build original edges for current window
    void buildEdges(vector<OriginalEdge> &edges) {
        edges.clear();
        int N = 16; // 0..7 = vin[1..8], 8..15 = vout[1..8]
        auto vin = [](int v)->int { return v; };      // v: 0..7
        auto vout = [](int v)->int { return 8 + v; }; // v: 0..7

        int INF = max(0, TOT); // safe upper bound for internal edges
        if (INF == 0) INF = 1; // avoid zero

        // Internal edges with lower bound 1: vin[v] -> vout[v]
        for (int v = 0; v < 8; ++v) {
            OriginalEdge e;
            e.u = vin(v);
            e.v = vout(v);
            e.lower = 1;
            e.upper = INF; // enough to allow all extra cycle flow
            e.cost = 0;
            e.flow = 0;
            e.dinicU = -1; e.dinicIdx = -1;
            edges.push_back(e);
        }

        // Slide edges vout[u] -> vin[v] with capacity c[u][v], cost -1
        for (int u = 0; u < 8; ++u) {
            for (int v = 0; v < 8; ++v) {
                if (c[u][v] > 0) {
                    OriginalEdge e;
                    e.u = vout(u);
                    e.v = vin(v);
                    e.lower = 0;
                    e.upper = c[u][v];
                    e.cost = -1;
                    e.flow = 0;
                    e.dinicU = -1; e.dinicIdx = -1;
                    edges.push_back(e);
                }
            }
        }
    }

    // Create Dinic network to satisfy lower bounds and get initial feasible flow
    bool satisfyLowerBounds(vector<OriginalEdge> &edges) {
        int N = 16;
        int SS = 16, TT = 17;
        Dinic D(N + 2);
        D.init(N + 2);

        vector<int> demand(N, 0);

        // add edges with capacity (upper - lower), and record indices
        for (int i = 0; i < (int)edges.size(); ++i) {
            OriginalEdge &e = edges[i];
            int cap = e.upper - e.lower;
            int idx = D.addEdgeGetIndex(e.u, e.v, cap);
            e.dinicU = e.u;
            e.dinicIdx = idx;
            // handle demands
            demand[e.u] -= e.lower;
            demand[e.v] += e.lower;
        }

        int totalPos = 0;
        for (int i = 0; i < N; ++i) {
            if (demand[i] > 0) {
                D.addEdge(SS, i, demand[i]);
                totalPos += demand[i];
            } else if (demand[i] < 0) {
                D.addEdge(i, TT, -demand[i]);
            }
        }

        long long f = D.maxflow(SS, TT);
        if (f != totalPos) return false;

        // reconstruct flows: flow = lower + (used capacity on forward arc)
        for (auto &e : edges) {
            auto &fe = D.G[e.dinicU][e.dinicIdx];
            int cap_left = fe.cap;
            int cap_added = (e.upper - e.lower) - cap_left;
            e.flow = e.lower + cap_added;
            // assert bounds
            if (e.flow < e.lower) e.flow = e.lower;
            if (e.flow > e.upper) e.flow = e.upper;
        }
        return true;
    }

    // Build residual arcs from current original edges
    void buildResidual(const vector<OriginalEdge> &edges,
                       vector<ResidualArc> &arcs,
                       vector<vector<int>> &adj) {
        int N = 16;
        arcs.clear();
        adj.assign(N, {});
        auto addArc = [&](int u, int v, int cap, int cost, int eid, bool fwd) {
            int idx = (int)arcs.size();
            arcs.push_back({u, v, cap, cost, eid, fwd});
            adj[u].push_back(idx);
        };
        for (int i = 0; i < (int)edges.size(); ++i) {
            const auto &e = edges[i];
            // forward residual
            if (e.flow < e.upper) {
                addArc(e.u, e.v, e.upper - e.flow, e.cost, i, true);
            }
            // backward residual
            if (e.flow > e.lower) {
                addArc(e.v, e.u, e.flow - e.lower, -e.cost, i, false);
            }
        }
    }

    // Find any negative cycle in residual graph using Bellman-Ford-style method.
    // Returns list of arc indices forming a cycle; empty if none.
    vector<int> findNegativeCycle(const vector<ResidualArc> &arcs, const vector<vector<int>> &adj) {
        int N = 16;
        const int INF = 1e9;
        vector<int> dist(N, 0), inArc(N, -1), inNode(N, -1);
        int x = -1;
        // Flatten adjacency to iterate edges easily
        vector<pair<int,int>> all; all.reserve(arcs.size());
        for (int u = 0; u < N; ++u) {
            for (int idx : adj[u]) {
                all.emplace_back(u, idx);
            }
        }
        for (int it = 0; it < N; ++it) {
            x = -1;
            for (auto [u, idx] : all) {
                const auto &a = arcs[idx];
                if (a.cap <= 0) continue;
                if (dist[u] + a.cost < dist[a.to]) {
                    dist[a.to] = dist[u] + a.cost;
                    inArc[a.to] = idx;
                    inNode[a.to] = u;
                    x = a.to;
                }
            }
        }
        if (x == -1) return {}; // no negative cycle
        // Reconstruct
        int y = x;
        for (int i = 0; i < N; ++i) y = inNode[y];
        int cur = y;
        vector<int> cyc;
        do {
            int eidx = inArc[cur];
            cyc.push_back(eidx);
            cur = inNode[cur];
        } while (cur != y);
        // cyc forms a cycle in reverse order; order doesn't matter
        return cyc;
    }

    int maximizeSlides(vector<OriginalEdge> &edges) {
        // Negative cycle canceling
        while (true) {
            vector<ResidualArc> arcs;
            vector<vector<int>> adj;
            buildResidual(edges, arcs, adj);
            vector<int> cyc = findNegativeCycle(arcs, adj);
            if (cyc.empty()) break;
            int delta = INT_MAX;
            for (int idx : cyc) delta = min(delta, arcs[idx].cap);
            if (delta <= 0) break;
            // apply augmentation along the cycle
            for (int idx : cyc) {
                const auto &a = arcs[idx];
                auto &e = edges[a.eid];
                if (a.fwd) e.flow += delta;
                else       e.flow -= delta;
            }
        }
        // Count slides used
        int used = 0;
        for (auto &e : edges) if (e.cost == -1) used += e.flow;
        return used;
    }

    // Check feasibility for current window and return whether X >= Kneed
    bool feasibleAtLeastK() {
        // Early prunes
        if (TOT < max(Kneed, 8)) return false;
        for (int v = 0; v < 8; ++v) if (outCap[v] == 0 || inCap[v] == 0) return false;

        vector<OriginalEdge> edges;
        buildEdges(edges);

        // Satisfy lower bounds (each vertex used at least once)
        if (!satisfyLowerBounds(edges)) return false;

        // Maximize number of slides via negative-cycle canceling
        int X = maximizeSlides(edges);
        return X >= Kneed;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int M, K;
    if (!(cin >> M >> K)) return 0;
    vector<Slide> a(M);
    for (int i = 0; i < M; ++i) {
        cin >> a[i].x >> a[i].y >> a[i].v;
        --a[i].x; --a[i].y; // to 0-based
    }
    sort(a.begin(), a.end(), [](const Slide &A, const Slide &B){
        if (A.v != B.v) return A.v < B.v;
        if (A.x != B.x) return A.x < B.x;
        return A.y < B.y;
    });

    WindowSolver WS;
    WS.Kneed = K;

    auto addEdgeToWindow = [&](int idx) {
        int u = a[idx].x, v = a[idx].y;
        WS.c[u][v] += 1;
        WS.outCap[u] += 1;
        WS.inCap[v] += 1;
        WS.TOT += 1;
    };
    auto removeEdgeFromWindow = [&](int idx) {
        int u = a[idx].x, v = a[idx].y;
        WS.c[u][v] -= 1;
        WS.outCap[u] -= 1;
        WS.inCap[v] -= 1;
        WS.TOT -= 1;
    };

    const long long INF_ANS = (1LL<<62);
    long long ans = INF_ANS;

    int r = -1;
    for (int l = 0; l < M; ++l) {
        // expand r while current window infeasible
        while (r + 1 < M) {
            // quick early prune check for current [l..r]
            bool ok = WS.feasibleAtLeastK();
            if (ok) break;
            ++r;
            addEdgeToWindow(r);
        }
        // check once more for current [l..r]
        if (WS.feasibleAtLeastK()) {
            long long cur = a[r].v - a[l].v;
            if (cur < ans) ans = cur;
        }
        // slide left bound
        removeEdgeFromWindow(l);
        // ensure r >= l-1; r stays, typical two-pointer
    }

    if (ans == INF_ANS) cout << -1 << '\n';
    else cout << ans << '\n';
    return 0;
}
```