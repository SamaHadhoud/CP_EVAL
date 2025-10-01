#include <bits/stdc++.h>
using namespace std;

struct Edge {
    long long u, v;
    int w;
};

struct CompEdge {
    int u, v;
    int w;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    long long M;
    if (!(cin >> N >> M)) return 0;
    vector<Edge> edges(N);
    for (int i = 0; i < N; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }
    
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });
    
    // Coordinate compression for A-values
    vector<long long> vals;
    vals.reserve(2LL * N);
    for (const auto& e : edges) {
        vals.push_back(e.u);
        vals.push_back(e.v);
    }
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    
    auto get_idx = [&](long long x) -> int {
        return int(lower_bound(vals.begin(), vals.end(), x) - vals.begin());
    };
    
    vector<CompEdge> cedges;
    cedges.reserve(N);
    for (const auto& e : edges) {
        cedges.push_back({ get_idx(e.u), get_idx(e.v), e.w });
    }
    
    vector<long long> available(vals.size(), 0);
    long long ans = 0;
    
    for (const auto& e : cedges) {
        int u = e.u, v = e.v;
        if (available[u] == 0) {
            ans += 1;
            available[u] += M;
        }
        available[u] -= 1;
        available[v] += M;
    }
    
    cout << ans << '\n';
    return 0;
}