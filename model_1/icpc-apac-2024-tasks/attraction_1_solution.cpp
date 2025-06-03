#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pii> cities(n + 1);
    for (int i = 1; i <= n; i++) {
        int x, y;
        cin >> x >> y;
        cities[i] = {x, y};
    }

    vector<vector<int>> graph(n + 1);
    vector<int> deg(n + 1, 0);
    map<pii, ll> edge_to_a;

    for (int i = 0; i < m; i++) {
        int u, v, a_val;
        cin >> u >> v >> a_val;
        if (u > v) swap(u, v);
        graph[u].push_back(v);
        graph[v].push_back(u);
        deg[u]++;
        deg[v]++;
        edge_to_a[{u, v}] = a_val;
    }

    vector<unordered_set<int>> adj_set(n + 1);
    for (int i = 1; i <= n; i++) {
        adj_set[i] = unordered_set<int>(graph[i].begin(), graph[i].end());
    }

    ll best = 0;

    for (const auto& e : edge_to_a) {
        if (e.second > best) {
            best = e.second;
        }
    }

    for (const auto& e : edge_to_a) {
        int u = e.first.first;
        int v = e.first.second;
        int u0 = u;
        int v0 = v;
        int x = u;
        int y = v;

        if (deg[x] > deg[y] || (deg[x] == deg[y] && x > y)) {
            swap(x, y);
        }

        vector<int> common;
        for (int w : graph[x]) {
            if (w == y) continue;
            if (w > v0 && adj_set[y].count(w)) {
                common.push_back(w);
            }
        }

        for (int w : common) {
            ll total_tri = e.second;
            auto it1 = edge_to_a.find(minmax(u0, w));
            auto it2 = edge_to_a.find(minmax(v0, w));
            if (it1 != edge_to_a.end() && it2 != edge_to_a.end()) {
                total_tri += it1->second;
                total_tri += it2->second;
                if (total_tri > best) {
                    best = total_tri;
                }
            }
        }

        unordered_set<int> common_set(common.begin(), common.end());
        for (int w : common) {
            for (int x_val : graph[w]) {
                if (x_val > w && common_set.count(x_val)) {
                    vector<pii> edges = {
                        minmax(u0, v0),
                        minmax(u0, w),
                        minmax(u0, x_val),
                        minmax(v0, w),
                        minmax(v0, x_val),
                        minmax(w, x_val)
                    };
                    ll total_k4 = 0;
                    bool valid = true;
                    for (auto& edge : edges) {
                        auto it = edge_to_a.find(edge);
                        if (it == edge_to_a.end()) {
                            valid = false;
                            break;
                        }
                        total_k4 += it->second;
                    }
                    if (valid && total_k4 > best) {
                        best = total_k4;
                    }
                }
            }
        }
    }

    cout << best << endl;

    return 0;
}