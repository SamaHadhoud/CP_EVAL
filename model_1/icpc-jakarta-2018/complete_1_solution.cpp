#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    if (n == 0) {
        cout << 0 << endl;
        return 0;
    }

    vector<int> deg(n+1, 0);
    vector<pair<int, int>> edges;
    int total_edges = n * (n-1) / 2;

    for (int i=0; i<m; i++) {
        int a, b;
        cin >> a >> b;
        if (a > b) swap(a, b);
        edges.push_back({a, b});
        deg[a]++;
        deg[b]++;
    }

    if (m == total_edges) {
        cout << 0 << endl;
        return 0;
    }

    auto check = [&](int k) -> bool {
        vector<vector<char>> added(n+1, vector<char>(n+1, 0));
        vector<vector<char>> pushed(n+1, vector<char>(n+1, 0));
        vector<int> deg_local = deg;
        for (auto &e : edges) {
            added[e.first][e.second] = 1;
        }

        int added_count = m;
        queue<pair<int, int>> Q;

        for (int u=1; u<=n; u++) {
            for (int v=u+1; v<=n; v++) {
                if (added[u][v]) continue;
                if (deg_local[u] + deg_local[v] >= k) {
                    pushed[u][v] = 1;
                    Q.push({u, v});
                }
            }
        }

        while (!Q.empty()) {
            auto edge = Q.front();
            Q.pop();
            int u = edge.first;
            int v = edge.second;

            if (added[u][v]) 
                continue;

            added[u][v] = 1;
            added_count++;
            deg_local[u]++;
            deg_local[v]++;

            for (int w=1; w<=n; w++) {
                if (w == u || w == v) continue;
                int a = min(u, w);
                int b = max(u, w);
                if (!added[a][b] && !pushed[a][b]) {
                    if (deg_local[u] + deg_local[w] >= k) {
                        pushed[a][b] = 1;
                        Q.push({a, b});
                    }
                }
                int c = min(v, w);
                int d = max(v, w);
                if (!added[c][d] && !pushed[c][d]) {
                    if (deg_local[v] + deg_local[w] >= k) {
                        pushed[c][d] = 1;
                        Q.push({c, d});
                    }
                }
            }
        }

        return (added_count == total_edges);
    };

    int low = 0;
    int high = 2 * (n - 1);
    int ans = 0;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (check(mid)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    cout << ans << endl;
    return 0;
}