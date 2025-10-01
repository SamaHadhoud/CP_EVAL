#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w;
};

const long long INF = (1LL<<62);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<Edge> edges(m);
    vector<vector<pair<int,int>>> g(n+1);
    g.reserve(n+1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        edges[i] = {u, v, w};
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    auto dijkstra = [&](int src) {
        vector<long long> dist(n+1, INF);
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
        dist[src] = 0;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (auto [v, w] : g[u]) {
                long long nd = d + w;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    pq.push({nd, v});
                }
            }
        }
        return dist;
    };

    vector<long long> distS = dijkstra(1);
    vector<long long> distT = dijkstra(n);

    if (distS[n] >= INF) {
        cout << -1 << '\n';
        return 0;
    }
    long long d = distS[n];

    // Build DAG of tight edges (edges that can be part of a shortest 1->n path)
    vector<vector<int>> dag(n+1);
    dag.reserve(n+1);
    vector<char> onShortest(m, 0);   // undirected edge lies on some shortest 1->n path
    for (int i = 0; i < m; ++i) {
        int u = edges[i].u, v = edges[i].v, w = edges[i].w;
        bool tight_uv = (distS[u] < INF && distT[v] < INF && distS[u] + (long long)w + distT[v] == d);
        bool tight_vu = (distS[v] < INF && distT[u] < INF && distS[v] + (long long)w + distT[u] == d);
        if (tight_uv) {
            dag[u].push_back(v);
            onShortest[i] = 1;
        } else if (tight_vu) {
            dag[v].push_back(u);
            onShortest[i] = 1;
        }
    }

    // Count number of shortest 1->n paths in DAG, capped at 2
    vector<int> order(n);
    iota(order.begin(), order.end(), 1);
    sort(order.begin(), order.end(), [&](int a, int b){
        if (distS[a] != distS[b]) return distS[a] < distS[b];
        return a < b;
    });
    vector<int> ways(n+1, 0);
    ways[1] = 1;
    for (int u : order) {
        if (ways[u] == 0) continue;
        for (int v : dag[u]) {
            int nv = ways[v] + ways[u];
            if (nv > 2) nv = 2;
            if (nv != ways[v]) ways[v] = nv;
        }
    }
    if (ways[n] >= 2) {
        cout << (2*d) << '\n';
        return 0;
    }

    // Unique shortest path: compute best second traversal using at least one non-shortest-path edge
    long long M = INF;
    for (int i = 0; i < m; ++i) {
        if (onShortest[i]) continue;
        int u = edges[i].u, v = edges[i].v, w = edges[i].w;
        if (distS[u] < INF && distT[v] < INF) {
            M = min(M, distS[u] + (long long)w + distT[v]);
        }
        if (distS[v] < INF && distT[u] < INF) {
            M = min(M, distS[v] + (long long)w + distT[u]);
        }
    }

    if (M >= INF) {
        cout << -1 << '\n';
    } else {
        cout << (d + M) << '\n';
    }
    return 0;
}