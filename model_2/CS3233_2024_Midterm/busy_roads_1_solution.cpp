#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int L, R;
    long long T;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    long long C;
    if (!(cin >> N >> M >> C)) return 0;

    vector<vector<Edge>> g(N + 1);
    for (int i = 0; i < M; ++i) {
        int A, B, L, R;
        long long T;
        cin >> A >> B >> T >> L >> R;
        g[A].push_back({B, L, R, T});
        g[B].push_back({A, L, R, T});
    }

    const long long INF = (long long)4e18;
    vector<long long> dist(N + 1, INF);
    dist[1] = 0;

    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du != dist[u]) continue;
        if (u == N) {
            cout << du << "\n";
            return 0;
        }
        long long x = du % C;
        for (const auto &e : g[u]) {
            long long wait = 0;
            if (x < e.L) wait = e.L - x;
            else if (x <= e.R) wait = 0;
            else wait = (long long)C - x + e.L;

            long long nd = du + wait + e.T;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                pq.push({nd, e.to});
            }
        }
    }

    cout << dist[N] << "\n";
    return 0;
}